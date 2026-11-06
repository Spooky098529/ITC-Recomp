# Known functions: the command buffer

The graphics library's (the 360's Direct3D, linked into the game)
**command buffer**: the memory its packet writers fill (device +40 the
write position), cut into **segments** that are kicked to the GPU one at a
time, with **data space** reserved beside them (`BeginVertices`' inline
vertices, `Device::Resolve`'s and the clears' blocks, the vertex shader
patch's copies), the **fences** and the GPU's read position each segment
writes into the progress block at its end, the **waits** on them, the
**callbacks** a segment asks the GPU to raise on a CPU, the coherency
flush packets for memory the CPU wrote, and the **chains** of 4 KB blocks
predicated tiling records its segments and draws in. The functions are
`8229DBF0`-`8229F228`, with `Device::WriteDefaultDrawRegisters`
(`sub_822AF978`) and the data cache flush (`sub_822AC470`). How the
library drives the GPU is in `../gpu-functions.md` ("Waits on the GPU",
the draw functions), the kick and the progress block in
`../native-renderer.md` ("Plan for milestone 6", "Exclusive mode", "The
indexed draw without its packets"), the device's fields in
`../known-data.md`. Mapped by the B7 loop (`../loops/b7-loop.md`) from its
lead in `../loops/leads.md` (the command buffer, found by the device
state's mapping agent), 2026-11-06; 16 rows moved from `functions.md`.

**The buffer.** `Device::SetUpRing` (`sub_8229F280`, `functions.md`,
overridden) allocates it: +13504 its start, +13508 its end, +13512 a
segment's size. A segment runs from +13528 to the end +44 (the soft end
+48 = +44 − 160: every packet writer ends the segment through
`sub_8229EF48` when its write position +40 has passed it), keeping 14
words for its tail. Data is reserved after the segment (the data cursor
+13516) and wraps to the buffer's start (the wrap cursor +13524, the
generation +13520) without passing the open segment; past the segment's
data budget (+13532 against +13536) the segment is cut short. Ending a
segment (`sub_8229EDA8`) writes its tail (`sub_8229E768`: the read
position and the fence, two EVENT_WRITE_SHD into the progress block at
+10768, the fence counter +10780 advanced by 2) and kicks it
(`Device::DeferredKick`, overridden: our kick, `src/native/gpu_link.cpp`,
walks the segment at once and applies only its effects outside the GPU:
the fences and positions as memory writes, the scratch registers, the
interrupts as calls to the callback block's functions). Under predicated
tiling (+12960 set by the tiling's start, `sub_822B6AD8`, a lead) segments
are appended to the chain at +13016 instead and kicked per tile by the
library's tiling worker. When nothing can be reserved (+10809 bit 6),
writers are pointed at a 4,800-byte dummy segment at +15176. A recording
command buffer object (+10808 bit `0x80`, the object at +13160:
`sub_822AD690`, `sub_822AD8F8`, `sub_822AD850`) never ran in play; those
functions get no rows.

**What our renderer reads.** Our renderer draws from the command list
replay and the device's register shadow, not from packets
(`../native-renderer.md`). From the command buffer it reads:

- **Inline vertices**: `BeginVertices` (`sub_825BD8F0`, `functions.md`,
  Wrapped) reserves the vertices' space with `sub_8229E308` and returns
  it; the caller writes the vertices there, and at `End` (`sub_825BDDA0`)
  our hook copies count × stride bytes from that address into the
  renderer's mirror (`Renderer_InlineDraw`, `src/native/replay_hooks.cpp`;
  `Passes::InlineDraw`, `src/native/passes.cpp`), setting the inline fetch
  constant (95, with 93 and 94) itself. So it needs only guest memory the
  caller can write that stays valid from `BeginVertices` to `End`; not
  the packets `BeginVertices` writes around it, nor the segment.
- **The vertex shader patch's copy**: `sub_822AEBF0` (from
  `Device::LoadShaders`) copies a vertex shader's microcode into space
  reserved at the write position (`sub_8229F148`) and patches it there;
  our hook on the patch (`sub_822AE340`, `src/native/replay_hooks.cpp`)
  copies the patched microcode out right after. It needs the space valid
  during the patch only.
- **Fences, positions and callbacks**, through our kick: the waits here
  (`sub_8229DC28`, `sub_8229E908`, `sub_8229F1B0`) read the progress
  block the segments' tails write; the callbacks (`sub_8229E9D0`) are
  how the present callback (`sub_822A0718`) and the tiling worker's wake
  (`sub_822B6BF8`) run. None of the waits loops in play (the wait helpers
  `sub_8229D6F0`, `sub_8229D8A0`, `sub_8229D720` never ran): the kick
  completes everything at once.

Graphics: what our Vulkan renderer needs stays native, the rest is
Dropped (the user's rule; the precedents are `device-state.md`'s packets,
`graphics-resources.md`'s fence waits and cache flushes, 2026-10-24). The
rows say which kind each looks like; none is decided here. Kept-kind: the
inline vertex space (as guest memory valid until `End`) and the shader
patch's space; and, for as long as recompiled code writes packets whose
fences and callbacks something reads, the segment flow (the reserve, the
segment end, the tail, the next segment) and the callback packets.
Dropped-kind: the coherency flush packets and the pending flush range
(the user's call on cache flushes), the data cache flush, the waits (they
pass at once; fence waits the user's call), the default draw registers'
packets (`sub_822AF978`; its dirty marks are kept-kind, as
`device_state.cpp` keeps them for `sub_8229F680`), and the tiling chains
once predicated tiling's recompiled worker no longer reads them. A native
library with no recompiled packet writers left would need none of the
buffer: fences as a counter, callbacks called directly, inline vertices
from any guest scratch memory.

**Native code** (the B7 loop, 2026-11-06): plain C++ in
`src/graphics/command_buffer.cpp` over the device in guest memory, which
the recompiled packet writers share, behind adapters in
`src/hooks/command_buffer.cpp` (`--native_command_buffer`, falling back to
the originals); harness `command_buffer` (a device, a command block and a
progress block of its own, every word random). The segments' tail
(`sub_8229E768`), the tiling's segment save (`sub_8229DBF0`) and the
chains' close (`sub_8229DFB8`) write what the originals write.

Callers in other files: `BeginVertices` (`sub_825BD8F0`),
`DrawIndexedVertices` (`sub_825BE708`), `Device::Resolve`
(`sub_822AFA58`), `Device::EndTiling` (`sub_825BF200`), `sub_825BED08`,
`Device::Swap` and `Device::SetUpRing` (overridden, calling
`sub_8229EF48`, `sub_8229EC00`, `sub_8229F0E0`, `sub_8229F1B0` and
`sub_822AC470` through `ctx`, `src/hooks/graphics_device.cpp`),
`sub_82091BC0` (the draw listener's GPU wait), `sub_822A0718` (the
present callback), `sub_822AEBF0`, the draw's packet writers
(`sub_822ADC08`, `sub_822ADEF0`, `sub_822AE070`, `sub_822AED18`) and the
tiling worker (`sub_822B6BF8`, `sub_822B6780`), all in `functions.md`;
`sub_8229F680` (`device-state.md`, native without its calls here);
`Resource::Lock` and `Unlock` (`graphics-resources.md`, native without
theirs). Not mapped here (leads): the tiling's start (`sub_822B6AD8`)
and `Device::Clear`'s worker (`sub_8229FC40`). Not seen in play, so no
rows: the recording command buffer's `sub_822AD690`, `sub_822AD8F8`,
`sub_822AD850`, the fence list's flush `sub_822AD790` and the wait in `sub_822B64A0`.

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
| `sub_8229E308` | `Device::AllocateFromCommandBuffer` (probably) | | Read 2026-11-06 (B7 loop). r3 the device, r4 a size in words, r5 the alignment in bytes; returns an address in the command buffer's data space (beside the segment's packets), or 0. With +10809 bit 6 (out of space) returns 0 at once. Else `sub_8229E100` with r4 1 (data) and the size in bytes (with a recording object at +13160, never in play, `sub_822AD690` or `sub_822AD8F8` by its +152); adds the size to the segment's data count +13532 and, past the budget +13536 outside tiling (+12960 0), cuts the segment short (soft end +48 = the write position +40, end +44 = +40 + 160) so the next writer ends it, moving the data cursor +13516 to the buffer's end +13508 when the wrap cursor +13524 isn't at the start +13504; on 0 sets +10809 bit 6. 11 callers, 7 seen in play: `BeginVertices` (`sub_825BD8F0`, `functions.md`: the inline vertices, r4 count × stride / 4, r5 16), `Device::Resolve` (`sub_822AFA58`), `Device::Clear`'s worker `sub_8229FC40` (a lead), the tiling's end `sub_822B6BF8`, `sub_8229E5D0` and `sub_8229EB10` here and `sub_8229EF48` (the flush block's 4 words); `sub_8229F680` (`device-state.md`, native without it), `sub_822AD790`, `sub_822AD850` and `sub_825BDDC0` not seen. Kept-kind for `BeginVertices`: our renderer copies an inline draw's vertices from the address it returns at `End` (`sub_825BDDA0`; `Renderer_InlineDraw`, `src/native/replay_hooks.cpp`, and `Passes::InlineDraw`, `src/native/passes.cpp`), so it needs guest memory the caller can write that stays valid until `End` (not the packets around it: the renderer sets the inline fetch constant itself). Dropped-kind for the other callers' packet data. |
| `sub_8229E100` | | | The next command buffer segment, for `sub_8229EC00` (r4 2, r5 the size, r6 32); sets +10810 bit 3 among others. Not read in detail. Read 2026-11-06 (B7 loop): r3 the device, r4 the kind (bit 0 set: data, from `sub_8229E308`; clear: a segment, from `sub_8229EC00` with 2), r5 a pointer to the size in bytes (a segment's actual size is written back), r6 the alignment. Data is placed from the data cursor +13516 (0: the segment's end +44 + 60) up to the buffer's end +13508, the cursor moved past it; beyond the end, from the wrap cursor +13524 with the generation +13520 + 1, never past the open segment's start +13528. A segment starts 32-byte aligned after the data cursor (or the write position +40 + 4) and takes at least the size asked (54 words when none) or the segment size +13512, capped at the buffer's end; when that's too short it wraps (from +13524 aligned, +13524 set back to the start +13504, the generation + 1 and +10810 bit 3 set, so `sub_8229E5D0` adds a coherency flush of the whole buffer; probably). Under predicated tiling (+12960 the address the tiled frame's segments reach, +12964 its generation) it fails rather than pass it. Before returning, waits for the GPU to have read past the end (`sub_8229DC28`, with the end and the generation). Returns the address, or 0. Kept-kind with the segments and the inline vertex space (`sub_8229E308`). Ready group with `sub_8229DC28`. |
| `sub_8229DC28` | | | Read 2026-11-06 (B7 loop). Waits until the GPU has read past r4 in generation r5 (r3 the device): the progress block's (+10768) +4 holds the last kicked segment's end with its generation's low 2 bits (written by the segment's tail, `sub_8229E768`); passes when the generations are the same, or one apart with r4 at or before that end; else spins with the wait helpers (`sub_8229D6F0` code 2, `sub_8229D8A0`, `sub_8229D720`). Only from `sub_8229E100` (its ready group). Passes at once in play: our kick writes the position as it kicks, and the helpers never ran. Dropped-kind (a wait). |
| `sub_8229F148` | | | Reserves r4 words in the command buffer: ends the segment if +40 + 4·r4 passes the end (+44), then a new segment (`sub_8229EC00`) if still short; returns the write position (+40; the words are written after it), or 0. From Swap (64 words for `VdSwap`), `sub_8229B9C8`, `sub_8229BFE8`, `sub_822AEBF0`, the gamma packets `sub_822B0C38`/`sub_822B0D18`, `RetrainEdram`, `Device::SetUpScaler`, `sub_822B5208`. Read 2026-11-06 (B7 loop): the segment end is `sub_8229EF48` (it kicks); returns 0 when the next segment fails (+10809 bit 6). 9 callers, one seen in play: `sub_822AEBF0` (from `Device::LoadShaders`, `functions.md`), which copies a vertex shader's microcode into the reserved words and patches the copy there; our hook on the patch (`sub_822AE340`, `src/native/replay_hooks.cpp`) copies the patched microcode out right after, so that space is kept-kind (valid during the patch). The others write packets (Dropped-kind). |
| `sub_8229EF48` | | Wrapped | Ends the command segment when the buffer is full; counted for `--native_renderer_check_draw_writes` (`src/native/draw_packets.cpp`). Read 2026-11-06 (B7 loop): r3 the device; returns the write position +40 after it. Outside predicated tiling (+12960 0) and recording (+10808 bit `0x80`): the pending coherency flush block (`sub_8229E5D0`), when it has words, kicked on its own ahead of the segment (4 words reserved with `sub_8229E308`, `Device::DeferredKick` with the list +13016). Then `sub_8229EDA8` (the segment ended and kicked, the next started). With the "training failed" flag `829537BC` set (never: `Device::Init` leaves it 0) and +10809 bit 1 clear, waits for the segment's fence (`sub_8229E908`, +10780 − 2, code 0) and sets +10809 bit 1. 73 call sites (every packet writer when past the soft end +48, 21 callers seen in play); `Device::Init`'s, `Device::SetUpRing`'s and `Device::Swap`'s overrides call it through `ctx`. Kept-kind while recompiled code writes packets whose fences and callbacks the kick applies. |
| `sub_8229EDA8` | `Device::KickSegment` (probably) | | Read 2026-11-06 (B7 loop). r3 the device. Ends the open segment (from +13528 to the write position +40 + 4) and starts the next. With +10809 bit 6 nothing is kicked. Recording (+10808 bit `0x80` with the object at +13160, its +152 0; never in play): the segment's physical address and word count appended to the object's list (+13180, `sub_822AD850` when it reaches +13184). Predicated tiling (+12960 set): `0x81000000` \| the word count and the physical address appended to the chain at +13016 (`sub_8229EB10` for a new block), kicked per tile by the tiling worker. Else the tail (`sub_8229E768`) and `Device::DeferredKick` (`sub_8229E848`, overridden: kicks at once) with the physical start, the word count, r7 0 and the list +13016. Then a new segment (`sub_8229EC00`, r4 0) when the next start (32-byte aligned) passes the soft end +48; else the next segment starts there (+13528; +40 = it − 4) and, with +19980 set, the data cursor +13516 is rounded up to 4 KB. Only from `sub_8229EF48`. Kept-kind while the kick reads segments. |
| `sub_8229E768` | `Device::WriteSegmentTail` (probably) | Overridden | Read 2026-11-06 (B7 loop). r3 the device, r4 the write position (words stored with a pre-increment); returns r4 + 40. Writes 10 words: INVALIDATE_STATE (`0xC0003B00`, `0x7FFF`), then two EVENT_WRITE_SHD (`0xC0025800`, event 3): the progress block's +4 gets the segment's end (the write position) \| the generation & 3 (+13520), its +0 the fence +10780. Stores that position and generation at +12968 and +12972 (read by the tiling's start, `sub_822B6AD8`), and with +10809 bit 1 ("GPU hung") and +19980 0 writes both values into the progress block itself. Then +10780 += 2. Leaf; only from `sub_8229EDA8`. Our kick applies the two EVENT_WRITE_SHD as memory writes (`src/native/gpu_link.cpp`): the read position waits (`sub_8229DC28`) and fence waits (`sub_8229E908`) read them. Kept-kind while recompiled waits and fences read the progress block. Ready group (a leaf). Plain C++ in `src/graphics/command_buffer.cpp` since 2026-11-06 (`WriteSegmentTail`, the same writes in the original's order) behind an adapter (`src/hooks/command_buffer.cpp`, `--native_command_buffer`); harness `command_buffer` (200 cases: the hung bit and +19980 each way, the progress block in the 0xE0000000 view and elsewhere). Seen running in an attract race (itc_3596). |
| `sub_8229EC00` | | | The next segment and the frame throttle: r3 device, r4 words (Swap: +13512 >> 2). With +10809 bit 6: the write position at +15176. Else the next segment from `sub_8229E100` (or `sub_822AD690`/`sub_822AD8F8` through the object at +13160), +13528 its start, +13516 and +13532 zeroed, write position +40, end +44, soft end +48 (+44 − 160). Then waits (`sub_8229E908`) for the fence +13540 (+13544 with +20440 bit 0) and zeroes it. Returns the segment (0 on failure: bit 6 set). From Swap, `sub_8229F148`, `sub_8229EDA8`. Needed to draw: the next frame's commands go there; the wait passes at once in exclusive mode. Read 2026-11-06 (B7 loop): with +10809 bit 6 already set, or when `sub_8229E100` fails (setting it), the write position goes to the 4,800-byte dummy segment at +15176 (end +15176 + 4,800, soft end 160 before it) so writers keep writing somewhere, and it returns 0 (Swap's wait still runs on the fail path). The size asked is (r4 + 14) × 4 bytes (0 when r4 is 0, then `sub_8229E100`'s default); with a recording object (+13160, its +172 set; never in play) at least the segment size. The new segment's end +44 leaves 14 words for the tail. The throttle's wait is code 3. Seen callers: `sub_8229EDA8`, `sub_8229F148`; `Device::Swap`'s override calls it through `ctx` (`src/hooks/graphics_device.cpp`). Kept-kind with the segments; its fence wait Dropped-kind. |
| `sub_8229E5D0` | `Device::WriteFlushPackets` (probably) | | Read 2026-11-06 (B7 loop). r3 the device, r4 and r5 out: the block's physical address and its word count (0: nothing). Writes an 11-word coherency block for each of: the pending CPU-written range at +11560 (taken and emptied atomically by `sub_8229DE48`; widened by `sub_8229DDD8`), rounded out to 4 KB, and with +10810 bit 3 (the buffer wrapped, set by `sub_8229E100`; not cleared here) the whole buffer (+13504 to +13508): `COHER_STATUS_HOST` (`0xA31`) = `0x03000000`, `COHER_SIZE_HOST` and `COHER_BASE_HOST` (`0xA2F`, `0xA30`) the range, and a WAIT_REG_MEM (`0xC0043C00`) until `COHER_STATUS_HOST`'s bit 31 clears. Reserved with `sub_8229E308` (32-byte alignment). From `sub_8229EF48` and `sub_822B6BF8`. Dropped-kind: coherency packets, which the renderer skips (the user's call on cache flushes, 2026-10-24, `graphics-resources.md`); the range has no writer in play since `Resource::Lock` is native. |
| `sub_8229DE48` | | | Read 2026-11-06 (B7 loop). Takes the 64-bit pending flush range at r3 (device +11560) and leaves it empty (`0x00000000FFFFFFFF`), by a reserved load and store loop: r4 gets its low word (the start, +11564), r5 its high word (the end, +11560). Leaf; only from `sub_8229E5D0`. Dropped-kind with it. Ready group (a leaf). |
| `sub_8229DDD8` | | | Widens the device's pending cache flush range (r3 = device +11560: a 64-bit start and end, the start the smaller, the end the larger) to r4-r5, by a compare-and-swap. From `Resource::Lock` off the device's thread (always in play), whose native code (`graphics-resources.md`) leaves the call out (Dropped, the user's call: the renderer skips coherency packets). Read 2026-11-06 (B7 loop): the 64-bit word is +11560 the end and +11564 the start (empty: 0 and `FFFFFFFF`); `sub_8229E5D0` takes it (`sub_8229DE48`) and writes the coherency packets for it. With `Resource::Lock` native, nothing writes the range in play. Ready group (a leaf). |
| `sub_822AC470` | | | The data cache flush over r3 to r4 (128-byte lines, eight a step; returns at once for r3 from `0x7F100000` to `0x86FFFFFF`); its translation writes only its own stack and leaves r3. From `Device::SetUpRing` when a command buffer was already set up (r3 and r4 its first and last physical addresses minus `0x40000000`) and `Resource::Unlock`, whose native code (`graphics-resources.md`) leaves the call out (Dropped, the user's call: no data cache to flush on PC). Since 2026-11-06 also from `sub_8229EB10` (seen: each new tiling chain block, its physical address minus `0x40000000`, so the `0xC0000000` view, flushed) and `sub_822AEBF0`; 14 callers, 2 seen in play. Dropped-kind (no data cache to flush on PC, the user's call). Ready group (a leaf). |
| `sub_8229E908` | | | Waits for room in the command buffer, comparing progress block +0 with the command position (+10780) (see "Plan for milestone 6"). From `sub_8229EC00`. Not read in detail. Also a fence wait: r3 the device, r4 a fence (0: none), r5 a code, r6 a resource; passes at once when the fence is at or before the one the GPU is done with (device +10768's word, against +10780), else may end the segment (`sub_8229EF48`) and wait. From `Resource::Lock` and `Resource::Destroy`, whose native code (`graphics-resources.md`) leaves the call out (Dropped, the user's call). Read 2026-11-06 (B7 loop): r5 the wait's code, kept by `sub_8229D6F0` and used by `sub_8229D720` for the waited time's total (+20056 for 3, else +20048): 0 from `sub_8229EF48`, 3 `sub_8229EC00` (the frame throttle), 4 `sub_8229F1B0`, 6 `sub_8229EAF0`; r6 isn't read. The fence is done when +10780 − fence is below +10780 − the progress block's +0 (fence 0: no wait). A wait on the open segment's fence (the fence is +10780) ends the segment first, outside tiling. Seen callers: `sub_8229EC00`, `sub_8229EF48`, `sub_8229F1B0`. Passes at once in play (its wait helpers never ran): our kick applies the segments' fence writes as it kicks. Dropped-kind (a fence wait; the user's call, 2026-10-24). |
| `sub_8229F1B0` | | | The full GPU wait (spins on device +10868; see "Plan for milestone 6" in `docs/native-renderer.md`); passes at once in exclusive mode. From `Device::SetUpRing`, `Device::Release`, `Device::SetClockGating`, `RetrainEdram`. Not read in detail. Read 2026-11-06 (B7 loop): ends the segment when past the soft end, writes a WAIT_UNTIL (register `0x5C8` = `0x20000`), waits for the open fence (+10780, code 4, `sub_8229E908`, which ends the segment and so kicks the frame's last packets), then spins until the pending kick count +10868 is 0 (always 0 in play: the deferred kick's override never raises it). Seen in play from `sub_82091BC0` (once a frame, before the present); `Device::SetUpRing`'s override calls it through `ctx`. Dropped-kind (a GPU idle wait), but its segment end is what kicks the frame's last fences and callbacks while those stay. |
| `sub_8229F0E0` | | | Writes a WAIT_UNTIL (register `0x5C8` = `0x20000`) and register `0xD04` = 0 into the command buffer, ending the segment first if past +48 (`sub_8229EF48`). Only from `Device::SetUpRing`. Read 2026-11-06 (B7 loop): as above (r3 the device; the segment ended through `sub_8229EF48` when past the soft end +48; the new write position stored at +40). Called through `ctx` by `Device::SetUpRing`'s override (`src/hooks/graphics_device.cpp`). Dropped-kind (packets nothing reads). |
| `sub_8229E9D0` | | | From `sub_822A0718` (r4 the write position, r6 `822A0688`, r7 from +13240 and +11596); writes packets. Not read. Read 2026-11-06 (B7 loop): writes a **callback** into the command stream at r4 (r3 the device, r5 the flags with the CPUs in bits 24-29 (`0x3F000000`; 0 means 4, CPU 2), r6 a function, r7 its argument; returns the new position). Unless r5 has bits outside the CPUs, a WAIT_UNTIL first (`0x5C8` = `0x20000`); then `CALLBACK_ADDRESS` and `CALLBACK_CONTEXT` (`0x57C`, `0x57D`) = r6, r7; `SCRATCH_REG0` (`0x578`) = the CPUs; a WAIT_REG_MEM (`0xC0043C01`) on the callback block (+10772); an INTERRUPT (`0xC0005401`) for the CPUs; `CALLBACK_ACK` (`0x45E`) = the CPUs; a second WAIT_REG_MEM on the callback block for 0 (the callbacks done); `CALLBACK_ADDRESS` = `0x0BADF00D`. Leaf. Seen callers: `sub_8229F228`, `sub_822A0718` (the present callback `822A0688`), `sub_822B6BF8` (the tiling worker's wake `822B6A78`). Our kick runs the INTERRUPT as calls to the callback block's functions for those CPUs (`Interrupt` in `src/native/gpu_link.cpp`). Kept-kind while callbacks go through the stream (a native library would call them directly). Ready group (a leaf). |
| `sub_8229F228` | `Device::InsertCallback` (probably) | | Read 2026-11-06 (B7 loop). r3 the device, r4 the flags (to `sub_8229E9D0`'s r5: the CPUs in bits 24-29), r5 a function, r6 its argument: ends the segment when past the soft end (`sub_8229EF48`), writes the callback's packets (`sub_8229E9D0`) at the write position and stores the new position. Seen from `Device::EndTiling` (`sub_825BF200`, which callback not read); also the movie player's `sub_824842C8` (never seen; the movie library is replaced). Kept-kind while callbacks go through the stream (the kick runs them). |
| `sub_8229DBF0` | `Device::SaveSegment` (probably) | Overridden | Read 2026-11-06 (B7 loop). r3 the device: copies the write position +40, the end +44, the segment's start +13528, the wrap cursor +13524, the data cursor +13516 and the generation +13520 to +13056..+13076 (`sub_8229E0C0` puts them back). Leaf; only from the tiling's start `sub_822B6AD8` (a lead). Kept-kind with predicated tiling. Ready group (a leaf). Plain C++ in `src/graphics/command_buffer.cpp` since 2026-11-06 (`SaveSegment`) behind an adapter (`src/hooks/command_buffer.cpp`, `--native_command_buffer`); harness `command_buffer`. Not run by the game in a 40 s attract race. |
| `sub_8229E0C0` | `Device::RestoreSegment` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 the device; puts back what `sub_8229DBF0` saved at +13056..+13076 (the soft end +48 = the end − 160). Only from the tiling's end `sub_822B6BF8`, on a path not taken in play. |
| `sub_8229E098` | `Device::SetOutOfSpace` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 the device; sets +10809 bit 6 and points the write position at the dummy segment +15176 (end + 4,800, soft end 160 before it), as `sub_8229EC00` does when a segment can't be had. From `sub_8229DB38` (`functions.md`) and `sub_822AD4A0` on paths not taken in play. |
| `sub_8229ED60` | `CommandChain::Init` (probably) | | Read 2026-11-06 (B7 loop). r3 a chain (20 bytes: +0 its first block, +4 the current block's first word, +8 the write cursor, +12 the block's end, +16 the device), r4 the device: stores the device, clears +4, +8 and +12, starts the first block (`sub_8229EB10`) and keeps it at +0. Only from the tiling's start `sub_822B6AD8` (a lead), for the chains at device +13016 (the segments), +12976 (the tiled draws, 16-byte entries from `BeginVertices`) and +12996. Kept-kind with predicated tiling (the tiling worker reads the chains). |
| `sub_8229EB10` | `CommandChain::NewBlock` (probably) | | Read 2026-11-06 (B7 loop). r3 a chain: reserves a 4,224-byte block in the command buffer (`sub_8229E308`, 128-byte aligned). A block's first word holds where its content ends and the word after the content the next block's address, both as `0xC0000000` + the physical address: the old block's first word (+4, when set) gets the cursor's, the old cursor (+8, when set) the new block's; the new block's first word its own start + 4 for now. Flushes the block from the data cache (`sub_822AC470`) and sets +4 = the block, +8 = block + 4, +12 = block + 4,220; returns the block. When the reservation fails (+10809 bit 6) the block is the dummy segment at device +15176 and the chain's +0 is cleared. 9 callers, 7 seen: the chains' writers when a block is full: `BeginVertices` and `DrawIndexedVertices` (`sub_825BE708`) under predicated tiling (+12976), `sub_825BED08`, `Device::EndTiling`, `sub_822B6AD8`, `sub_8229ED60` and `sub_8229EDA8` (+13016). Kept-kind with predicated tiling. |
| `sub_8229DFB8` | `CommandChain::Close` (probably) | Overridden | Read 2026-11-06 (B7 loop). r3 a chain: the current block's first word (+4) gets the cursor's address (`0xC0000000` + its physical address), the cursor the end marker `0xC0000000`, and +12 is cleared (the next write starts a block). Leaf; from the tiling's end `sub_822B6BF8`. Kept-kind with predicated tiling. Ready group (a leaf). Plain C++ in `src/graphics/command_buffer.cpp` since 2026-11-06 (`CloseChain`) behind an adapter (`src/hooks/command_buffer.cpp`, `--native_command_buffer`); harness `command_buffer` (the block and the cursor the same word now and then). Not run by the game in a 40 s attract race. |
| `sub_822AF978` | `Device::WriteDefaultDrawRegisters` (probably) | | Read 2026-11-06 (B7 loop). r3 the device, r4 the write position (pre-increment); writes 15 words and returns the new position: `VGT_MAX_VTX_INDX`, `VGT_MIN_VTX_INDX`, `VGT_INDX_OFFSET` (`0x2100`-`0x2102`) = `0xFFFF`, 0, 0; `PA_CL_CLIP_CNTL`, `PA_SU_SC_MODE_CNTL`, `PA_CL_VTE_CNTL` (`0x2204`-`0x2206`) = `0x10000`, `0x10000`, `0x300`; `PA_SC_AA_MASK` (`0x2312`) = `0xFFFF`; `0x200D` = 0; a type-3 `0x60` packet (SET_BIN_MASK_LO, probably) with device +12448. Then marks what it overwrote dirty so the next draw writes it again from the shadow: +24 `0x400000`, `0x200000`, `0x2000000`; +16 `0x80`, `0x40`, `0x20`; +32 `0x80000`. Leaf. 4 callers, 2 seen in play: `Device::Resolve` (`sub_822AFA58`, Wrapped) and `sub_8229FC40` (`Device::Clear`'s worker, a lead); `sub_8229F680` (`device-state.md`: its native code leaves the call out and makes the same marks, `src/graphics/device_state.cpp`) and `sub_822B5208` not seen. Dropped-kind for its packets (the renderer reads the shadow, not packets); kept-kind for its dirty marks (the renderer reads the shadow by them). Ready group (a leaf). |
| `sub_8229EAF0` | `Device::BlockOnFence` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 a fence; the fence wait (`sub_8229E908`, code 6) on the device in `VdGlobalDevice` (`[0x820007B0]`). 5 callers, none seen. Dropped-kind (a fence wait). |
| `sub_8229F030` | `Device::InsertFence` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 the device; +10796 = the open fence (+10780), the segment ended (`sub_8229EF48`, whose tail writes it), returns the fence. Only from `sub_822B5208` (never seen). |
| `sub_8229F068` | `Device::IsFencePending` (probably) | | Not seen in play. Read 2026-11-06 (B7 loop): r3 a fence (0: returns 0); on the device in `VdGlobalDevice`, ends the segment when it's the open fence, then returns 1 while the GPU hasn't reached it (as `sub_8229E908` compares). Only from `sub_8229BFD0` (never seen). |
| `sub_8229D6F0` | | | Not seen in play. Read 2026-11-06 (B7 loop): starts a wait (r3 a 24-byte record on the caller's stack, r4 the device, r5 the wait's code): +0 the device, +4 the code, +8 the progress block's fence then, +12 and +16 the thread's tick count (`[r13 + 256]` + 88), +20 the timebase. From the waits `sub_8229DC28`, `sub_8229E908`, `sub_8229DCC0` and `sub_822B64A0`, which in play never had to wait. |
| `sub_8229D8A0` | | | Not seen in play. One step of a busy wait (`../gpu-functions.md`, "Waits on the GPU": a few `db16cyc`, the tick count, giving up after 5,000 ticks through `sub_822ACAA8`); r3 the wait's record, returns 0 to stop. Not read in detail here. From the same four waits. |
| `sub_8229D720` | | | Not seen in play. Read 2026-11-06 (B7 loop): ends a wait (r3 its record): with a code, adds the timebase ticks waited to the device's +20056 (code 3) or +20048 (others), and when the device has a wait callback at +13084 calls it (r3 0, r4 the code, f1 the time: the ticks × +20044 × the float at `0x82001BDC`). From the same four waits. |
| `sub_8229DCC0` | | | Waits for room in the primary ring, reading its read pointer from progress block +60 (see "Plan for milestone 6" in `docs/native-renderer.md`). Not read in detail. |
| `sub_8229DD70` | | | Writes r5 words from r4 into the primary ring at its write position (+10820, wrapping with +13500), after waiting for room (`sub_8229DCC0`); doesn't write CP_RB_WPTR. From `Device::SetUpRing` (ME_INIT, no longer) and `sub_822B6370` (not read). |
| `sub_8229E3F0` | `Device::Kick` | Overridden | `src/native/gpu_link.cpp`. r3 device, r4 (size in words, address) pairs, r5 their count. The kicked buffers are applied directly (only their effects outside the GPU) instead of written to the ring; the original is no longer called. `--native_renderer_log_gpu` logs. Since 2026-11-06 in `command-buffer.md`: its only caller in play is `Device::DeferredKick`'s override; the kicked segments' fences, positions and callbacks are what the waits and callbacks here read. |
| `sub_8229E848` | `Device::DeferredKick` | Overridden | Same file. Queued kicks while GPU interrupts were outstanding; every kick completes at once now, so it kicks straight away; the original is no longer called. From `sub_8229EDA8` (each segment, r8 the list +13016) and `sub_8229EF48` (the flush block). |
| `sub_8229DFF8` | | | Links a block into a list in the command buffer: r3 the list (+4, +8 its last), r4 the block, r5 and r6 stored in it; stores the block's address through the old last's pointer (the MMIO store macro, but not shown to be a register) and calls `sub_822AC470` twice. Only from the deferred kick `sub_8229E848`, whose native version doesn't call it: never called (attract race and play). Not read in detail. |
| `sub_8229DEA0` | `Device::OnInterrupt` | | The library's interrupt callback (r3 source: 0 vblank, 1 the command stream's INTERRUPT; r4 device): on a vblank counts +15148, counts down +15156 (zeroing callback block +4 at 0) and calls the callback at +15144; on an INTERRUPT calls callback block +16 with +20 and clears its CPU's bit. Reads interrupt status `7FC86544`. Never called (attract race and play, 2026-10-20, a counter): only `sub_822AD190` registered it (left out by native `Device::Init`), our kick's interrupts call the callback block's functions themselves, and there are no vblanks without the GPU plugin. Its override (logging only) was removed. |
