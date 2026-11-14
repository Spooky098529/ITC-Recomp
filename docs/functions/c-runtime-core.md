# Known functions: C runtime core

The parts of the game's statically linked C runtime (Microsoft's, for
the Xbox 360) that sit over kernel imports, and the XAPI pieces beside
them it calls: the per-thread data over the kernel's TLS (`_mtinit`,
`_getptd` and their neighbors), the lock table and its critical
sections (`_mtinitlocks`, `__crtInitCritSecAndSpinCount`, the stream
and file handle locks), `errno` and the error paths every C runtime
function shares (`_errno`, `_dosmaperr`, `_invalid_parameter`), the
exit, abort and C++ exception paths (`_amsg_exit`, `abort`, `_purecall`,
`terminate`, `_CxxThrowException`, the unhandled exception filters),
the streams (`fclose`, `fflush`, `fputs`, `_flsbuf`, their buffers and
`_initstdio`) and the low-level I/O under them (`_close`, `_write`,
`_lseeki64`, the handle table), the float formatting helpers the
conversion table points to (%e, %a, %g), and the start-up that runs
them (`_cinit`, `_initterm_e`, the thread notify routines). It sits at
about `825A8000` to `825BAFFF`, between the rest of the C runtime
([`c-runtime.md`](c-runtime.md)) and the kernel import thunks, with
XAPI's thread and exception pieces at `82297870`-`82297BF8`. A
separate file from `c-runtime.md` because that one was mapped as the C
runtime "as seen in play", without what calls kernel imports ("Not
here" in its intro, left for the core until 2026-11-14); since "B7's
goal" (`../loops/b7-loop.md`, the user's call, 2026-11-14) these
wrappers are ported too, calling the imports as the original does.
Mapped by the B7 loop (`../loops/b7-loop.md`) from the static map's top
blockers (`../loops/static-map.md`, 2026-11-14): `_getptd_noexit`
(`sub_825AA4C0`) and `__set_flsgetvalue` (`sub_825AA400`) unblock ~305
functions, as every C runtime function that can set `errno` reaches
them.

Names are the Microsoft C runtime's (Visual C++ 2005's sources, which
the Xbox 360 XDK's runtime follows) matched by structure: the imports
called, the tables and constants read, the error numbers, the lock
numbers (1 the stream table's scan, 10 the handle table, 16 + n the
first 20 streams, 0 the signals, 8 the exit lock). "Probably" where the
match is by shape only. Almost nothing here ran in play: only the
start-up (`_mtinit`, `_mtinitlocks`, `_ioinit`, `_initstdio`, `_cinit`,
the thread start and its notify routines) and `_getptd` from `rand`.
The rest (stdio over files, the error paths, the exit) is reachable but
never taken: the game writes no C runtime files, every error path is
behind a check that passed, and closing the window ends the title
without the exit path.

Shared with other files (their rows stay there): `doexit`
(`sub_825AA218`) and its unlock (`sub_825AA338`), `_lock`
(`sub_825B3430`), `_unlock` (`sub_825B32D0`), `_init_pointers`
(`sub_825AA398`) and its setters, `memset`, `memcpy`, `strcpy_s`,
`_output_l`, `__iob_func`, `_cfltcvt_l`, `_cftof_l`, `_fptostr`,
`_fltout2`, `__strgtold12_l`, `_ld12tod` and `rand` in
[`c-runtime.md`](c-runtime.md); `malloc`, `calloc`, `free`
(`sub_825A9A90`, `sub_825AC320`, `sub_825A9B58`) in
[`heap.md`](heap.md); `_write_nolock` (`sub_825B0F48`, named `_write`
there), `_commit` (`sub_825B4828`), `_lseeki64_nolock` (`sub_825B6500`,
named `_lseek` there), `SetEndOfFile` (`sub_825B9D30`) and
`FlushFileBuffers` (`sub_825BA510`) in
[`files-and-saves.md`](files-and-saves.md); `SetEvent`
(`sub_825BA1E0`) in [`video-output.md`](video-output.md); `xstart`
(`0x82296C80`), the CheckHeap (`sub_82297CE0`) and PAL-50 check
(`sub_82296AB0`) it calls in [`system-services.md`](system-services.md);
`GetLastError` (`sub_82295F18`), `SetLastError` (`sub_82296FD0`),
`SetLastErrorFromStatus` (`sub_82296E88`), `OutputDebugStringA`
(`sub_82296270`) and the current thread's id (`sub_825B9A60`) in
[`functions.md`](functions.md); the shared "returns 0" (`sub_825C8388`,
here in `_isatty`'s place) in
[`vehicle-accessors.md`](vehicle-accessors.md); the empty function
(`sub_82106540`) in [`audio.md`](audio.md). Not here (leads in
`../loops/leads.md`): the static initializers `_cinit` runs
(`825D9140`-`825DB498`) and the C++ library's throws above
`_CxxThrowException` (`std::_Xran` `sub_825A6B18`, `std::string`'s
`length_error`). The globals worked out are in `../known-data.md`.

Ours (B7 loop, 2026-11-14): plain C++ in `itc_recomp/src/c_runtime/core.cpp`
over the same guest words, the kernel imports called as the originals
call them through `c_runtime::core::Calls` (guest calls the adapters in
`src/hooks/c_runtime_core.cpp` make; the last error through
`last_error::`, calloc, malloc and free the native heap's when it's on the
layer), behind `--native_c_runtime_core`. Harness `c_runtime_core`: the
cases give both sides the same fresh state (fresh TLS slots, the main
thread's slots set per case, test blocks in the lock table's entries not
made at boot) and put the game's back right after; frees are compared by
whether the allocation layer still holds each block.

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
| `sub_822979E0` | `_cinit` | | r3 = 1 (from `xstart`; not read). When `[0x8207239C]` (`_FPinit`, holding `0x825A6E48`, the float conversion table's set-up in `c-runtime.md`) isn't 0, calls it; then `_initterm_e` inline over the C initializers `0x82600284`-`0x82600290` (`_initstdio` `sub_825AD4D0`, `__initmbctable` `sub_825B6958`; stops on a nonzero r3 and returns it), then `_initterm` inline over the C++ initializers `0x82600010`-`0x82600280` (entries 0 or -1 skipped): ~150 static initializers at `825D9140`-`825DB498` (a lead), `__CxxSetUnhandledExceptionFilter` (`sub_825DAC58`) among them; returns 0. Seen in play (once at boot). Read in full. (`c-runtime.md`'s row for `sub_825A6E48` gives the tables as `0x8263xxxx`: they are at `0x8260xxxx`, checked in the image.) |
| `sub_82297AC0` | `_initterm_e` (probably XAPI's own, over its table) | | No arguments: calls each nonzero word of `0x82600000`-`0x8260000C` (`__onexitinit` `sub_825A6F00` in `c-runtime.md`, `_ioinit` `sub_825B4668`) until one returns nonzero; returns that, or 0. From `xstart` after `_mtinit`. Seen in play (once at boot). Read in full. |
| `sub_825DAC58` | `__CxxSetUnhandledExceptionFilter` | | A C++ initializer (entry of `_cinit`'s table at `0x82600020`): `atexit(0x825B4D90)` (`__CxxRestoreUnhandledExceptionFilter`, `atexit` `sub_825A7068` in `c-runtime.md`); `[0x82971348]` = `SetUnhandledExceptionFilter(0x825B4CD0)` (`__CxxUnhandledExceptionFilter`; the previous filter, 0 at boot). Seen in play (once at boot). Read in full. |
| `sub_825AA708` | `_mtinit` | | No arguments; returns 1, or 0 after `_mtterm`. Sets the FLS stand-ins: `[0x8297115C]` = `0x825AA3F8` (TlsAlloc), `[0x82971160]` = `0x825DBBF4` (`KeTlsGetValue`'s thunk), `[0x82971164]` = `0x825DBC04` (`KeTlsSetValue`'s), `[0x82971168]` = `0x825DBC24` (`KeTlsFree`'s); `[0x8266B7B8]` (`__getvalueindex`) = `KeTlsAlloc`, its value set to the getter (`KeTlsSetValue`); `_init_pointers` (`sub_825AA398`, `c-runtime.md`); `_mtinitlocks` (`sub_825B31D8`); `[0x8266B7B4]` (`__flsindex`) = the allocator through `[0x8297115C]` (with `_freefls` `0x825AA5C0`, unused by `TlsAlloc`); the main thread's data: `calloc(1, 196)` (`sub_825AC320`, `heap.md`), stored through `[0x82971164]`, +20 = 1 (`_holdrand`), +92 = `0x8266BE60` (`_pxcptacttab`, `_XcptActTab`), +0 = the thread's id (`sub_825B9A60`), +4 = -1 (`_thandle`); `[0x82971158]` = `0x825B3650` and `XRegisterThreadNotifyRoutine(0x82971150, 1)` (`sub_82297B98`). From `xstart` (first after the PAL-50 check). Seen in play (once at boot). Read in full. Calls `KeTlsAlloc`, `KeTlsSetValue` directly and the others through the stand-ins (B7's goal: ported, calling the imports as it does). |
| `sub_825AA3F8` | `__crtTlsAlloc` (probably) | Overridden | A branch to `KeTlsAlloc` (r3, the FLS callback, ignored). Called only through `[0x8297115C]` by `_mtinit`. Seen in play (once at boot). Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::CrtTlsAlloc` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), `KeTlsAlloc` called as the original does; the adapter stays (reached through `[0x8297115C]` from the recompiled `_mtinit`). Harness `c_runtime_core` (8 cases: each side's slot compared, then freed). |
| `sub_825AA400` | `__set_flsgetvalue` | Native | No arguments: when the TLS slot `[0x8266B7B8]` reads 0 on this thread (`KeTlsGetValue`), stores the getter `[0x82971160]` there (`KeTlsSetValue`); returns the slot's value as read. Leaf but for the imports. From `_getptd_noexit`. Seen in play. Read in full. `rand`'s native version (`c_runtime::ThreadData`, `c-runtime.md`) already does its reads natively. Ours (B7 loop, 2026-11-14): `c_runtime::core::SetFlsGetValue` (`src/c_runtime/core.cpp`), the TLS imports called as the original does; no adapter (only `_getptd_noexit` calls it; `--native_c_runtime_core` switches that caller). Harness `c_runtime_core` (24 cases on the main thread's getter slot: 0, the game's, random). |
| `sub_825AA448` | `_mtterm` | Overridden | No arguments: when `__flsindex` (`[0x8266B7B4]`) isn't -1, frees it through `[0x82971168]` and sets -1; when `__getvalueindex` (`[0x8266B7B8]`) isn't -1, `KeTlsFree` and -1; then `_mtdeletelocks` (`sub_825B3260`). From `_mtinit`'s failure paths. Not seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::MtTerm` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), `KeTlsFree` and `[0x82971168]` called as the original does. Harness `c_runtime_core` (12 cases: each index -1 or a fresh slot, put back after; the slots' freeing checked by allocating again; the lock table as `_mtdeletelocks`'). |
| `sub_825AA4C0` | `_getptd_noexit` | Overridden | No arguments; returns the calling thread's C runtime data (196 bytes), or 0. Saves the last error (`GetLastError` `sub_82295F18`); `__set_flsgetvalue`; the getter (the TLS value at `[0x8266B7B8]`) called with `__flsindex`; when 0: `calloc(1, 196)`, stored with `[0x82971164]` (`KeTlsSetValue`), and set up as `_mtinit` does (+20 = 1, +92 = `0x8266BE60`, +0 the thread's id, +4 = -1), or freed (`free`) when the store fails; puts the last error back (`SetLastError` `sub_82296FD0`). From `_errno`, `__doserrno`, `_dosmaperr`, `_getptd`, `raise`. Seen in play (through `_getptd` from `rand`). Read in full. The top blocker of the static map (with `__set_flsgetvalue`). Ours (B7 loop, 2026-11-14): `c_runtime::core::GetptdNoexit` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`): the TLS imports and the getter called as the original does, the last error through `last_error::`, calloc and free the native heap's (the game's when it isn't on the layer), the thread's id `CurrentThreadId`. Harness `c_runtime_core` (40 cases on the main thread: its data there, none (each side's new block compared by contents), none with the store failing; the getter slot empty or set; random last errors). |
| `sub_825AA580` | `_getptd` | | `_getptd_noexit`; when 0, `_amsg_exit(16)` (`_RT_THREAD`, `sub_825AA158`). Returns the data. From `rand` (`sub_825A9BB8`, `c-runtime.md`, whose native version reads the same words) and `terminate`. Seen in play. Read in full. |
| `sub_825AA5C0` | `_freefls` | Native | r3 = a thread's data (or 0: nothing): frees (`free`) its buffers at +36, +44, +52, +60, +68, +72 when set, +92 unless it's `_XcptActTab` (`0x8266BE60`), then the data. The FLS callback `_mtinit` passes; from `_freeptd`. Seen in play (from `_freeptd`). Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::FreeFls` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), free the native heap's. Native (2026-11-14): its adapter removed, as its only caller in play, `_freeptd`, is native and calls the plain C++ (no game call seen in an attract race); `--native_c_runtime_core` now switches only that caller. Harness `c_runtime_core` (40 cases: blocks from the layer, compared by whether it still holds them). |
| `sub_825AA670` | `_freeptd` | Overridden | r3 = a thread's data, or 0 for the calling thread's (read through the getter, when the TLS slot is set). When `__flsindex` isn't -1: the FLS value set to 0 (`[0x82971164]`) and `_freefls`; then the TLS slot `[0x8266B7B8]` set to 0 (`KeTlsSetValue`). From the thread notify routine (`sub_825B3650`, at a thread's end). Seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::FreePtd` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), the TLS imports and `[0x82971164]` called as the original does. Harness `c_runtime_core` (32 cases on the main thread's slots with test blocks: r3 the block, r3 0 with it in the data slot, the getter slot empty, `__flsindex` -1; frees compared through the layer). |
| `sub_825B3650` | the C runtime's thread notify routine (probably) | | r3 = 1 at a thread's start, 0 at its end (XAPI's thread notify call, `sub_82297B38`). Start: calls `[0x820723A0]` when set; end: `[0x820723A4]` when set, then `_freeptd(0)`. Both words hold the empty `sub_82106540` (`audio.md`) in the image. Registered by `_mtinit` (`0x82971150`'s +8). Seen in play. Read in full. |
| `sub_825B31D8` | `_mtinitlocks` | | No arguments: for each of the 36 entries of the lock table (`0x8266BD40`, 8 bytes) whose +4 is 1, its critical section is the next 28 bytes of `0x829711A0` (stored at +0) and `__crtInitCritSecAndSpinCount(cs, 4000)` (`sub_825B35A0`); returns 1, or 0 with that entry's +0 cleared when one fails. From `_mtinit`. Seen in play (once at boot). Read in full. |
| `sub_825B3260` | `_mtdeletelocks` | Native | No arguments: frees (`free`) every lock table entry's critical section that is set and not made at boot (+4 not 1), and clears +0 (no `RtlDeleteCriticalSection`: the kernel has none). From `_mtterm`. Not seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::MtDeleteLocks` (`src/c_runtime/core.cpp`); no adapter (only `_mtterm` calls it; `--native_c_runtime_core` switches that caller). Harness `c_runtime_core` (16 cases: the entries not made at boot, all 0 in play, given test blocks for the case, then put back). |
| `sub_825B35A0` | `__crtInitCritSecAndSpinCount` | Overridden | r3 = a critical section, r4 = a spin count: calls the initializer at `[0x8297132C]` (set to `sub_825B3570` the first time when 0; `_init_pointers` clears it at boot), inside a `__try` (the frame's +80 the result); returns its result. From `_mtinitlocks`, `__lock_fhandle` and the C runtime's lock maker `sub_825B32F0` (`c-runtime.md`'s `_lock`, never seen). Seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::InitCritSecAndSpinCount` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`); the `__try` left out (nothing to catch: the initializer is `sub_825B3570`'s plain C++, or a guest call for another). Seen running at boot. Harness `c_runtime_core` (24 cases on a scratch critical section, `[0x8297132C]` 0 or set). |
| `sub_825B3570` | `__crtInitCritSecNoSpinCount` (probably) | Overridden | `RtlInitializeCriticalSection(r3)` (the spin count ignored); returns 1. Called only through `[0x8297132C]`. Seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::CritSecNoSpinCount` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), `RtlInitializeCriticalSection` as the original; the adapter stays (reached through `[0x8297132C]`). Harness `c_runtime_core` (24 cases). |
| `sub_82297B98` | `XRegisterThreadNotifyRoutine` | Overridden | r3 = a notify block (+0 and +4 the list links, +8 the routine), r4 = 1 to add, 0 to remove: under the critical section `0x82638780` (`RtlEnterCriticalSection`, `RtlLeaveCriticalSection`), linked at the tail of the list at `0x8263879C` or unlinked. From `_mtinit`. Seen in play (once at boot). Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::RegisterThreadNotifyRoutine` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), the list's critical section entered and left as the original. Seen running at boot. Harness `c_runtime_core` (8 cases: a scratch block added and removed on the game's list, the harness holding the lock; native and original mixed). |
| `sub_82297B38` | `XapiCallThreadNotifyRoutines` (probably) | Overridden | r3 = 1 (a thread's start) or 0 (its end): under the same critical section, calls every block's routine (+8) on the list at `0x8263879C` with r3. From `xstart` (1, for the main thread) and the thread start (`sub_825BA558`). Seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::CallThreadNotifyRoutines` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), each routine a guest call. Seen running (each thread's start). Harness `c_runtime_core` (24 cases: test lists of 0 to 3 blocks swapped in under the lock, routines the empty function or `SetUnhandledExceptionFilter`, whose word is compared). |
| `sub_825BA558` | `XapiThreadStartup` (probably) | | r3 = the thread's routine, r4 = its parameter: the notify routines with 1, the routine (r4 as r3), the notify routines with 0, then `ExTerminateThread` with the routine's result, inside a `__try` whose handler isn't read. The start address `CreateThread` (`sub_825BA380`, `app-start-up.md`) gives the kernel. Seen in play (each thread made through `CreateThread`). Read in full. |
| `sub_825A81E0` | `_errno` | Native | Returns the address of the calling thread's `errno` (its data +8, through `_getptd_noexit`), or `0x8266B658` when there is no data. From every C runtime function with an error path (EINVAL 22, EBADF 9, ERANGE 34): 22 direct callers in the static map. Not seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::Errno` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), over the native `_getptd_noexit`. Native (2026-11-14): its adapter removed, as its callers in play (`memmove_s`, `sprintf`, `_vsnprintf_l`, `_output_l`) are native and reach it only through their originals' error paths, which run the original (no game call seen in an attract race). Harness `c_runtime_core` (16 cases on the main thread: its data, none with the store failing). |
| `sub_825A8218` | `__doserrno` | Overridden | As `_errno` with +12, or `0x8266B65C`. From the low-level I/O (`_close`, `_write`, `_commit`, `_lseeki64`, the handle table). Not seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::DosErrno` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`). Harness `c_runtime_core` (16 cases, as `_errno`'s). |
| `sub_825A8250` | `_dosmaperr` | Overridden | r3 = a Windows error: stored as `_doserrno`, then `errno` = `_get_errno_from_oserr(r3)`. From `_close_nolock`, `_write_nolock`, `_lseeki64_nolock`. Not seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::DosMapErr` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`), `_getptd_noexit` twice as the original. Harness `c_runtime_core` (116 errors: the table's, the ranges' edges, random; the main thread's data or the fallback words). |
| `sub_825A8178` | `_get_errno_from_oserr` | Overridden | r3 = a Windows error: its pair in the 45-entry table at `0x8266B4F0` (error, errno), else 13 (EACCES) for 19 to 36, 8 (ENOEXEC) for 188 to 202, 22 (EINVAL) otherwise. Leaf. From `_dosmaperr` and `free`'s failure path (`heap.md`). Not seen in play. Read in full. Ours (B7 loop, 2026-11-14): `c_runtime::core::GetErrnoFromOsErr` (`src/c_runtime/core.cpp`, `--native_c_runtime_core`); the adapter stays for `free`'s failure path (recompiled). Harness `c_runtime_core` (116 errors). |
| `sub_825AFCC8` | `_invalid_parameter` | | r3-r7 (expression, function, file, line; all 0 in this build): calls the handler at `[0x82978320]` when set (`_init_pointers` clears it); else `_crt_debugger_hook(2)` and a trap (`twi 31,r0,22`). From 26 functions' checks (null streams, bad handles, bad buffers). Not seen in play. Read in full. |
| `sub_825B68B8` | `_crt_debugger_hook` | | r3 (ignored): `[0x8297820C]` = 0 (`_debugger_hook_dummy`, for a debugger's breakpoint). Leaf. From `_invalid_parameter` and `_invoke_watson`. Not seen in play. Read in full. |
| `sub_825AFD10` | `_invoke_watson` (probably) | | An exception record for `0xC000000D` (STATUS_INVALID_PARAMETER) and a zeroed 2,624-byte context on the stack (`memset`), `SetUnhandledExceptionFilter(0)`, `UnhandledExceptionFilter`; `_crt_debugger_hook(2)` when that returns 0; then `KeBugCheck(30)`. From `_cftoe2_l` and `_fltout2` (`c-runtime.md`; their `strcpy_s` checks). Not seen in play. Read in full. |
| `sub_825AA158` | `_amsg_exit` | | r3 = a run-time error number: `_FF_MSGBANNER`, `_NMSG_WRITE(r3)`, then the exit routine at `[0x8266B7B0]` (`_aexit_rtn`: `sub_825AA1A0` in the image, `_exit` `sub_825AA378` once `_init_pointers` ran) with 255. From `_getptd` (16) and `_fptrap` (2). Not seen in play. Read in full. |
| `sub_825AA1A0` | the initial `_aexit_rtn` (probably) | | `KeBugCheck(0)`. No direct callers: the image's value of `[0x8266B7B0]`, replaced at boot. Not seen in play. Read in full. |
| `sub_825AEDA0` | `_NMSG_WRITE` | | r3 = a run-time error number: its message from the 23-entry table at `0x8266BA88` (number, text), to `OutputDebugStringA` (`sub_82296270`, which drops it); nothing for a number not in it. From `_amsg_exit`, `_FF_MSGBANNER`, `abort` (10) and `_purecall` (25). Not seen in play. Read in full. |
| `sub_825AEDE8` | `_FF_MSGBANNER` | | `_NMSG_WRITE(252)` then `_NMSG_WRITE(255)` (the banner's lines). From `_amsg_exit`. Not seen in play. Read in full. |
| `sub_825B64F8` | `_fptrap` | | `_amsg_exit(2)` (`_RT_FLOAT`). No direct callers: in the float conversion table's image values and the printf engines' (`sub_825A6DB0`, `_output_l`, `_woutput_l`; replaced at start-up). Not seen in play. Read in full. |
| `sub_825AA1B8` | `_initterm` | | r3 = begin, r4 = end: calls each nonzero word between them. Leaf but for the calls. From `doexit` (`sub_825AA218`, `c-runtime.md`: the pre-terminators `0x82600294`-`0x8260029C`, `_endstdio`, and the terminators `0x826002A0`-`0x826002A4`). Not seen in play. Read in full. |
| `sub_825AA378` | `_exit` | | `doexit(r3, 1, 0)` (`sub_825AA218`). Stored as `_aexit_rtn` by `_init_pointers`; from `abort` and `raise` (3). Not seen in play. Read in full. |
| `sub_825AA388` | `_cexit` | | `doexit(0, 0, 1)`. From `xstart` after `main` returns (it doesn't in play). Not seen in play. Read in full. |
| `sub_825AD070` | `abort` | | With bit 0 of `__abort_behavior` (`[0x8266B7C4]`, 3 in the image): `_NMSG_WRITE(10)`; when the SIGABRT action (`sub_825B49B0`) isn't 0, `raise(22)`; with bit 1: an exception record for `0x40000015` (STATUS_FATAL_APP_EXIT) and a zeroed context, `SetUnhandledExceptionFilter(0)`, `UnhandledExceptionFilter`; then `_exit(3)`. From `_Debug_message` (`sub_825A6948`) and `_purecall`. Not seen in play. Read in full. |
| `sub_825AD120` | `_set_abort_behavior` | | r3 = flags, r4 = mask: `__abort_behavior` = (old & ~mask) \| (flags & mask); returns the old value. Leaf. From `_purecall` (0, 1). Not seen in play. Read in full. |
| `sub_825B49B0` | `__get_sigabrt` (probably) | | Returns `[0x82971340]` (SIGABRT's action). Leaf. From `abort`. Not seen in play. Read in full. |
| `sub_825B49C8` | `raise` | | r3 = a signal: 2, 6, 15, 21 and 22 (the actions at `0x82971338` and the words after it, under `_lock(0)`), or 4, 8 and 11 (the thread's exception action table, `_getptd_noexit`'s +92, 12 bytes an entry); other numbers EINVAL (`_invalid_parameter`). SIG_IGN returns 0, SIG_DFL `_exit(3)`; else the action reset and called with the signal, the lock left (`sub_825B4C8C`). From `abort`. Not seen in play. Not read in detail (212 lines). |
| `sub_825B4C8C` | `raise`'s `__finally` | | `_unlock(0)` when `raise` took the lock (its frame's r28). Not seen in play. Read in full. |
| `sub_825A7098` | `_purecall` | | Calls the handler at `[0x82971178]` when set (`_init_pointers` clears it); `_NMSG_WRITE(25)`, `_set_abort_behavior(0, 1)`, `abort`. No direct callers: the pure virtual slots of 113 vtables (the frame driver's interface's slots 1 to 3, `frame-driver.md`; the morph jobs', `morph-jobs.md`), so it unblocks ~190 functions in the static map. Not seen in play. Read in full. |
| `sub_825B3498` | `terminate` | | Calls the thread's terminate handler (`_getptd`'s +108) when set, inside a `__try`; then `KeBugCheck(0xC0000144)` (STATUS_UNHANDLED_EXCEPTION). Stored at `[0x82971328]` by `sub_825B3548` (`c-runtime.md`; probably `_initp_eh_hooks`); from `__CxxUnhandledExceptionFilter`. Not seen in play. Read in full. |
| `sub_825AD198` | `_CxxThrowException` | | r3 = the thrown object, r4 = its throw info: copies the 32-byte exception template at `0x8207275C` (code `0xE06D7363`, flags 1, 3 parameters, magic `0x19930520`) to the stack, the object and throw info as parameters 1 and 2, the magic number `0x01994000` when the throw info's flags have bit 3 (pure), then `RaiseException` (`sub_825BA420`). From `std::_Xran` (`sub_825A6B18`, a lead). Not seen in play. Read in full. |
| `sub_825BA420` | `RaiseException` | | r3 = code, r4 = flags (bit 0 kept), r5 = parameter count (at most 15), r6 = parameters: an exception record on the stack (the address `0x825BA420` itself), the parameters copied (`sub_825A7830`, `c-runtime.md`), `RtlRaiseException`. From `_CxxThrowException`. Not seen in play. Read in full. |
| `sub_825B4CD0` | `__CxxUnhandledExceptionFilter` | | r3 = the exception pointers: a C++ exception (`0xE06D7363`, 3 parameters, magic `0x19930520` to `0x19930522`) goes to `terminate`; else the previous filter (`[0x82971348]`) when set and `sub_825B8910` says it's valid, else 0. No direct callers: set by `__CxxSetUnhandledExceptionFilter`. Not seen in play. Read in full. |
| `sub_825B4D90` | `__CxxRestoreUnhandledExceptionFilter` | | `SetUnhandledExceptionFilter([0x82971348])`. Registered with `atexit` by `__CxxSetUnhandledExceptionFilter`; runs only on the exit path. Not seen in play. Read in full. |
| `sub_825B8910` | `_ValidateExecute` (probably, a stub) | | r3 = a pointer: 1 when it isn't 0, else 0. Leaf. From `__CxxUnhandledExceptionFilter`. Not seen in play. Read in full. |
| `sub_82297870` | `SetUnhandledExceptionFilter` | | r3 = the new filter: swapped with `[0x829714A0]`; returns the old one. Leaf. From `__CxxSetUnhandledExceptionFilter` (at boot), its restore, `abort` and `_invoke_watson`. Seen in play (once at boot). Read in full. |
| `sub_82297940` | `UnhandledExceptionFilter` | | r3 = the exception pointers: when a debugger is attached (`KeDebugMonitorData` `[0x820007F0]`, 0 here: no call), its routine (+24) with 10; else the filter at `[0x829714A0]` when set, returning -1 when it does, else 0. From `abort` and `_invoke_watson`. Not seen in play. Read in full. |
| `sub_822979D0` | `GetCommandLineA` (probably) | | Returns `[0x820007EC]` (the kernel variable `ExLoadedCommandLine`, xboxkrnl ordinal `0x1AE`). Leaf. From `xstart` when `[0x829537B8]` is set (it splits the line into up to 16 arguments for `main`). Not seen in play. Read in full. |
| `sub_825B9BA8` | a debug monitor notification (probably) | | r3 = a code, f1 = a value: when a debugger is attached (`[0x820007F0]`, 0 here), its routine (+24) called twice with 39: r3 \| `0x02200000`, then the float's bits. From `sub_822B1CB0` (`functions.md`, the graphics library's debugger callback) and `sub_8245F4E0` (`xaudio-engine.md`). Not seen in play. Read in full. |
| `sub_82294E20` | `CloseHandle` | | r3 = handle: `NtClose`; 1, or 0 with the last error. Called for files, events and enumerators (the file class's close, the save slot's close wait, the enumeration's wait, `Archive::Shutdown`). Moved from `functions.md` (2026-11-14); here for the C runtime's `_close_nolock`. 20 callers in the static map (seen in play). |
| `sub_825BA168` | `ResetEvent` | | r3 = an event: `NtClearEvent`; 1, or 0 with `SetLastErrorFromStatus` (`sub_82296E88`). From `sub_825776A8` (under `sub_82575C90`, `system-services.md`). Not seen in play. Read in full. |
| `sub_825AD4D0` | `_initstdio` | | `_nstream` (`[0x82978328]`) = 512 when 0, at least 20; `__piob` (`[0x82978324]`) = `calloc(_nstream, 4)` (20 again when that fails; 26 when both fail); the first 20 slots point at `_iob` (`0x8266B7D0`, 32 bytes a stream); for the first three, `_file` (+16) = -2 when `__pioinfo`'s handle is -1, -2 or 0. A C initializer (`_cinit`'s table at `0x82600288`). Seen in play (once at boot). Read in full. |
| `sub_825AD5F8` | `_endstdio` | | `_flushall`; when `_exitflag` (`[0x82971144]`) is set, `_fcloseall`; frees `__piob`. A pre-terminator (`0x82600298`, run by `doexit` through `_initterm`). Not seen in play. Read in full. |
| `sub_825B4668` | `_ioinit` | | The first handle block: `calloc(32, 44)` into `__pioinfo[0]` (`[0x82978220]`), `_nhandle` (`[0x82978210]`) = 32; each entry: +0 = -1 (the OS handle), +4 = 0 (`osfile`), +5 = 10, +8 = 0, +40 = 0, +41 and +42 = 10; then the first three: +4 = `0xC1`, +0 = -2. Returns 0, or -1 when the allocation fails. In XAPI's initializer table (`0x82600008`, `sub_82297AC0`). Seen in play (once at boot). Read in full. |
| `sub_825AD638` | `_lock_file` | | r3 = a stream: inside `_iob` (`0x8266B7D0` to +608), `_lock(16 + index)` (`sub_825B3430`); else `RtlEnterCriticalSection(stream + 32)`. From `fclose`, `fflush`, `fputs`. Not seen in play. Read in full. |
| `sub_825AD670` | `_lock_file2` | | r3 = a stream index, r4 = the stream: below 20 `_lock(16 + index)`, else `RtlEnterCriticalSection(stream + 32)`. From `flsall`. Not seen in play. Read in full. |
| `sub_825AD688` | `_unlock_file` | | As `_lock_file`, with `_unlock` (`sub_825B32D0`) or `RtlLeaveCriticalSection`. From the `__finally` blocks of `fclose`, `fflush`, `fputs`. Not seen in play. Read in full. |
| `sub_825AD6C0` | `_unlock_file2` | | As `_lock_file2`, leaving. From `flsall`'s `__finally`. Not seen in play. Read in full. |
| `sub_825B7958` | `__lock_fhandle` | | r3 = a file handle: when its entry's critical section isn't made yet (+8), under `_lock(10)` makes it (`__crtInitCritSecAndSpinCount(entry + 12, 4000)`, +8 counted up; the `__finally` `sub_825B7A20`); then enters it; returns 1, or 0 when it couldn't be made. Entries: `__pioinfo[handle >> 5] + 44 × (handle & 31)`. From `_close`, `_write`, `_commit`, `_lseeki64`. Not seen in play. Read in full. |
| `sub_825B7A20` | `__lock_fhandle`'s `__finally` | | `_unlock(10)`; reloads the caller's registers from its frame. Not seen in play. Read in full. |
| `sub_825B7A58` | `_unlock_fhandle` | | r3 = a file handle: `RtlLeaveCriticalSection` on its entry + 12. Leaf but for the import. From the `__finally` blocks of `_close`, `_write`, `_commit`, `_lseeki64`. Not seen in play. Read in full. |
| `sub_825B7880` | `_get_osfhandle` | | r3 = a file handle: its entry's OS handle (+0) when it's below `_nhandle` and open (+4 bit 0); -2 or out of range: `_doserrno` 0, `errno` 9 (EBADF; `_invalid_parameter` for out of range), -1. From `_close_nolock`, `_commit`, `_lseeki64_nolock`. Not seen in play. Read in full. |
| `sub_825B77E8` | `_free_osfhnd` | | r3 = a file handle: when open with an OS handle, +0 = -1 and returns 0; else `errno` 9, `_doserrno` 0, -1. Leaf but for `_errno`. From `_close_nolock`. Not seen in play. Read in full. |
| `sub_825B0E88` | `_fileno` | | r3 = a stream: its handle (+16); a null stream: EINVAL, `_invalid_parameter`, -1. From the stream functions (`_flush`, `fputs`, `_flsbuf`, `_stbuf`...). Not seen in play. Read in full. |
| `sub_825B0C28` | `_close_nolock` | | r3 = a file handle: unless it's handle 1 with 2 open or 2 with 1 open (the other's `osfile` bit 0, block +92 or +48) and both have the same OS handle, `CloseHandle` its OS handle (`sub_82294E20`), the error from `GetLastError`; `_free_osfhnd`; `osfile` (+4) = 0; on an error `_dosmaperr` and -1, else 0. From `_close`. Not seen in play. Read in full. |
| `sub_825B0D10` | `_close` | | r3 = a file handle: -2 or out of range: EBADF (`_invalid_parameter` for out of range), -1; else under `__lock_fhandle`, `_close_nolock` when still open, else EBADF and -1; the `__finally` `sub_825B0E50`. From `_fclose_nolock`. Not seen in play. Read in full. |
| `sub_825B0E50` | `_close`'s `__finally` | | `_unlock_fhandle` on the caller's handle. Not seen in play. Read in full. |
| `sub_825B11A8` | `_write` | | r3 = a file handle, r4 = buffer, r5 = count: the same checks as `_close`, then `_write_nolock` (`sub_825B0F48`, `files-and-saves.md`) under `__lock_fhandle`; the `__finally` `sub_825B1304`. From `_fwrite_nolock`, `_flush`, `_flsbuf`. Not seen in play. Read in part (its checks and calls). |
| `sub_825B1304` | `_write`'s `__finally` | | `_unlock_fhandle` on the caller's handle. Not seen in play. Read in full. |
| `sub_825B4978` | `_commit`'s `__finally` | | `_unlock_fhandle` on the caller's handle (`_commit` `sub_825B4828`, `files-and-saves.md`: `FlushFileBuffers` on the OS handle under `__lock_fhandle`, `_doserrno` from `GetLastError`, else EBADF). Not seen in play. Read in full. |
| `sub_825B65D0` | `_lseeki64` | | r3 = a file handle, r4 = a 64-bit offset, r5 = origin: the same checks as `_close`, then `_lseeki64_nolock` (`sub_825B6500`, `files-and-saves.md`) under `__lock_fhandle`; returns the 64-bit position or -1; the `__finally` `sub_825B6730`. From `_flsbuf` (append mode: to the end). Not seen in play. Read in part (its checks and calls). |
| `sub_825B6730` | `_lseeki64`'s `__finally` | | `_unlock_fhandle` on the caller's handle. Not seen in play. Read in full. |
| `sub_825A9580` | `fclose` | | r3 = a stream: null: EINVAL, `_invalid_parameter`, -1; a string stream (+12 bit `0x40`): +12 = 0; else `_fclose_nolock` under `_lock_file`, the `__finally` `sub_825A9664`. From `_fcloseall`. Not seen in play. Read in full. |
| `sub_825A9664` | `fclose`'s `__finally` | | `_unlock_file` on the caller's stream. Not seen in play. Read in full. |
| `sub_825A94C0` | `_fclose_nolock` | | r3 = a stream: null: EINVAL, -1. When it's in use (+12 & `0x83`): `_flush`, `_freebuf`, `_close(_fileno)` (-1 when that fails), the temporary name (+28) freed; +12 = 0; returns `_flush`'s result or -1. From `fclose`. Not seen in play. Read in full. |
| `sub_825B4E78` | `_fcloseall` | | Under `_lock(1)`: `fclose` on every stream from the fourth on (`__piob[3..]`) that is in use, counting the successes; those past the first 20 freed and their slot cleared; the `__finally` `sub_825B4F38`. Returns the count. From `_endstdio`. Not seen in play. Read in full. |
| `sub_825B4F38` | `_fcloseall`'s `__finally` | | `_unlock(1)`. Not seen in play. Read in full. |
| `sub_825ACF70` | `fflush` | | r3 = a stream: stdout and stderr (`__iob_func` + 32 and + 64) return 0 at once (this build's console streams go to `OutputDebugStringA`); null: `flsall(0)`; else `_fflush_nolock` under `_lock_file`, the `__finally` `sub_825AD02C`. From `_Debug_message` (null). Not seen in play. Read in full. |
| `sub_825AD02C` | `fflush`'s `__finally` | | `_unlock_file` on the caller's stream. Not seen in play. Read in full. |
| `sub_825ACD10` | `_fflush_nolock` | | r3 = a stream: null: `flsall(0)`; `_flush` (-1 when it fails); with `_IOCOMMIT` (+12 bit `0x4000`), `_commit(_fileno)` (-1 when that fails); else 0. From `flsall` and `fflush`. Not seen in play. Read in full. |
| `sub_825ACC78` | `_flush` | | r3 = a stream: when it's writing (+12 & 3 = 2) with a buffer (+12 & `0x108`) holding bytes (+0 - +8 > 0), `_write(_fileno, +8, count)`; all written: `_IORW` streams drop `_IOWRT`; else +12 \|= 32 (`_IOERR`) and -1; +0 = +8, +4 = 0. From `_fclose_nolock`, `_fwrite_nolock`, `_fflush_nolock`, `_ftbuf`. Not seen in play. Read in full. |
| `sub_825ACD90` | `flsall` | | r3 = 1 (`FLUSHALL`) or 0 (`FLUSHNULL`): under `_lock(1)`, for each of the `_nstream` slots of `__piob` in use, `_fflush_nolock` under `_lock_file2` (the `__finally` `sub_825ACF04`): all of them counting the successes, or the writing ones (+12 bit 1) noting a failure; the outer `__finally` `sub_825ACEB4`. Returns the count (1) or 0/-1 (0). From `_flushall`, `fflush`, `_fflush_nolock`. Not seen in play. Read in full. |
| `sub_825ACF04` | `flsall`'s inner `__finally` | | `_unlock_file2` on the slot's stream; reloads the loop's registers. Not seen in play. Read in full. |
| `sub_825ACEB4` | `flsall`'s outer `__finally` | | `_unlock(1)`. Not seen in play. Read in full. |
| `sub_825AD068` | `_flushall` | | `flsall(1)`. From `_endstdio`. Not seen in play. Read in full. |
| `sub_825AD280` | `fputs` | | r3 = a string, r4 = a stream: null either: EINVAL, `_invalid_parameter`, -1; unless a string stream, the handle's entry (`__pioinfo`, or `__badioinfo` `0x8266BFB8` for -1 and -2) must have +40's text-mode bits clear (EINVAL otherwise); `strlen`, then under `_lock_file`: `_stbuf`, `_fwrite_nolock(s, 1, len)`, `_ftbuf`; the `__finally` `sub_825AD488`. Returns 0, or -1 when not all were written. From `_Debug_message`. Not seen in play. Read in full. |
| `sub_825AD488` | `fputs`'s `__finally` | | `_unlock_file` on the caller's stream. Not seen in play. Read in full. |
| `sub_825A96A0` | `_fwrite_nolock` | | r3 = buffer, r4 = size, r5 = count, r6 = stream; returns the items written. stdout and stderr (`__iob_func` + 32, + 64) go to `OutputDebugStringA` 255 bytes at a time (copied to the stack, terminated); else the CRT's loop: into the buffer (`memcpy` `sub_825A7410`), whole buffers straight through `_write` after `_flush`, single bytes through `_flsbuf`; an error sets `_IOERR`. From `fputs` (and `fwrite` `sub_825B4250`, outside the static map's reach). Not seen in play. Read in full. |
| `sub_825AEE28` | `_flsbuf` | | r3 = a character, r4 = a stream: not writable: EBADF and `_IOERR`; a string stream: ERANGE. Sets `_IOWRT`; a stream without a buffer gets one (`_getbuf`), stdout and stderr too unless on a terminal (`sub_825C8388`, standing for `_isatty`: 0); with a buffer: writes what's there (`_write`), or in append mode seeks to the end first (`_lseeki64`), then stores the character; without: writes the one byte. Returns the character, or -1. From `_fwrite_nolock` and `_vsprintf_l` (the string stream's terminator). Not seen in play. Read in full. |
| `sub_825B40B8` | `_stbuf` | | r3 = a stream: for stdout (0) or stderr (1) when `sub_825C8388` (for `_isatty`) isn't 0 — never, so it returns 0 — and no buffer yet: `_cflush` (`[0x82971174]`) counted, the shared buffer `[0x82971330 + 4 × n]` (`malloc(4096)` the first time, else the stream's own 2-byte one at +20), +12 \|= `0x1102`; returns 1. From `fputs` (and `fwrite`). Not seen in play. Read in full. |
| `sub_825B41A0` | `_ftbuf` | | r3 = `_stbuf`'s result, r4 = the stream: when it was 1 and +12 has `_IOFLRTN` (`0x1000`): `_flush`, then the temporary buffer taken back (+12 loses `0x1100`, +24, +0, +8 = 0). From `fputs` (and `fwrite`). Not seen in play. Read in full. |
| `sub_825B4790` | `_getbuf` | | r3 = a stream: `_cflush` counted; `malloc(4096)` into +8 with +24 = 4096 and `_IOMYBUF` (8); else the 2-byte buffer at +20, +24 = 2, `_IONBF` (4); +4 = 0, +0 = +8. From `_flsbuf`. Not seen in play. Read in full. |
| `sub_825B0EE0` | `_freebuf` | | r3 = a stream: when in use with `_IOMYBUF`, frees the buffer (+8) and clears +0, +4, +8 and the `_IOMYBUF` and `_IOSETVBUF` bits. From `_fclose_nolock`. Not seen in play. Read in full. |
| `sub_825A70F0` | `_vsprintf_l` | | r3 = buffer, r4 = format, r5 = locale, r6 = `va_list`: null buffer or format: EINVAL, -1; else a string stream on the stack (+0 and +8 the buffer, +4 `0x7FFFFFFF`, +12 66), `_output_l` (`sub_825AF130`, `c-runtime.md`), then the terminator (stored, or `_flsbuf(0)` when there's no room). Returns `_output_l`'s count. From `vsprintf`. Not seen in play. Read in full. |
| `sub_825A71B0` | `vsprintf` | | r3 = buffer, r4 = format, r5 = `va_list`: `_vsprintf_l` with locale 0. From `DebugText`'s printf (`sub_82090F88`, `framework.md`), `sub_8258C3A8` and `DebugPrintf` (`sub_825A1318`, `device-shaders.md`). Not seen in play. Read in full. |
| `sub_825A6948` | `_Debug_message` (probably, Dinkumware's) | | r3 = a message, r4 = a detail (or `"unknown"` at `0x82000CF4`): `fflush(0)`, then `fputs` of both and `"\n"` (`0x82013A84`) to stderr (`__iob_func` + 64), then `abort`. From `std::_Throw`. Not seen in play. Read in full. |
| `sub_825A69D8` | `std::_Throw` (probably) | | r3 = an exception object: its `what()` (vtable slot 1), then `_Debug_message("exception: ", what)` (`0x820722BC`). From `sub_82091200`, `sub_82091260` (the game's C++ library code, unmapped) and `sub_825944B0` (`device-shaders.md`). Not seen in play. Read in full. |
| `sub_825A85C0` | `strtol` | | r3 = string, r4 = end out, r5 = base: `strtoxl` (`sub_825A82C8`, `c-runtime.md`) with the global locale (`0x8266BC28`) and flags 0 (signed). From `atol`. Not seen in play. Read in full. |
| `sub_825AAF08` | `atol` (probably; `atoi` has the same code) | | `strtol(r3, 0, 10)`. From `sub_822B4330` (unmapped). Not seen in play. Read in full. |
| `sub_825A8630` | `strrchr` | | r3 = string, r4 = character: the last occurrence, or 0; for 0, the terminator. Leaf. From `_cftoa_l`. Not seen in play. Read in full. |
| `sub_825AC970` | `bsearch` | | r3 = key, r4 = base, r5 = count, r6 = width, r7 = compare: halving search calling r7(key, element); a null base with a count, a 0 width or a null compare: EINVAL and 0. From `sub_82575B68` (under `system-services.md`'s `sub_82576BE0`). Not seen in play. Read in full. |
| `sub_825ABE30` | `exp` (probably, without error reporting) | | f1 = x: n = round(x × 1/ln 2), r = x - n × ln 2 (the constants at `0x820726F0`: +24 1/ln 2, +32 and +40 ln 2 in two parts, +48 0.5, the polynomials' coefficients up to +88), (0.5 + p/(q - p)) scaled by 2^(n+1) through the exponent bits; x ≥ 709.78 gives `[0x8266BC30]` (+inf), x ≤ -708.40 gives 0.0. Leaf. From `sub_82582AD8`, `sub_82583C98` (unmapped) and the movie library. Not seen in play. Read in full. |
| `sub_825AEB70` | `_cfltcvt` | | r3 = the double's address, r4 = buffer, r5 = its size, r6 = format, r7 = precision, r8 = caps: as `_cfltcvt_l` (`sub_825AEB20`, `c-runtime.md`) with locale 0: 'e'/'E' `_cftoe_l`, 'f' `_cftof_l`, 'a'/'A' `_cftoa_l`, else `_cftog_l`. No direct callers: entries 0 and 5 of the float conversion table (`sub_825A6DB0`). Not seen in play. Read in full. |
| `sub_825AE1A8` | `_cftoe_l` | | %e: r3 = the double's address, r4 = buffer, r5 = size, r6 = precision, r7 = caps, r8 = locale: `_fltout2`, `_fptostr` (`c-runtime.md`), `_cftoe2_l`; EINVAL for a null buffer or size 0. From `_cfltcvt_l`, `_cfltcvt`, `_cftoa_l`. Not seen in play (only %f is). Not read in detail. |
| `sub_825ADF28` | `_cftoe2_l` | | Lays out %e's text from `_fltout2`'s digits: sign, the digit and decimal point (`memmove` `sub_825AA840`), "e" or "E", the exponent's sign and three digits; `strcpy_s` (`sub_825A9DD8`) for "inf"-like results, `_invoke_watson` when that fails; EINVAL or ERANGE for a small buffer. From `_cftoe_l` and `_cftog_l`. Not seen in play. Not read in detail (199 lines). |
| `sub_825AE2C0` | `_cftoa_l` | | %a (hexadecimal floats): EINVAL/ERANGE checks, infinities and NaNs through `_cftoe_l`, the mantissa's hex digits rounded to the precision, "p" and the binary exponent; `strrchr` for the point. From `_cfltcvt_l` and `_cfltcvt`. Not seen in play. Not read in detail (347 lines). |
| `sub_825AE9D0` | `_cftog_l` | | %g: `_fltout2` and `_fptostr`, then `_cftoe2_l` or `_cftof2_l` (`sub_825AE708`, `c-runtime.md`) by the exponent against the precision. From `_cfltcvt_l` and `_cfltcvt`. Not seen in play. Not read in detail. |
| `sub_825ADD38` | `_forcdecpt_l` | | r3 = a number's text: after the leading digits (or at an 'e'/'x' after the first character's `tolower`), inserts the locale's decimal point (`[[[0x8266C024]]]`), moving the rest up a byte. From `_forcdecpt` and the float conversion table (entry 9). Not seen in play. Read in full. |
| `sub_825ADDD8` | `_cropzeros_l` | | r3 = a number's text: from the decimal point, drops the trailing zeros before the exponent (and the point when nothing follows it), moving the exponent down. Leaf. From `_cropzeros` and the table (entry 8). Not seen in play. Read in full. |
| `sub_825ADE90` | `_positive` | | r3 = a double's address: 1 when it is ≥ 0.0, else 0. Leaf. The table's entry 4. Not seen in play. Read in full. |
| `sub_825ADEB0` | `_fassign_l` | | r3 = 1 for a double, else a float; r4 = out, r5 = string, r6 = locale: `_atodbl_l` into a double, or `_atoflt_l` into a float, stored at r4. From `_fassign` and the table (entry 7). Not seen in play. Read in full. |
| `sub_825ADF10` | `_fassign` | | `_fassign_l` with locale 0. The table's entry 2. Not seen in play. Read in full. |
| `sub_825ADF18` | `_forcdecpt` | | `_forcdecpt_l` with locale 0. The table's entry 3. Not seen in play. Read in full. |
| `sub_825ADF20` | `_cropzeros` | | `_cropzeros_l` with locale 0. The table's entry 1. Not seen in play. Read in full. |
| `sub_825B6098` | `_atodbl_l` | | r3 = the double out, r4 = string: `__strgtold12_l` (`sub_825B36B8`, `c-runtime.md`; global locale `0x8266BC28`) into a 12-byte long double on the stack, `_ld12tod` (`sub_825B69B8`); returns 0, 3 (underflow) or 4 (overflow) from both results. From `_fassign_l`. Not seen in play. Read in full. |
| `sub_825B6140` | `_atoflt_l` | | As `_atodbl_l` into a float with `_ld12tof`. From `_fassign_l`. Not seen in play. Read in full. |
| `sub_825B70D0` | `_ld12tof` | | r3 = a 12-byte long double, r4 = the float out: `_ld12tod`'s rounding with the float's format descriptor (`0x8266C52C`, `../known-data.md`'s `0x8266C514` + 24). Leaf. From `_atoflt_l`. Not seen in play. Not read in detail (597 lines, `_ld12cvt` inlined). |
