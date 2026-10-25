# Known functions: the heap and memory allocation

How the game gets memory: the C runtime's allocators (operator new and
delete, malloc, free, calloc, realloc, `_msize`), LocalAlloc and XMemAlloc,
over the process heap (XAPI's Win32-style heap, statically linked:
HeapAlloc, HeapFree, HeapReAlloc, HeapSize, its handle at `0x8297149C`),
VirtualAlloc and VirtualFree, and physical memory (XPhysicalAlloc,
XPhysicalAllocEx, XPhysicalFree). Since 2026-10-22 the nine functions
under them are routed to the native allocation layer
(`itc_recomp/src/memory/heap.h`, adapters in `src/hooks/heap.cpp`
behind `--native_heap`, harness `heap`), which every allocation,
recompiled and native, goes through; native code calls the layer
directly. How it's built, what the game was seen to allocate, and why:
`../native-memory.md`. The heap's creation at boot (`sub_82297C08`,
`sub_822997B0`) is in `system-services.md` (B5's start-up).

Above the layer (read in full by the B7 loop, 2026-10-22, from the C
runtime heap's lead): operator new and delete, malloc, free, calloc,
realloc, `_msize`, XMemAlloc, XMemFree, LocalAlloc, LocalFree and
GetProcessHeap are still recompiled. In play each comes down to one
layer call (`memory::Alloc` with flags 0 or `kHeapZeroMemory`, `Free`,
`Realloc`, `Size`, or the physical calls): sizes over `0xFFFFF000` fail,
size 0 becomes 1 (except HeapReAlloc's own size 0, which realloc never
passes), and the error paths (errno through `_errno` `sub_825A81E0`,
`_callnewh` `sub_825AD150` and the new handler at `[0x82971170]`,
`std::bad_alloc` thrown by operator new, the fatal error with no process
heap) never ran in play and can't with `--native_heap`, as the layer
aborts when out of memory: native versions keep them on the recompiled
path (their adapters calling `__imp__` when the layer gives 0, say). Two
callees outside this file run in play from other callers and so count as
blockers to `b7_groups.py`: GetLastError (`sub_82295F18` jumping to
`sub_82296EC0`, in `functions.md` beside SetLastError, a ready group; only
on free's and realloc's failure paths here) and `atexit` (`sub_825A7068`,
native since 2026-10-25, in `c-runtime.md`; only on operator new's
`bad_alloc` path). With those paths left recompiled, GetProcessHeap
(a leaf), malloc, free, `_calloc_impl`, calloc, `_msize`, realloc,
LocalAlloc, LocalFree, XMemAlloc, XMemFree and operator new and delete
can go native from the bottom up. The process heap's creation stays (the
adapters compare its handle), until the heap's adapters don't need it.

Made native 2026-10-22 (the B7 loop, migration 1): all fifteen, as plain
C++ in `itc_recomp/src/memory/c_runtime_heap.cpp` over the layer, with
adapters in `src/hooks/c_runtime_heap.cpp` behind
`--native_c_runtime_heap` (on by default; harness `c_runtime_heap`);
`_calloc_impl`, LocalAlloc and LocalFree are Native, their adapters gone
(only native callers reach them). The
adapters run the plain C++ only while `--native_heap` routes the heap to
the layer (`hooks::NativeHeapActive`), as it calls the layer directly. A
function that would take an error path returns nothing with nothing
changed, and its adapter runs the original from the start, which fails
the same way and takes the path (the layer's failures have no side
effects but a warning logged once). The harness found the map's reading
of XMemAlloc's physical attributes off: the protection's index is bits
28-29 and the default alignment field is 12 (4 KiB).

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
| `sub_82295330` | `VirtualFree` | Overridden | r3 = address, r4 = size, r5 = type; returns BOOL. The game only calls it as `(address, 0, MEM_RELEASE)` (seen by the counter, 2026-10-22). Ours (`src/hooks/heap.cpp`): `memory::FreePages` for the layer's range; other addresses go to the original, and other types are logged once and do nothing. Harness `heap`. |
| `sub_82295398` | `VirtualAlloc` | Overridden | r3 = address, r4 = size, r5 = type, r6 = protection; calls `NtAllocateVirtualMemory` (zero bits 0) and returns the address, or 0 after `sub_82296E88`. The game uses address 0, protection 4 and the types `0x1000`, `0x801000` (with MEM_NOZERO, from `sub_8258DFF0`) and `0x20801000` (with MEM_LARGE_PAGES, from `sub_824EFE20`). About 150 callers. Ours: `memory::AllocPages`, 4 KiB pages (64 KiB-aligned with large pages), always cleared; other forms go to the original, logged once. Harness `heap`. |
| `sub_822964C8` | `XPhysicalAlloc` | Overridden | r3 = size, r4 = physical address (-1 for any), r5 = alignment, r6 = protection. When `[0x829537B0]` isn't 0, clears X_MEM_LARGE_PAGES from the protection. -1 asks for the range 0 to -1 at the alignment; otherwise exactly `[r4, r4 + size)` with no alignment. Calls `MmAllocatePhysicalMemoryEx`, and `sub_82296FD0(8)` on failure. Ours: `memory::AllocPhysical` (the SDK's physical heap, as its import does). Harness `heap`. |
| `sub_82296550` | `XPhysicalAllocEx` | Overridden | r3 = size, r4 = lowest and r5 = highest physical address, r6 = alignment, r7 = protection (the same mask as `sub_822964C8`). Calls `MmAllocatePhysicalMemoryEx`, and `sub_82296FD0(8)` on failure. From the display set-up `sub_8258D568` (64 KB and 1 MB) and others. Ours: `memory::AllocPhysical`. Harness `heap`. |
| `sub_822965C8` | `XPhysicalFree` | Overridden | r3 = address: `MmFreePhysicalMemory(0, r3)`. Ours: `memory::FreePhysical` (the SDK heap's `Release`). Harness `heap`. |
| `sub_822965C0` | `XPhysicalSize` (probably: XAPI's wrapper of MmQueryAllocationSize) | | r3 = a block of physical memory: a tail call to the kernel import `MmQueryAllocationSize` (6 lines), so r3 comes back the import's answer (the block's allocated size). Two callers, both seen in play: the course scenery's `Scenery::AllocPhysical` and `Scenery::FreePhysical` (`course-scenery.md`), which add and take off the size at `[0x826C5518]`. A kernel import: left as it is by the B7 loop (2026-10-24, the memset lead), with the core; replacing it is a native-memory question (the layer's own size of a physical block, `memory::` in `../native-memory.md`). Read in full 2026-10-24 (the B7 loop). |
| `sub_822965D8` | `XQueryMemoryProtect` (probably: XAPI's wrapper of MmQueryAddressProtect) | | r3 = an address: a tail call to the kernel import `MmQueryAddressProtect` (its protection comes back in r3). Three callers (six call sites), one seen in play: `sub_82562FF0` (387 lines, reached only through a pointer; three calls, each followed by `sub_8254DCD8` or `sub_8254DFF0`, which copy, probably picking a path by the memory's protection; a lead). A kernel import: left as it is (2026-10-24, the B7 loop), with the core. Read in full 2026-10-24 (the B7 loop). |
| `sub_822976F8` | `XMemAlloc` | Overridden | r3 = size, r4 = attributes; returns the block. With bit `0x80000000` (physical): alignment `1 << ((r4 >> 24) & 0xF)` (the field set to 12, 4 KiB, when 0; the map said 3, corrected by the harness 2026-10-22), protection from the table at `0x82638770` by `(r4 >> 28) & 3` (bits 28-29; the map said `>> 26`), `sub_822964C8(size, -1, ...)`, and the block cleared (memset `sub_825A71D0`) with bit `0x40000000`. Otherwise `LocalAlloc((r4 >> 24) & 0x40, size)` (`sub_82297F48`): bit `0x40000000` becomes LMEM_ZEROINIT, so HEAP_ZERO_MEMORY. Its callees are all overridden or the leaf GetProcessHeap. 223 direct callers, 104 seen in play (texture creation `sub_822A5D08`, the code around `82483460`-`824AFA70` and `825C0B60`-`825D5A48`, not read, `Voice::Init` `sub_8257B7A8`, the shader compiler's state `sub_823653B0`, ...). Read in full 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): the physical path's alignment and protection, then `memory::AllocPhysical` as XPhysicalAlloc's adapter calls it (the large-page check at `0x829537B0` included), and the block cleared on the host; otherwise LocalAlloc's plain C++. Harness `c_runtime_heap`. |
| `sub_82297790` | `XMemFree` | Overridden | r3 = block, r4 = attributes. With bit `0x80000000`: nothing for a null block, else jumps to XPhysicalFree (`sub_822965C8`). Otherwise jumps to LocalFree (`sub_82297F90`), so r3 comes back 0 or the block (callers treat it as void, probably). 282 direct callers, 98 seen in play. Read in full 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): `memory::FreePhysical` (r3 left as the block, as XPhysicalFree's adapter leaves it) or LocalFree's plain C++. Harness `c_runtime_heap`. |
| `sub_82297F48` | `LocalAlloc` | Native | r3 = flags, r4 = size: `HeapAlloc(GetProcessHeap(), (r3 >> 3) & 8, r4)`, so LMEM_ZEROINIT (`0x40`) becomes HEAP_ZERO_MEMORY. Only from XMemAlloc. Ours (`src/memory/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): `memory::Alloc(size, (flags >> 3) & 8)`. Harness `c_runtime_heap`. Native since its adapter went (2026-10-22): its only caller, XMemAlloc, calls the plain C++, and `--native_c_runtime_heap` now switches only XMemAlloc. |
| `sub_82297F90` | `LocalFree` | Native | r3 = block: `HeapFree(GetProcessHeap(), 0, r3)`; returns 0 when freed, else the block. Only from XMemFree. Ours (`src/memory/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): `memory::Free`: 0 when freed, else the block. Harness `c_runtime_heap`. Native since its adapter went (2026-10-22): its only caller, XMemFree, calls the plain C++, and `--native_c_runtime_heap` now switches only XMemFree. |
| `sub_82297FD8` | | | The heap's own code: calls `NtAllocateVirtualMemory` (probably committing or growing a segment). No longer runs with `--native_heap` (the heap's four functions are routed). Not read. |
| `sub_822982B8` | | | The heap's own code, calling `NtAllocateVirtualMemory`, as `sub_82297FD8`. Not read. |
| `sub_82298A88` | | | HeapReAlloc's in-place resize: r3 = heap, r4 = flags, r5 = header, r6 = size, r7 = new size in 16-byte units; returns non-zero when the block grew into its free neighbor. Not read in detail. |
| `sub_82298FF8` | `HeapSize` | Overridden | r3 = heap, r4 = flags (unread), r5 = block; returns its usable size, or -1 when the header's busy bit (byte at -11, `0x1`) is clear. For segment blocks, half-word -16 (16-byte units) × 16 less byte -10; for large blocks (header bit `0x8`), word -24 less half-word -16 less 48. Only from `_msize`. Ours: `memory::Size`, the size asked for, kept on the host. Harness `heap`. |
| `sub_822990A0` | | | The heap's own code, calling `NtAllocateVirtualMemory` or `NtFreeVirtualMemory`, as `sub_82297FD8`. Not read. |
| `sub_82299208` | | | The same as `sub_822990A0`. Not read. |
| `sub_82299448` | | | The same as `sub_822990A0`. Not read. |
| `sub_82299D60` | `HeapAlloc` | Overridden | r3 = heap, r4 = flags, r5 = size; returns the block. Blocks are 16-aligned (seen by the harness). The process heap's own flags are 0; callers pass 0 (malloc) or `0x8` (calloc, HEAP_ZERO_MEMORY). Ours: `memory::Alloc` for the process heap's handle (`0x8297149C`); other handles go to the original, logged once (none seen). Harness `heap`. Not read in detail. |
| `sub_8229A648` | `HeapFree` | Overridden | r3 = heap, r4 = flags, r5 = block; returns BOOL. Flags 0 (free, LocalFree). Ours: `memory::Free`. Harness `heap`. Not read in detail. |
| `sub_8229A930` | `HeapReAlloc` | Overridden | r3 = heap, r4 = flags, r5 = block, r6 = size; returns the block. ORs in the heap's flags (heap +24). A null block or a size over `0x7FFFFFFF` returns 0; size 0 keeps a size of 0. Takes the heap's lock (heap +1408) without `0x1`; returns a non-busy block unchanged. Resizes in place (`sub_82298A88`), else HeapAlloc, memcpy, HeapFree; `0x10` fails instead of moving, `0x8` clears the growth, `0x4` raises STATUS_NO_MEMORY. Only from realloc, with flags 0. Ours: `memory::Realloc`. Harness `heap`. |
| `sub_8229B670` | `GetProcessHeap` | Overridden | Returns `[0x8297149C]`, the process heap made by `sub_82297C08`. A leaf. Called by malloc (twice), free, `_calloc_impl`, realloc, `_msize`, LocalAlloc, LocalFree and the shader code's `sub_8235AAC0` (twice; a lead in `../loops/leads.md`); also by `sub_825B32F0`, never seen in play. The heap's adapters compare r3 with this word (`src/hooks/heap.cpp`), so a native version returns the same word (read 2026-10-22, the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): the word read. Harness `c_runtime_heap`. |
| `sub_825A6880` | `operator new` | Overridden | r3 = size; returns the block. malloc (`sub_825A9A90`) until it returns non-zero; after each failure `_callnewh(size)` (`sub_825AD150`: calls the new handler at `[0x82971170]` and gives 1 when it returns non-zero, else 0), and when that gives 0, throws `std::bad_alloc`: a static one at `0x82970FF8` made the first time (guard bit 0 of `0x82971004`; +0 the vtable `0x82000CCC`, +4 `[0x8266B33C]`, probably its message, +8 0; its clean-up `0x825DB3F8` registered with `atexit` `sub_825A7068`), copied to the stack by `sub_825A67A8` (probably the exception's copy constructor) and thrown by `_CxxThrowException` (`sub_825AD198`, throw info `0x820772DC`). None of the failure path ran in play (`sub_825AD150`, `sub_825A67A8` and `sub_825AD198` never ran), and with `--native_heap` malloc doesn't fail (the layer aborts when out of memory), so in play this is malloc. 121 direct callers, 93 seen in play (`CommandList::Init` `sub_8258CD28`, the UI movies' texts, `sub_82119B38`, ...); calls a frame not counted. Read 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): malloc's plain C++; when it fails, the original from the start (the new handler and `std::bad_alloc`, not run by the harness). Harness `c_runtime_heap`. |
| `sub_825A6940` | `operator new` | Overridden | Jumps to `sub_825A6880` (probably the array form, `operator new[]`). 37 direct callers, 26 seen in play (the debug font's load `sub_82090CC0`, the model animation, ...). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): operator new's plain C++, the original when it fails. Harness `c_runtime_heap`. |
| `sub_825A70E8` | `operator delete` | Overridden | r3 = block: jumps to free (`sub_825A9B58`), so a null block does nothing. 284 direct callers, 93 seen in play. Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): free's plain C++, the original when the layer refuses the block. Harness `c_runtime_heap`. |
| `sub_825A71C0` | `operator delete` | Overridden | Jumps to `sub_825A70E8` (probably the array form, `operator delete[]`). 45 direct callers, 22 seen in play. Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): operator delete's plain C++, the original when the layer refuses the block. Harness `c_runtime_heap`. |
| `sub_825A9A90` | `malloc` | Overridden | r3 = size; returns the block, or 0. Above -4096 (`0xFFFFF000`): `_callnewh(size)` (its result unused), errno 12 (ENOMEM, through `_errno` `sub_825A81E0`) and 0. With no process heap (GetProcessHeap gives 0): a fatal error (`sub_825AEDE8`, `sub_825AEDA0(30)`, `sub_825AA1A0(255)`: probably `_FF_MSGBANNER`, `_NMSG_WRITE` and the exit; never ran). Size 0 becomes 1, then `HeapAlloc(GetProcessHeap(), 0, size)` (from `0x825A9AF0`). On failure, when `[0x8297119C]` (probably `_newmode`; nothing in `generated/` stores it, so probably 0) isn't 0, `_callnewh` (`sub_825AD150`) and a retry while it gives non-zero; otherwise errno 12 (stored twice). None of the failure paths ran in play, and with `--native_heap` they can't (the layer aborts when out of memory). 10 direct callers, 4 seen in play: operator new, realloc, and `sub_825AF130` and `sub_825B13D0` (`c-runtime.md`). Read 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): `memory::Alloc(size or 1, 0)`; a size over `0xFFFFF000`, no process heap or the layer giving 0 go to the original from the start (its errno, `_callnewh` and fatal paths). Harness `c_runtime_heap` (with sizes the layer refuses, `0x80000000` and `0xFFFFF000`, through the original's errno path). |
| `sub_825A9B58` | `free` | Overridden | r3 = block: nothing for null, else `HeapFree(GetProcessHeap(), 0, r3)`; when that fails, errno from GetLastError (`sub_82295F18`, in `functions.md`) mapped by `_get_errno_from_oserr` (probably: `sub_825A8178`) into `_errno` (`sub_825A81E0`). The failure path never ran in play (`sub_825A8178` and `sub_825A81E0` never ran; with `--native_heap`, `memory::Free` fails only for an address the layer didn't give, logged once). No return value. 19 direct callers, 6 seen in play: operator delete, realloc, `sub_825AA4C0`, `sub_825AF130` and `sub_825B13D0` (`c-runtime.md`), and `sub_825AA5C0` (8 calls; probably the per-thread data's clean-up, with the core's per-thread data in `c-runtime.md`'s "Not here", no row). Read 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): `memory::Free`; when the layer refuses the block (logged once), the original runs from the start (HeapFree fails again, then its errno path). Harness `c_runtime_heap`. |
| `sub_825AC248` | `_calloc_impl` | Native | r3 = count, r4 = size, r5 = where to put errno (may be null). When count isn't 0 and `0xFFFFF000 / count` (unsigned) is below size: errno 12 through `_errno` (`sub_825A81E0`), `_invalid_parameter(0, 0, 0, 0, 0)` (`sub_825AFCC8`) and 0. Otherwise count × size (the low 32 bits), 0 becoming 1, and while that's at most `0xFFFFF000`, `HeapAlloc(GetProcessHeap(), 8, bytes)` (HEAP_ZERO_MEMORY, from `0x825AC2D0`); on failure, with `_newmode` (`[0x8297119C]`) set, `_callnewh` and a retry while it gives non-zero, then 12 into `*r5` when r5 isn't null, and 0. None of the failure paths ran in play. Only from calloc. Read 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): the overflow check, then `memory::Alloc(bytes or 1, kHeapZeroMemory)`; the overflow (with `_invalid_parameter`) and the layer giving 0 go to the original from the start. Harness `c_runtime_heap` (the overflow not run, as `_invalid_parameter` may end the process). Native since its adapter went (2026-10-22): its only caller, calloc, calls the plain C++, and `--native_c_runtime_heap` now switches only calloc (whose fallback runs the original calloc, and through it this original). |
| `sub_825AC320` | `calloc` | Overridden | r3 = count, r4 = size: `sub_825AC248(r3, r4, &error)` with `error` 0 on the stack; when the result is 0 and `error` isn't, errno = `error` (when `_errno` gives non-null). 7 direct callers, 5 seen in play, all the C runtime's own set-up (`sub_825A6F00`, `sub_825AA4C0`, `sub_825AA708`, `sub_825AD4D0`, `sub_825B4668`: `atexit`'s table, the per-thread data and the stream and lock tables, `c-runtime.md`'s "Not here"; `atexit`'s table's set-up, `sub_825A6F00`, native since 2026-10-25, calls the plain C++ directly). Read 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): `_calloc_impl`'s plain C++; a failure goes to the original (its errno). Harness `c_runtime_heap`. |
| `sub_825AEBC8` | `realloc` | Overridden | r3 = block, r4 = size; returns the block or 0. A null block is malloc (`sub_825A9A90`); size 0 is free (`sub_825A9B58`) and returns 0; above -4096 `_callnewh(size)`, errno 12 and 0. Otherwise `HeapReAlloc(GetProcessHeap(), 0, block, size)` (0 taken as 1); on failure, with `_newmode` (`[0x8297119C]`) set, `_callnewh` and a retry while it gives non-zero (errno from GetLastError when it gives 0), else errno from GetLastError (`sub_82295F18`, `sub_825A8178`, `sub_825A81E0`). None of the failure paths ran in play. 2 direct callers, 1 seen in play: `atexit`'s table growth (`sub_825A6F60`, `c-runtime.md`'s "Not here"); `sub_825AECC0` (probably `_recalloc`) never ran. Read 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): malloc's or free's plain C++ for a null block or size 0, else `memory::Realloc(block, size, 0)`; a size over `0xFFFFF000` or any failure goes to the original from the start. Harness `c_runtime_heap` (a size the layer refuses, `0x80000000`, through the GetLastError path; over `0xFFFFF000` not run, as it calls `_callnewh` whatever `_newmode`). |
| `sub_825AED30` | `_msize` | Overridden | r3 = block: null sets errno 22 (`_errno` `sub_825A81E0`), calls `_invalid_parameter(0, 0, 0, 0, 0)` (`sub_825AFCC8`) and returns -1 (never ran); otherwise `HeapSize(GetProcessHeap(), 0, r3)`. Only caller: `atexit`'s table growth (`sub_825A6F60`, `c-runtime.md`'s "Not here"). Read 2026-10-22 (the B7 loop). Ours (`src/memory/c_runtime_heap.cpp`, its adapter in `src/hooks/c_runtime_heap.cpp`, `--native_c_runtime_heap`, only while `--native_heap` routes the heap to the layer; 2026-10-22, the B7 loop): `memory::Size`; a null block goes to the original (errno 22 and `_invalid_parameter`, not run by the harness). Harness `c_runtime_heap`. |
