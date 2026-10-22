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
| `sub_822976F8` | `XMemAlloc` | | r3 = size, r4 = attributes. With bit `0x80000000` (physical): alignment `1 << ((r4 >> 24) & 0xF)` (3 when 0), protection from the table at `0x82638770` by `(r4 >> 26) & 3`, `sub_822964C8(size, -1, ...)`, and the block cleared (memset) with bit `0x40000000`. Otherwise the process heap (not read in detail). The shader compiler's state comes from here (`sub_823653B0`). |
| `sub_82297790` | `XMemFree` | | r3 = block, r4 = attributes: with bit `0x80000000`, `sub_822965C8` (nothing for a null block); otherwise the process heap (not read in detail). |
| `sub_82297F48` | `LocalAlloc` | | r3 = flags, r4 = size: `HeapAlloc(GetProcessHeap(), (r3 >> 3) & 8, r4)`, so LMEM_ZEROINIT (`0x40`) becomes HEAP_ZERO_MEMORY. |
| `sub_82297F90` | `LocalFree` | | r3 = block: `HeapFree(GetProcessHeap(), 0, r3)`; returns 0 when freed, else the block. |
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
| `sub_8229B670` | `GetProcessHeap` | | Returns `[0x8297149C]`, the process heap made by `sub_82297C08`. |
| `sub_825A6880` | `operator new` | | r3 = size: malloc (`sub_825A9A90`), and while it fails, the new handler, then bad_alloc. Not read in detail. |
| `sub_825A6940` | `operator new` | | Jumps to `sub_825A6880` (probably the array form). |
| `sub_825A70E8` | `operator delete` | | Jumps to free (`sub_825A9B58`). |
| `sub_825A71C0` | `operator delete` | | Jumps to `sub_825A70E8` (probably the array form). |
| `sub_825A9A90` | `malloc` | | r3 = size: fails above -4096 (errno 12); with no process heap, a fatal error (`sub_825AEDE8`, message 30, exit 255). Size 0 becomes 1, then `HeapAlloc(GetProcessHeap(), 0, size)`. On failure, when `[0x8297119C]` (probably `_newmode`) isn't 0, calls `_callnewh` (`sub_825AD150`) and retries. |
| `sub_825A9B58` | `free` | | r3 = block: nothing for null, else `HeapFree(GetProcessHeap(), 0, r3)`; on failure, errno from GetLastError (`sub_82295F18`, `sub_825A8178`, `sub_825A81E0`). |
| `sub_825AC248` | `_calloc_impl` | | r3 = count, r4 = size, r5 = where to put errno: fails (errno 12, via `sub_825AFCC8`) when count × size overflows past -4096; otherwise allocates cleared on the process heap (HeapAlloc flags `0x8`, from `0x825AC2D0`). Not read in detail. |
| `sub_825AC320` | `calloc` | | `sub_825AC248(r3, r4, &error)`, then sets errno from `error` when the result is 0. |
| `sub_825AEBC8` | `realloc` | | r3 = block, r4 = size: a null block is malloc, size 0 is free, over -4096 fails. Otherwise `HeapReAlloc(GetProcessHeap(), 0, block, size)` (0 taken as 1), with the new handler on failure as malloc. |
| `sub_825AED30` | `_msize` | | r3 = block: null sets errno 22 (`sub_825AFCC8`) and returns -1; otherwise `HeapSize(GetProcessHeap(), 0, r3)`. |
