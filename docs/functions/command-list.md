# Known functions: command list

The graphics library's command list: the double-buffered memory the main
thread records render commands into (16-byte slots, a header `[id, arg1,
arg2, 0]` and its payload) and the render thread replays a tick later.
Recorders open a chunk, allocate slots in it (headers and payloads: shader
constants, inline vertices, padded bytes, resolves), close it, and hand
the closed chunk to the render queue. The format and the replay's command
table are in `../gpu-functions.md` ("Command format") and "Render command
recording" in `../frame-interpolation.md`; the list's fields and globals
are in `../known-data.md`; why AllocateSlots is native is "AllocateSlots"
in `../native-renderer.md`. Mapped by the B7 loop
(`../loops/b7-loop.md`) from its lead in `../loops/leads.md`.

The list the game records into is made once at boot by the scene's
constructor `sub_82098A98` (scene + 8, its pointer kept at `0x8296B918`
and `0x82970BD8`), and flipped each tick by `Scene::Update`
(`sub_820988C8`), both in [`functions.md`](functions.md). The ~40
recorders that call it (seen in play: `sub_82096030`, `sub_82096360`,
`sub_82096418`, `sub_8209AE48`, `sub_8209AF40`, `sub_8209B3D0`,
`sub_8209FA38`, `sub_820A0978`, `sub_820A0CC0`, `sub_820A0D38`,
`sub_820A1C30`, `sub_820A1CA8`, `sub_820AE688`, `sub_820C5DE8`,
`sub_820CECA0`, `sub_820D04E8`, `sub_820F34C8`, `sub_82109EF0`,
`sub_82130978`, `sub_82133340`, `sub_82133608`, `sub_82133790`,
`sub_82133D00`, `sub_82133D70`, `sub_82133F58`, `sub_82134060`,
`sub_821351F0`, `sub_82135B50`, `sub_821363F0`, `sub_82136FA8`,
`sub_821379D8`, `sub_82146BD0`, `sub_82147D88`, `sub_8217A370`,
`sub_8217AA98`, `sub_8217AEA8`, `sub_8217B508`, `sub_8217C370`,
`sub_82589990`, `sub_825953D0`, `sub_82595490`, `sub_82595550`,
`sub_825960D0`, `sub_82596B18`, `sub_82596D48`, `sub_8259D7B0`,
`sub_8259DAD0`, `sub_8259E098`; and in
[`sprites.md`](sprites.md) `sub_821281A8`, `sub_82128750`,
`sub_82128E70`, `sub_8209C1A0`, `sub_8209C6E0`, `sub_8209CC38`) belong to
their own subsystems and stay out of this file. Not in play, so no rows:
`sub_8258CCC8` (the destructor: vtable `0x8206C080` back, frees +20 with
`sub_825A71C0`, clears +24 and +28; its caller `sub_82098860` never ran)
and `sub_8258CE00` (the same, then `sub_825A70E8`, probably operator
delete, when bit 0 of r4 is set; no direct callers); the debug print
`sub_825A1318` the overflow and nesting checks call never ran either.
Shared callees in no file yet: `sub_825A6940` (probably `malloc` or
operator new) and `sub_825A71C0` (probably `free`), the C runtime heap
(a lead in `../loops/leads.md`). Nearby but not the command list:
`sub_8258C938` (vtable `0x8206C05C`, +4 = -1, +8 = +12 = 0: probably the
file class's constructor, `files-and-saves.md`'s area; from
`sub_82593AC0`) and `sub_8258CE60` (an input processor's constructor,
vtable `0x8206C0C4`). The rest of the game's functions are in
[`functions.md`](functions.md).

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
| `sub_8258C960` | `CommandList::CommandList` | | r3 = list: vtable `0x8206C080` at +0, zeroes +4 (the buffer being recorded), +8, +12, +20, +24, +28, +32 and the open flag (byte +52). Once at boot, from the scene's constructor `sub_82098A98` (on scene + 8). |
| `sub_8258CD28` | `CommandList::Init` | | r3 = list, r4 = slots in all (the scene asks for 512,000: 8 MB): frees the old memory at +20 (`sub_825A71C0`), rounds r4 up to even, allocates r4 × 16 bytes (`sub_825A6940`; -1 asked for above `0x0FFFFFFF` slots) into +20, +8 = half of r4 (each buffer's capacity in slots), buffer 0 at +24 (the allocation) and buffer 1 at +28 (its second half); flips +4 twice (left as it was), +32 = the buffer +4 selects, +12 = +16 = 0. Returns 1. Once at boot, from `sub_82098A98`. Blocked by the C runtime heap (`malloc` and `free`): made native, it would allocate guest memory without `ctx`, a decision for the user (see the heap's lead). |
| `sub_8258C9C0` | `CommandList::Flip` | | r3 = list: +4 ^= 1, so the other buffer is recorded while the render thread replays the one just recorded. Once a tick, from `Scene::Update` (`sub_820988C8`), before `sub_8258C998`. |
| `sub_8258C998` | `CommandList::BeginTick` | | r3 = list: +32 = the buffer +4 selects (+24 or +28), +16 = +12 (the slots the last recording used), +12 = 0. Doesn't move the write pointer (+44): opening a chunk does. Once a tick, from `Scene::Update` (`sub_820988C8`). |
| `sub_8258C9D0` | `CommandList::OpenChunk` | | r3 = list. Nothing when a chunk is open (byte +52); otherwise +36 (the chunk's start), +44 (the write pointer), +48 and +40 = +32 (where the last chunk ended), +52 = 1, and the count of open chunks at `0x8296BD28` goes up by 1, with a debug message (`sub_825A1318`, the string at `0x8206C084`; never ran) when one was already open (probably a nesting warning). From every recorder (40 seen in play; see above), once per chunk. |
| `sub_8258CA50` | `CommandList::CloseChunk` | | r3 = list. Nothing when no chunk is open; otherwise +52 = 0, +40 (the chunk's end) and +32 (where the next chunk starts) = +44, and the count at `0x8296BD28` down by 1. Its callers then hand the chunk (+36 to +40) to the render queue (probably; e.g. `sub_820AE688` with `sub_8258CA88`'s count). Same callers as OpenChunk. |
| `sub_8258CA88` | `CommandList::ChunkSlots` | | r3 = list: (+40 - +36) >> 4, the closed chunk's size in slots. From `sub_820AE688`, `sub_8217B508` and `sub_8217C370` (seen in play). |
| `sub_8258CAA0` | `CommandList::AllocateSlots` | Overridden | r3 = list, r4 = slot count; returns the first 16-byte slot (+44, which moves on by r4 × 16; +12 counts the slots). A full list (+12 + r4 not below the capacity +8) gets a debug message (`sub_825A1318`, the string at `0x8206C0A8`) and 0; never seen. ~51,000 calls a frame (most from `sub_82146BD0`, one slot each; "AllocateSlots" in `../native-renderer.md`); 58 callers seen in play, including `sub_8258CB08`, `sub_8258CB98` and `sub_8258CC30`. Ours (`src/interpolation/blend_state.cpp`, `--native_allocate_slots`): allocated natively, leaving `r3`, `r9`-`r12` and `cr6` as the original does; the original only for a full list or with the flag off; its diagnostics (`--log_slot_callers`, `--log_overlay_uploads`, `--log_recording_by_pass`) out of line. (Was "Wrapped only for diagnostics", from before `allocate-slots`.) |
| `sub_8258CB08` | `CommandList::AllocateVertices` | | r3 = list, r4 = vertex count, r5 (byte) = with a header: when set, a slot (AllocateSlots) with command 5 (inline vertices for the helper object), arg1 = r4. Then (r4 × 24 + 15) >> 4 slots (24-byte vertices), allocated inline as AllocateSlots does (the same overflow message and 0); returns them. From `sub_821281A8`, `sub_82128750`, `sub_82128E70`, `sub_8217A370`, `sub_8217AA98`, `sub_8217B508`, `sub_8217C370`, `sub_8259DAD0`, `sub_8259E098` (seen in play). |
| `sub_8258CB98` | `CommandList::AllocateBytes` | | r3 = list, r4 = bytes, r5 (byte) = with a header: when set, a slot with command 21 (skipped payload), arg1 = the slot count ((r4 + 15) >> 4). Then that many slots, as AllocateVertices; returns them. Command 21's handler (`sub_8258EAD0`) reads arg1 as bytes, not slots: whether any caller asks for the header wasn't checked (the call sites looked at pass r5 = 0). From 14 recorders, all seen in play (`sub_82096418`, `sub_8209B3D0`, `sub_8209C1A0`, `sub_8209FA38`, `sub_820A0CC0`, `sub_820A0D38`, `sub_820A1C30`, `sub_820F34C8`, `sub_82109EF0`, `sub_821281A8`, `sub_82128750`, `sub_82133D70`, `sub_821363F0`, `sub_82136FA8`). |
| `sub_8258CC30` | `CommandList::AllocateResolve` | | r3 = list, r4 (byte) = with a header: when set, a slot with command 25 (Resolve), arg1 = 5. Then 5 slots (80 bytes, for the 68-byte resolve item), as AllocateVertices; returns them. From `sub_8209B3D0`, `sub_820A1C30`, `sub_820A1CA8`, `sub_82130978`, `sub_82133F58` (seen in play). |
