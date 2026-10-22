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

## Still to find

- The range's size: a temporary counter on the heap's four functions and
  the VirtualAlloc wrappers, logging the bytes in use at their peak and
  the flags seen, over an attract race and a drive. Removed afterwards.
- The heap's functions' arguments, read in detail (HeapReAlloc's and
  HeapSize's are inferred from their callers so far).

## Steps

1. This write-up.
2. The counter, for the range's size.
3. The layer, the adapters and the harness cases, in one change.
4. Checks: an attract race with `--run_harnesses=all`, the texture cache
   by log with the flag on and off, the pad replay drive
   (`pad-replay.md`), then a race the user drives.
5. The functions' rows (shown to the user first), the "the user decides"
   notes in `loops/leads.md` and `loops/b7-loop.md` removed, CLAUDE.md's
   B7 sentence, and an entry in "The boundary over time".
