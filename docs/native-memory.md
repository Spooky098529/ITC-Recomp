# Native memory: one allocation layer

B7's first step (`rexglue-removal.md`, under B7). Every allocation the
game makes, from recompiled code in `generated/` and from our native code
in `src/`, goes through one layer of our own. The layer's interface is
what the rest of the code sees; its backing, the part that finds the
bytes, is what gets swapped for host memory allocation later. Designed
with the user, 2026-10-22; not built yet.

## Why

Native code can't allocate today: the only allocators are the game's
own (recompiled, needing `ctx`) and the SDK's kernel imports under them.
The B7 loop stopped at every function that allocates (the C runtime's
heap, the VirtualAlloc wrappers, std::string, the set-up functions in
`car-physics.md`, the UI movies' creation side; leads in
`loops/leads.md`). Calling the game's heap from native code would keep
the dependency, and a native heap beside the game's would corrupt
memory as soon as a block made on one side is freed on the other. One
layer under both removes both problems, and gives the end of the port a
single place to change.

## Where the game gets memory

Read in `generated/` (2026-10-22). Below the C runtime, everything the
game allocates comes from three entry points, all seen in play
(`itc_recomp/coverage/seen_in_play.txt`):

| Entry point | Functions | Reached from |
|---|---|---|
| The process heap | HeapAlloc `sub_82299D60`, HeapFree `sub_8229A648`, HeapReAlloc `sub_8229A930`, HeapSize `sub_82298FF8` | operator new (`sub_825A6880`, and `sub_825A6940` jumping to it), operator delete (`sub_825A70E8`, and `sub_825A71C0` jumping to it), malloc (`sub_825A9A90`), free (`sub_825A9B58`), calloc (`sub_825AC320` over `sub_825AC248`), realloc (`sub_825AEBC8`), `_msize` (`sub_825AED30`), LocalAlloc and LocalFree (probably: `sub_82297F48`, `sub_82297F90`), XMemAlloc and XMemFree without the physical flag (`sub_822976F8`, `sub_82297790`), and the shader compiler (`sub_8235AAC0`) |
| VirtualAlloc and VirtualFree | `sub_82295398` (`(0, size, 4096, 4)`: MEM_COMMIT, PAGE_READWRITE), `sub_82295330` (`(address, 0, 0x8000)`: MEM_RELEASE) | About 150 of the game's functions: the loads, the UI movies' files and texts, the archive streaming |
| Physical memory | XPhysicalAlloc `sub_822964C8` (size, highest address -1, alignment, protection) and XPhysicalAllocEx `sub_82296550` (size, lowest and highest address, alignment, protection), XPhysicalFree `sub_822965C8` | XMemAlloc with the physical flag, and the game directly (textures, vertex and command buffers: `sub_820ECF08`, `sub_82177378`, the graphics library's `sub_8258D568`, ...) |

- The process heap is the game's own Win32-style heap, statically linked
  (XAPI's RtlAllocateHeap and the like). Its handle is kept at
  `0x8297149C`, read by `sub_8229B670` (GetProcessHeap). It's made once at
  boot by `sub_82297C08`, through `sub_822997B0` (probably RtlCreateHeap,
  its only caller). No other heap is made.
- The other callers of `NtAllocateVirtualMemory` and
  `NtFreeVirtualMemory` (`sub_82297FD8`, `sub_822982B8`, `sub_822990A0`,
  `sub_82299208`, `sub_82299448`, `sub_822997B0`) are the heap's own
  code, creating and growing its segments. Once the heap's four functions
  are routed, they no longer run, apart from the heap's creation at boot
  (one small reservation, never used).
- `XamAlloc` and `XamFree` have callers (`sub_82575D78` and others), but
  none runs in play. `MmAllocatePhysicalMemoryEx` has a third caller,
  `sub_822B4600`, which doesn't run either.
- Routing the nine functions in the table sends every allocation that
  runs in play through the layer, so a block made and freed through any
  of the paths above is fine.

### HeapReAlloc's and HeapSize's arguments

Read in `generated/` (2026-10-22).

- HeapReAlloc `sub_8229A930` is `(heap r3, flags r4, block r5, size r6)`,
  returning the block's address in r3. Its only caller is realloc
  (`sub_825AEBC8`), with the process heap and flags 0 (realloc handles a
  null block and size 0 itself, calling malloc and free, and fails sizes
  over -4096). What it does:
  - The heap's own flags (the word at heap +24) are ORed into `flags`.
  - A null block returns 0; a size over `0x7FFFFFFF` returns 0; size 0 is
    taken as 1.
  - Without HEAP_NO_SERIALIZE (`0x1`) it takes the heap's lock (heap
    +1408).
  - A block whose header doesn't have its busy bit (header byte at block
    -11, bit `0x1`) is returned unchanged.
  - It shrinks or grows in place when it can (the neighbor's free space,
    `sub_82298A88`); otherwise, unless HEAP_REALLOC_IN_PLACE_ONLY
    (`0x10`, which returns 0), it allocates a new block with HeapAlloc
    (`sub_82299D60`, the flags without `0x8`), copies the smaller of the
    old and new sizes (memcpy `sub_825AA840`), and frees the old block
    with HeapFree (`sub_8229A648`).
  - HEAP_ZERO_MEMORY (`0x8`) clears the part past the old size, in place
    or moved (memset `sub_825A71D0`).
  - On failure with HEAP_GENERATE_EXCEPTIONS (`0x4`) it raises
    `0xC0000017` (STATUS_NO_MEMORY, through RtlRaiseException's import).
- HeapSize `sub_82298FF8` is `(heap r3, flags r4, block r5)`, returning
  the block's usable size in r3. `flags` isn't read. Its only caller is
  `_msize` (`sub_825AED30`), with the process heap and flags 0. It returns
  -1 when the busy bit is clear; otherwise, for a block from the heap's
  segments, its size in 16-byte units (the half-word at block -16) times
  16 less its unused bytes (the byte at block -10); for a large block of
  its own pages (header bit `0x8`), the committed size (the word at block
  -24) less the unused bytes (the half-word at block -16) less 48. That
  is, the size the caller asked for, which is what the layer's `Size`
  returns from its host-side table.
- VirtualAlloc `sub_82295398` is `(address r3, size r4, type r5,
  protection r6)`, calling `NtAllocateVirtualMemory` with zero bits 0 and
  returning the address, or 0 on failure (after `sub_82296E88`, probably
  SetLastError from the status). The size rounded by the kernel isn't
  returned.

## The layer

`itc_recomp/src/memory/`, plain C++ (no `ctx`):

- The heap: `Alloc(size, flags)`, `Free(address)`, `Realloc(address,
  size, flags)`, `Size(address)`.
- Pages: `AllocPages(size)` and `FreePages(address)`. VirtualAlloc's
  memory is page-aligned and zeroed.
- Physical memory: `AllocPhysical(size, low, high, alignment,
  protection)` and `FreePhysical(address)`.

How it's reached:

- Recompiled code: thin `REX_FUNC` adapters on the nine functions read
  the registers, call the layer, and set r3. They're behind
  `--native_heap` (on by default once checked); with the flag off, they
  call `__imp__` and the game's own heap runs as before. HeapAlloc,
  HeapFree, HeapReAlloc and HeapSize check the handle in r3 against the
  process heap's and fall back to `__imp__` for any other (none is
  expected; logged once if seen).
- Native code calls the layer directly. Nothing in `src/` allocates
  guest memory any other way.

Not routed through it:

- Reads and writes. Native code still reads and writes guest memory
  through `src/guest.h`, as now. The layer hands out addresses; it
  doesn't stand between code and the bytes.
- What the SDK allocates for itself: thread stacks, kernel objects, the
  `default.xex` image and its data. Those go with the core (E1, E2).

## The backing, for now

While any recompiled code remains, it can only use 32-bit guest
addresses into memory laid out as on the Xbox, so the backing has to
hand out guest memory until the port's end.

- The heap and pages: one range reserved once, on the first allocation,
  from the SDK's virtual heap (`rex::memory::Memory::LookupHeapByType(false,
  4096)` and `BaseHeap::Alloc` in `rex/system/xmemory.h`, the SDK's
  header included rather than reimplemented), the same heap
  `NtAllocateVirtualMemory` gives from, so nothing else is handed the
  same addresses. Its size comes from a log (below).
- Inside the range, the simplest allocator that works: first fit over a
  sorted list of free ranges, joined with their neighbors when blocks
  are freed. Each block's size is kept on the host side, so no headers
  are written into guest memory and recompiled code sees only the bytes
  it asked for. Heap blocks are 8-aligned; pages are 4,096-aligned and
  cleared on allocation. HEAP_ZERO_MEMORY (`0x8`) clears the block.
- Physical memory: the SDK's physical heap, as `MmAllocatePhysicalMemoryEx`
  does now, so the addresses stay in the physical views that the
  renderer's mirrors and the texture cache rely on. Only the route
  changes for now.
- One lock around the layer: the game allocates from several threads.

Agreed details:

- Flag values not seen before (HeapAlloc's, HeapReAlloc's, VirtualAlloc's
  allocation type and protection, the physical protection bits) are
  logged once each.
- Running out of memory logs and aborts. operator new's new-handler loop
  stays recompiled above the layer and never loops.
- The texture cache's behavior is checked by log (`--log_gpu_memory`)
  against a run with `--native_heap=false`: textures are freed when the
  game loads new ones over their memory, which depends on where memory is
  handed out.
- Harnesses compare what was allocated (sizes, zeroing, data kept across
  Realloc), never pointer values, which differ between the two heaps.

## The swap at the end

The layer exists so that the port's end changes one place. How guest
memory finally goes is decided then (the user's call): one swap of the
backing, or structures moved to host memory one at a time as their last
recompiled user goes. The layer allows either, and nothing here sets up
access rules or interim backings.

## What the counter saw

A temporary counter (commit `43b3900`, removed in the next) overrode the
heap's four functions and VirtualAlloc and VirtualFree, ran the game's own
function first and kept each live block's size on the host: heap blocks
rounded to 8 bytes and pages to 4,096, as the layer would hand them out.
Runs on 2026-10-22: a 60 s attract race (`itc_741.log`) and the pad replay
drive `race_test.jsonl` (`itc_742.log`, `itc_743.log`: the title, the
menus, a race).

| | Attract race | Drive |
|---|---|---|
| Peak in use | 88.5 MiB | 172.6 MiB (in the front end, ~16 s after boot; the race stayed below it) |
| Heap at the peak | 17.2 MiB, 5,071 blocks | 17.3 MiB, 1,843 blocks |
| Pages at the peak | 71.3 MiB, 1,803 blocks | 155.3 MiB, 3,369 blocks |
| Largest request | heap 8,192,000; pages 13,107,200 | heap 8,192,000; pages 16,854,160 |
| Pages rounded to 64 KiB | | 366.6 MiB at the peak |

- The process heap's own flags (heap +24) are 0. HeapAlloc's flags are 0
  (malloc, from `0x825A9AF0`) and 0x8 (calloc, from `0x825AC2D0`);
  HeapReAlloc's 0 (realloc); HeapFree's 0 (free). No other heap handle
  was seen.
- VirtualAlloc's protection is always 4 (PAGE_READWRITE), its address
  always 0, and every call commits. Its types:
  - `0x1000` (MEM_COMMIT), most callers (first from `sub_8209E210`).
  - `0x801000`, MEM_COMMIT with MEM_NOZERO (`0x800000`, the SDK skips the
    clearing): `sub_8258DFF0` in the graphics library (not read in
    detail).
  - `0x20801000`, also with MEM_LARGE_PAGES (`0x20000000`):
    `sub_824EFE20` (not read in detail; it clears what it gets with
    memset itself, and allocates a second block the same way). The SDK
    takes these from the 64 KiB-page heap at `0x40000000`.
- VirtualFree was only ever `(address, 0, MEM_RELEASE)`.
- The SDK's `NtAllocateVirtualMemory` rounds every request without an
  address up to 64 KiB, with 64 KiB alignment, as the Xbox's allocation
  granularity does. The game's ~3,400 page blocks average ~47 KB, so that
  rounding more than doubles what pages take (366.6 MiB against 172.6 at
  the drive's peak).
- The SDK's 4 KiB virtual heap spans `0x00000000`-`0x3FFFFFFF` (1 GiB),
  so a range of a few hundred MiB fits in it.

## Steps

1. This write-up.
2. The counter, for the range's size (done, 2026-10-22: "What the counter saw").
3. The layer, the adapters and the harness cases, in one change.
4. Checks: an attract race with `--run_harnesses=all`, the texture cache
   by log with the flag on and off, the pad replay drive
   (`pad-replay.md`), then a race the user drives.
5. The functions' rows (shown to the user first), the "the user decides"
   notes in `loops/leads.md` and `loops/b7-loop.md` removed, CLAUDE.md's
   B7 sentence, and an entry in "The boundary over time".
