# Known functions: predicated tiling

The graphics library's (the 360's Direct3D, linked into the game)
**predicated tiling**: the menus' 3D views (the car on the Free Run
screen, the Course In city picture) are drawn into a 1280 × 720, 4×
MSAA surface with depth, larger than EDRAM, so the library records the
view's command segments once and has the GPU replay them once per tile
(3 tiles of 1280 × 256, 256, 208 in play), each tile resolved into the
destination texture. `Device::BeginTiling` (`sub_825BED08`) and
`Device::EndTiling` (`sub_825BF200`) are commands 32 and 33 of the render
command replay (`Command::*` 32 and 33, `sub_82590968` and
`sub_825909E8`, Wrapped in `functions.md`); menus only. In between, the
segment flow (`command-buffer.md`) appends each segment to the segment
chain at device +13016 instead of kicking it, and the draws record their
DRAW_INDX's address and screen extent in the draw chain at +12976
(`BeginVertices`, `DrawVertices`, `DrawIndexedVertices`, `sub_825BDDC0`).
At the end, a callback the GPU raises wakes the library's **tiling
worker**, one kernel thread a CPU the library makes at `Device::Init`
(`sub_822B6D70`, `ExCreateThread`), which would compute each draw's tile
mask from its extent and kick the segment chain once per tile with the
tile's bin select written into the primary ring. How the menus use it
and how our renderer draws them: "Predicated tiling" in
`../gpu-functions.md` and in `../native-renderer.md` (and its
"Interrupts are made at the library's level"); the chains, the segment
flow and the callbacks are in [`command-buffer.md`](command-buffer.md);
the device's fields in `../known-data.md`. Mapped by the B7 loop
(`../loops/b7-loop.md`) from its lead in `../loops/leads.md` (found by
the command buffer's mapping agent), 2026-11-06; 8 rows moved from
`functions.md`.

**What runs in play.** Command 32 calls `Device::BeginTiling` (flags 0,
3 tiles), command 33 `Device::EndTiling` (flags `0x303`, the scene
target's texture). Our kick (`Device::DeferredKick`, `sub_8229E848`, and
`Device::Kick`, `sub_8229E3F0`, overridden in `src/native/gpu_link.cpp`)
runs the end's callback block at once, but `Interrupt` there skips the
worker's wake (`sub_822B6A78`, counted as "deferred kick worker wakes
skipped"), so the worker's job (`sub_822B6780`) and everything under it
never ran: the segment chain is closed and never kicked, nothing reads
the chains or the header. The worker threads run: each waits on its
event (never set), the last one made with a 30 ms timeout, after which
it takes the device's critical section +13548 and checks +10810 bit 2
(nothing sets it).

**What our renderer reads.** Our renderer draws from the command list
replay, not from packets (`../native-renderer.md`). It sees commands 32
and 33 through their wrapped handlers (`Passes::BeginTiling` reads
command 32's own payload: the tile rectangles, the clear values) and
each tile's resolve through `Device::Resolve`'s wrap (`sub_822AFA58`,
`src/native/replay_hooks.cpp`, `Renderer_TiledResolve`), which reads the
tile count at device +12488. Through the native device state
(`src/graphics/device_state.cpp`, `device-state.md`) the tiling's device
fields reach the register shadow the renderer reads: +10808 bits 1 and
5 (`UpdatePredicatedTiling`, `TilingHoldsBoundSet`), +10811 bit 6 and
+12912 (`SetRenderTarget`'s surface info), +12456 and the saved surfaces
+12464..+12484, the widest and tallest tile +12916 and +12920
(`Device::SetViewport`'s clip). The viewport and scissor reset, the
clear at the start (`Device::Clear`'s worker `sub_8229FC40`, a lead in
`../loops/leads.md`) and `Device::DirtyAllState` are device state too.

Graphics: what our Vulkan renderer needs stays native, the rest is
Dropped (the user's rule; precedents in `device-state.md`,
`graphics-resources.md` and `command-buffer.md`). The rows say which kind
each looks like; none is decided here. Kept-kind: `BeginTiling`'s and
`EndTiling`'s device fields and calls (the resolves, the clear, the
viewport and scissor, `DirtyAllState`), and `Device::SetPredication`'s
fields (`sub_825BEB80`). Dropped-kind: the predication packets
(`SET_BIN_MASK`, `0xC0006000`), `BeginTiling`'s EVENT_WRITE, the
chains' markers and header (`sub_822B6AD8`'s and `sub_822B6BF8`'s chain
work, once nothing reads the chains: `command-buffer.md` says the same of
the chains themselves), the worker's callback packets and its job, the
L2 lock (`sub_822B6070`), the primary ring's bin select (`sub_822B6370`)
and the debugger report through `0x820007B4` and `0x820007F0` (both 0
without a debugger). Dropping the chains' start would leave +12960 at 0,
so the tiled segments would be kicked as any other (our kick applies
their fences at once); that changes the segment flow's path, not what
the renderer draws, which is the graphics rule's case, but it's the
lead's to confirm. The worker threads are the library's own (the user's
call for XAudio's, 2026-10-29: such threads, their events and waits
through the kernel's imports as the original does, host objects once
every user is native); the device's critical section +13548 only they
take after `Device::Init` makes it (the user's call for `Edram::Alloc`'s
lock, 2026-10-24: a host `std::mutex` taken by every user). Since the
worker's only work in play is a poll of a bit nothing sets, the threads
also look Dropped-kind under the graphics rule; that's for the lead.

Callers in other files: the replay's commands 32 and 33 (`functions.md`),
`Device::Init` (`sub_822AD298`, overridden in
`src/hooks/graphics_device.cpp`, calling `sub_822B6D70` through `ctx`);
the draws that append to the draw chain (`BeginVertices` `sub_825BD8F0`,
`sub_825BDDC0`, `DrawVertices` `sub_825BE300`, `DrawIndexedVertices`
`sub_825BE708`, in `functions.md`). Callees in other files: the chains,
the segment flow, `Device::SaveSegment` and `RestoreSegment`, the
callbacks, the kick and the primary ring's writer (`sub_8229DD70`,
`sub_8229DCC0`) and the waits' helpers (`sub_8229D6F0`, `sub_8229D8A0`,
`sub_8229D720`) in `command-buffer.md`; `Device::SetViewport`,
`SetScissorRect` and `DirtyAllState` in `device-state.md`; the data
cache flush `sub_822AC470` (`command-buffer.md`, Dropped); memcpy
(`sub_825A7410`, `c-runtime.md`); `Device::Resolve` (`sub_822AFA58`,
`functions.md`, Wrapped; a lead with its callees), `Device::Clear`'s
worker (`sub_8229FC40`, a lead); the present callback `sub_822A0718`
and `Device::Swap` (`functions.md`, called by the worker only with
+10810 bit 2). Not mapped: `sub_822B7540`, the only caller of
`sub_822B6AA8` (not seen in play, not read).

**Ready groups** (`b7_groups.py`): `sub_822B6AD8`, `sub_825BEB80`,
`sub_822B6BF8` (with `Device::RestoreSegment`, `sub_8229E0C0`, not seen
in play), `sub_822B6D70` and `sub_822B69A8` (the tool counts the present
callback `sub_822A0718` in its group, as its other callers in play are
native and call it through `ctx`; it's `functions.md`'s, below
`VideoOutput::WaitForVblank`). Blocked: `Device::BeginTiling` by
`Device::Clear`'s worker (`sub_8229FC40`, and the draw's packet writers
under it), `Device::EndTiling` by `Device::Resolve` (`sub_822AFA58`).

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
| `sub_825BED08` | `Device::BeginTiling` | | Graphics library; calls `sub_822A3B90`, sets +10811 bit 6. Not read. Read 2026-11-06 (B7 loop): r3 the device, r4 the flags (0 in play), r5 the tile count, r6 the tiles (16 bytes each: left, top, right, bottom), r7 the clear color (16 bytes; 0 for zeros), f1 the clear Z, r9 the stencil; r8 not read. With a debugger's callback (`0x820007B4`'s or the debug monitor data's `0x820007F0` +24, both 0 in play) reports code 43 (86, or 92 with flags bit 1). Stores the count at +12488 and the tiles at +12492 (16 bytes each), their top-left rounded down to 32 at +12732 (8 bytes each), and the widest right and tallest bottom at +12916 and +12920. With flags bit 1 (never in play): the tile size from RB_SURFACE_INFO's sample count (+10240), the surface's pitch rounded up to it at +12912, each tile's offset in tiles at +12852, and +10811 bit 6 set. +12924 the flags. Unless flags bit 0 or +10811 bit 6: the viewport and scissor saved, set to the defaults at `0x82013600` and `0x82013618` (`Device::SetViewport`, `SetScissorRect`), a clear of the rectangle (0, 0, the widest tile, the tallest) through `Device::Clear`'s worker (`sub_8229FC40`, r4 63, r6 the color, f1, r8 the stencil), and the two put back. Then +12928 the clear color (or zeros), +12944 Z, +12948 the stencil for the tiles after the first; the bound surfaces saved (+12468..+12484 from +12184..+12200; +12464 render target 0, or the depth surface without one); +12460 2, +12456 0, +10808 bit 5 set, the tiling bit (+10808 bit 1) worked out again as `SetRenderTarget` does; +12452, +10800, +10804 0 and +12448 -1. Then `sub_822B6AD8`, the marker `0x86000000` appended to the segment chain (+13016, `CommandChain::NewBlock` when full), an EVENT_WRITE (`0xC0004600`, 25) at the write position (the segment ended first when past the soft end), `Device::SetViewport` with `0x82013600` and `Device::DirtyAllState(device, 1)`. No return value of its own. Only from `Command::*` 32 (`sub_82590968`); menus only. Kept-kind: its fields, the clear, the viewport and `DirtyAllState`; Dropped-kind: the report, the marker and the EVENT_WRITE. Blocked by `sub_8229FC40` (a lead). |
| `sub_822B6AD8` | `Device::BeginTiledSegments` (probably) | Overridden | Read 2026-11-06 (B7 loop). r3 the device: ends the segment (`sub_8229EF48`), saves the segment state (`Device::SaveSegment`, `sub_8229DBF0`), sets the tiling's limit +12960 and generation +12964 from the last tail's +12968 and +12972 (+1), copies the segment chain (+13016, 20 bytes) to +13036, then starts the three chains (`CommandChain::Init`, `sub_8229ED60`): the segments +13016, the draws +12976, the third +12996. In the segment chain's first block reserves 256 bytes (`CommandChain::NewBlock`, `sub_8229EB10`, when they don't fit) for a header the worker's job reads: +0 and +4 the draw chain's and the third chain's first blocks (`0xC0000000` + their physical addresses), +8 the flags (+12924), +12 the tile count (+12488), +16 the 240 bytes from +12492 (the tiles, through memcpy `sub_825A7410`); the chain's cursor +13024 moved past it. Only from `Device::BeginTiling`. Seen in play. Ready group. Dropped-kind once nothing reads the chains, except the limit +12960 the segment flow reads (see the intro). Native 2026-11-06 (B7 loop): `BeginTiledSegments` in `src/graphics/predicated_tiling.cpp` behind its adapter (`--native_predicated_tiling`; its caller `Device::BeginTiling` is recompiled), calling the command buffer's plain C++ (`SaveSegment`, `InitChain`, `NewBlock`) and the segment end through `ctx` (`sub_8229EF48`'s counting override). Kept: the tiling's limit +12960 and generation +12964 (the segment flow reads them), the chain copy +13036, the three chains (the segment flow and the draws append to them) and the 256 bytes kept for the header with the cursor past them; the header's words aren't written (only the worker's job reads them, which never runs; with them the memcpy). Returns the header + 16 (the original's memcpy's r3). Harness `predicated_tiling`: "without the header" matches; "against the original as it is (expected to differ)" differs by the header's 256 bytes. |
| `sub_825BEB80` | `Device::SetPredication` (probably) | Overridden | Read 2026-11-06 (B7 loop). r3 the device, r4 the predication select (two bits a tile: `3 << 2 × tile`, or 0 for none). With 0: +12456 0, the tiling bit (+10808 bit 1) worked out again as `Device::SetRenderTarget` does, +12448 -1. Else +12448 r4, +12456 1, +10808 bit 1 cleared. Both: +12452 0, +10800 r4, +10804 0, then a `SET_BIN_MASK` packet (`0xC0006000`, +12448) at the write position (the segment ended first when past the soft end). Leaf (but `sub_8229EF48`). Seen in play, from `Device::EndTiling` (before each tile's resolve, then 0; around the inserted callback). Ready group. Kept-kind: the fields (the native device state reads +12456 and +10808); Dropped-kind: the packet. Native 2026-11-06 (B7 loop): `SetPredication` in `src/graphics/predicated_tiling.cpp` behind its adapter (`src/hooks/predicated_tiling.cpp`, `--native_predicated_tiling`); its caller (`Device::EndTiling`) is recompiled. The fields and the segment end when past the soft end kept; the `SET_BIN_MASK` packet left out (Dropped by the graphics rule: our kick doesn't act on it), so the write position stays and r3 is it. Harness `predicated_tiling`: "without its packet" (the original with the packet's two words put back as the native version leaves them, the write position and r3 before them) matches; "against the original as it is (expected to differ)" differs by those words. Frame dumps of the tuning shop's tiled car with the flag on and off match (itc_3643/3644). |
| `sub_825BF200` | `Device::EndTiling` | | Not read in detail (from `gpu-functions.md`, "Predicated tiling"): r4 flags (`0x303` seen), then the destination texture. Resolves each tile through `Device::Resolve` (calls at `0x825BF348`, and `0x825BF378` for the last), adding `0x100` and `0x200` on every tile but the last; ends with `sub_822B6BF8`. From `Command::*` 33 (`sub_825909E8`); menus only. Read 2026-11-06 (B7 loop): r3 the device, r4 the resolve flags, r5 the tiles (0: the device's +12492), r6 the destination texture (0: no resolves), r7, f1, r9 and r10 the last tile's clear color, Z, stencil and its last stack argument. For each of the +12488 tiles: the tile's rectangle copied, its right and bottom rounded up to 8 when they're the surface's edge (+12464's +36 size); `Device::SetPredication(3 << 2 × tile)` (`sub_825BEB80`); `Device::Resolve` (`sub_822AFA58`) with the rectangle, the texture and the tile's top-left as the destination point; on all but the last tile the flags with `0x100` (render target 0 bound, +12184) and `0x200` (depth bound, +12200) and the tiling's clear values (+12928, +12944, +12948), on the last its own. Then `SetPredication(0)`, the segment ended (`sub_8229EF48`); the CPUs for the callbacks: +12924's bits 24-29, else `0x01000000 << +11208` (the last worker's CPU). With more than one tile: `Device::InsertCallback` (`sub_8229F228`) of the worker's wake `sub_822B6A78` with the segment chain's first block (`0xC0000000` + its physical address), between `SetPredication(3)` and `(0)`. The segment ended again, the marker `0x86100000` appended to the segment chain (+13024); +12464, +12456 0, +10808 bit 5 and +10811 bit 6 cleared, the tiling bit worked out again, +12452, +10800, +10804 0, +12448 -1; `sub_822B6BF8(device, the tile count, the CPUs)`; `Device::SetViewport` with `0x82013600`. Returns 0, or `0x8007000E` (out of memory) with `Device::DirtyAllState(device, 0)` when `sub_822B6BF8` failed. Blocked by `Device::Resolve` (a lead). Kept-kind: the resolves, the fields, the viewport; Dropped-kind: the predication packets, the callback, the marker. |
| `sub_822B6BF8` | `Device::EndTiledSegments` (probably) | Overridden | The tiling worker's callbacks and job; logged with `--native_renderer_log_gpu`. Read 2026-11-06 (B7 loop): r3 the device, r4 the tile count, r5 the CPUs (`sub_8229E9D0`'s flags). Unless out of space (+10809 bit 6), the coherency flush packets (`sub_8229E5D0`, Dropped). Reserves 100 words of data (`sub_8229E308`) and writes the worker's wake there (`Device::WriteCallback`, `sub_8229E9D0`: r5 the CPUs \| 1, `sub_822B6A78` with the segment chain's first block); reserves 4 more words (probably the queue entry `sub_8229E848` takes in r4, as the original queued kicks). Clears the tiling's limit +12960 and +12964. Out of space: `Device::RestoreSegment` (`sub_8229E0C0`), bit 6 cleared, returns 1. Else closes the three chains (`CommandChain::Close`, `sub_8229DFB8`), defers the flush block (`sub_8229E848`, r7 0, the list +13036), and, unless the GPU hung (+10809 bit 1 with +19980 0), defers the callback block with 1 interrupt to come (2 with more than one tile); returns 0. Only from `Device::EndTiling`. Our kick runs the callback block at once and skips the wake (`Interrupt`, `src/native/gpu_link.cpp`). Its wrap (`src/native/gpu_link.cpp`) only logs with `--native_renderer_log_gpu`. Ready group (with `sub_8229E0C0`). Kept-kind: clearing +12960 (the segment flow reads it); Dropped-kind: the flush, the chains' close and the callback (nothing runs the worker's job). Native 2026-11-06 (B7 loop): `EndTiledSegments` in `src/graphics/predicated_tiling.cpp`; its wrap in `src/native/gpu_link.cpp` still logs with `--native_renderer_log_gpu` and hands the work to `hooks::EndTiledSegments` (`--native_predicated_tiling`; a diagnostics wrap kept around the native body, as the 29th run's). The command buffer's plain C++ (`Reserve`, `WriteCallback`, `CloseChain`, `RestoreSegment`) and the kick through `ctx` (`hooks::CommandBufferKick`); the coherency flush (`sub_8229E5D0`, Dropped: no block) and its kick left out. The callback and the chains' close kept (the kick's scratch writes and interrupt are effects our kick applies). The original's stack words read when a reservation fails are never used (a failed reservation sets +10809 bit 6, which takes the restore path). Harness `predicated_tiling` (the callback block's kick into the game's callback block, as in play). |
| `sub_822B6D70` | `Device::CreateWorkers` (probably) | Overridden | From `Device::Init`: creates the library's worker threads (`ExCreateThread`, `KeSetBasePriorityThread`) and ends the command segment; 0 on failure. Not read in detail. Read 2026-11-06 (B7 loop): r3 the device. Nothing with +20440 bit 8. Else the workers' shared state at +10812 (`+10816` and `+10824` from the primary ring's +13496 and +13500, +10872 0, +10904 `0xC0000000`, +10920 `0x7F100000` (the L2-locked area), an event at +10876), then for each CPU 0 to 5 with +20440 bit `0x01000000 << cpu`: +11208 the CPU, a 56-byte block at +11212 + 56 × CPU (+0 the shared state, +4 the CPU, +8 and +16 a 32 KB slice of the L2 area, +20 its end, +24 `0xC0000002`, +28 `0x7F000000` + 128 × CPU, +32 a notification event (a KEVENT made in place), +48 the thread's handle, +52 its id); an EVENT_WRITE_SHD (`0xC0025800`, event 3) at the write position (the segment ended first when past the soft end) writing block +12's value to the address at +28; `ExCreateThread` (32 KB stack, `sub_822B69A8` with the block, the CPU in the flags' top byte) and `KeSetBasePriorityThread(17)`. Returns 1, or 0 when a thread wasn't made. Once at boot, from `Device::Init` (overridden, through `ctx`). Ready group. Threads, events and waits the library makes for itself (the user's call for XAudio's, 2026-10-29); the EVENT_WRITE_SHD is Dropped-kind. Native 2026-11-06 (B7 loop): `CreateWorkers` in `src/graphics/predicated_tiling.cpp` behind its adapter (`src/hooks/predicated_tiling.cpp`, `--native_predicated_tiling`; `Device::Init`'s override calls it through `ctx`). The threads made and their priority set through the kernel's imports (`ExCreateThread`, `KeSetBasePriorityThread`) as the original (the user's call for XAudio's threads, 2026-10-29), the events' headers written in place, the segment end when past the soft end kept; the EVENT_WRITE_SHD packets left out (Dropped by the graphics rule: only `sub_822B64A0`, never run, reads what they write). Harness `predicated_tiling`: the cases without threads (bit 8, or no CPU bits: a real thread would outlive the case); the boot checked the rest: the shared state and the 6 blocks after `Device::Init` (handles and ids aside) were the same with the flag on and off, the write position 16 bytes short (one worker in play, its packet left out; itc_3647/3648). |
| `sub_822B69A8` | `Device::WorkerThread` (probably) | | The library's worker thread (r3 its block). Waits on its event (`KeWaitForSingleObject`, block +32); on a timeout (258), under the device's lock (+13548), when +10810 bit 2 is set (no code sets it): `sub_822A0718` and Swap with the header at +13444 (shows the last frame again); else `sub_822B6780`. Its event is set by the interrupt callback `sub_822B6A78`. Not read in detail. Read 2026-11-06 (B7 loop): the wait has a 30 ms timeout (-300,000 × 100 ns) only for the worker of the CPU at +11208 (block +4 equal to the shared state's +396; the others wait without one); the lock is `RtlEnterCriticalSection` on `VdGlobalDevice`'s +13548, which nothing else takes after `Device::Init`; on its event: `KeResetEvent`, then exits when the shared state's +4 (+10816) is 0, else runs the job `sub_822B6780` and waits again. Seen in play (polling). Ready group (with `sub_822A0718`, see the intro). The thread is the library's own (XAudio's precedent, 2026-10-29); the lock a host `std::mutex` by `Edram::Alloc`'s precedent (2026-10-24); Dropped-kind under the graphics rule if the threads go (see the intro). |
| `sub_822B6A78` | `Device::WakeWorker` (probably) | Wrapped | The tiling worker's callbacks and job; logged with `--native_renderer_log_gpu`. Read 2026-11-06 (B7 loop): the callback the GPU raises (r3 its argument, the segment chain's first block): stores it at `VdGlobalDevice`'s +10892 (the shared state's +80) and sets the event of the calling CPU's worker (`KeSetEvent` on +11244 + 56 × the PCR's CPU, +268). No direct callers: inserted by `Device::EndTiling` (`sub_8229F228`) and `sub_822B6BF8` (`sub_8229E9D0`). Not seen in play: our kick skips it (`Interrupt` in `src/native/gpu_link.cpp`, "deferred kick worker wakes skipped"); its wrap only logs. Dropped-kind. |
| `sub_822B6AA8` | `Device::WakeLastWorker` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): as `sub_822B6A78`, but sets the event of the worker of the CPU at +11208. Only from `sub_822B7540` (not seen in play, not read). |
| `sub_822B6780` | `Device::WorkerJob` (probably) | Wrapped | The tiling worker's callbacks and job; logged with `--native_renderer_log_gpu`. Not seen in play (its wake is skipped). Read 2026-11-06 (B7 loop): r3 the worker's block. Takes the shared state's spin lock (+0, `lwarx`/`stwcx.`) and counts the job's depth (+48); a wake while it runs (depth above 1) continues the saved walk. Takes the header address (+80, `sub_822B6A78`'s argument; none: the depth and lock cleared, returns), copies the header's 256 bytes to +112, computes the draws' tile masks (`sub_822B6288`) when +44 is 1, resets the bin select (+368 `0x7FFFFFFF`, `sub_822B6370`), then walks the segment chain: plain entries (a count in the high half) call the function at +16 with +20 for each; `0x80000000` sets the function and argument from the next two words; `0xC0000000`-kind words follow or wait for the next block (`sub_822B60C8`); the `0x81`-`0x87` markers go to `sub_822B6528`, which replays the tiles. Its wrap logs only. Not read in detail. Dropped-kind. |
| `sub_822B6288` | `Device::MaskTiledDraws` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 the header (in the job's copy): `sub_822B6188` over the draw chain (+0) without a flush, then over each block the third chain (+4) lists, with one. From `sub_822B6780`. |
| `sub_822B6188` | `Device::MaskTiledDrawBlock` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 the header, r4 a chain block, r5 whether to flush: for each 16-byte entry (the DRAW_INDX's address, then the draw's extent as four halfwords in 8-pixel units) and each tile (the header's +16 rectangles, +12 the count), sets the tile's two bits where they overlap and stores `0x80000000` \| the mask at the draw's +8; with r5 the data cache flush (`sub_822AC470`); follows the blocks until the end marker `0xC0000000`. From `sub_822B6288`. |
| `sub_822B60C8` | `Device::WorkerNextBlock` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 the shared state, r4 a chain position. Under the state's spin lock (+60, `KfAcquireSpinLock`): at the end marker `0xC0000000` (the next block not written yet) counts the depth +48 down and, at 0, the interrupts to come +56 (+10868) and `KeSetEvent` on +64, else `KeResetEvent`; keeps the position at +36, releases, waits on +64 and returns 0; otherwise returns the word (the next block). From `sub_822B6780`. Not read in detail. |
| `sub_822B6528` | `Device::WorkerMarkers` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 a chain position, r4 the shared state: walks the chain's marker words by their top byte: `0x81` kicks the segment (`Device::Kick` `sub_8229E3F0` with its size and address), `0x86000000` (the tiling's start) keeps the position (+388, +392), `0x86100000` (the end) moves to the next tile (+44 against the header's count +124) and back to the start, `0x86200000` locks the L2 area (`sub_822B6070`), `0x86300000` waits for the workers (`sub_822B64A0`) and unlocks it (`KeUnlockL2`), `0x82`-`0x84` probably nest and return (+40, +372..+384); the bin select after each (`sub_822B6370`). Returns the position. From `sub_822B6780`. Not read in detail. |
| `sub_822B6370` | `Device::WriteBinSelect` (probably) | | Calls `sub_8229DD70` (writes into the primary ring). Not read. Not seen in play. Read 2026-11-06 (B7 loop): r3 the shared state: the bin select for the current tile (two bits at 2 × +44, of 1 or 2 by +40's top bits; bit 31 too when +44 is 0 and +40's sign bit clear); when it differs from +368, stores it there and writes `SET_BIN_SELECT_LO` (`0xC0006200`, the select) straight into the primary ring (`sub_8229DD70`, `command-buffer.md`), told to a capture object (+19980's slots 6 and 7) and the debugger's callback (+20124) around it. From `sub_822B6780` and `sub_822B6528`. |
| `sub_822B6070` | `Device::LockL2` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): `KeLockL2(0, 0x7F100000, 0x20000, 1, 1)` and zeroes those 128 KB with `dcbzl`. From `sub_822B6528`'s `0x86200000` marker. |
| `sub_822B64A0` | `Device::WaitForWorkers` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): for each of the 6 worker blocks with +16 set, waits (`sub_8229D6F0`, `sub_8229D8A0`, `sub_8229D720`, `command-buffer.md`) until the word at the block's +28 address equals +8 with +12's low two bits. From `sub_822B6528`'s `0x86300000` marker. |
