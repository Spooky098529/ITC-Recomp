# Known functions: the render command replay's single commands

The render queue's **single commands**: the second table of render
command handlers, at `0x8265ED48` (37 words for the ids 0 to 36, then a
0 word before the command table `0x8265EDE0`; read from the image,
`itc/xex_image/default.bin`). `ChunkChain::RunSingles` (`sub_8258E630`,
the chunk walker's kind 0, [`device-shaders.md`](device-shaders.md),
native) walks a chunk's single-command item: +8 the slot count, then
16-byte slots from the item's +16, each `[id, arguments]`; the handler
of the slot's id is called with r3 = the slot and returns how many of
the following slots it used (its data), which the walker skips. The ids
are the command table's (`../gpu-functions.md`, "Command table" and
"Single commands"), but each handler reads its own slot layout: the
state and binding commands (1-4, 8-12, 20, 22-24) carry a count at +4
and do that many calls, one 16-byte entry each in the slots after
(1-4, 8-12 and 20 read from an entry's +0, 22-24 from its +4),
returning the count; the helper object's commands (5, 6, 13-16, `0x82970BB8`, the
primitive drawer, with its state at `0x82970BBC`, both published by
`RenderQueue::Replay`) and the draws (17-19) read the slot; 21 and
25-27 return the slot's +4 (their data's slot count); 0 and 29-36 are
the shared `sub_825C8388` (returns 0, [`vehicle-accessors.md`](vehicle-accessors.md)),
so a slot with one of those ids is skipped alone. Every handler finds
the device at `[0x8296BD90]`. On the render thread, under
`RenderQueue::Replay` (`sub_82591238`, once a frame from `Scene::Draw`).
Mapped by the B7 loop (`../loops/b7-loop.md`) from its lead in
`../loops/leads.md` (the render queue's methods, found by the B7 loop's
mapping agent, 2026-11-01), 2026-11-06; one row moved from
`functions.md` (`sub_8258F5C0`).

**Seen in play** (`itc_recomp/coverage/seen_in_play.txt`): 5, 7, 13
(and 14, the same function), 15 and 28. Their recorders: the minimap's
player triangle (`Triangle::RecordFlat`, [`minimap.md`](minimap.md):
`Primitive::RecordTransform`'s command 7 with the vertex constants c0 to
c3, then `[13, 4, 0, 0]`, three 24-byte vertices behind a command 5
header (`CommandList::AllocateVertices(3, 1)`,
[`command-list.md`](command-list.md)) and a slot with 15), counted a few
times a frame in a race (`../gpu-functions.md`), and the movie
(`Movie::OnTick` queues command 28 with itself at +4,
[`movies.md`](movies.md)), on the boot movies. The others never ran.

**What our renderer reads.** The single commands aren't wrapped by
`REPLAY_COUNTED_HANDLER` (`src/native/replay_hooks.cpp`): the stats
logger (`--log_replay_commands`) and `Renderer_OnCommand` and
`AfterCommand` don't see them, and frame interpolation's wrap of command
7 (`sub_8258FD48`, `src/interpolation/blend_state.cpp`) is the command
table's handler, not single 7's (the triangle's product is noted for
interpolation where `Primitive::RecordTransform` makes it). What the
renderer reads from each, by the graphics rule ("native if the renderer
needs it, else Dropped", `../loops/b7-loop.md`'s "Precedent first"):

- 5 stores the vertices' address and count in the drawer's state,
  which 15 draws from: kept.
- 7 writes the device's shader constants (device +1792 on for the
  vertex shader's float constants, `Device::SetVertexShaderConstantF`,
  [`device-state.md`](device-state.md)); the renderer reads them at its
  draws (`src/native/state.h`): kept.
- 13 stores the primitive type and the shader pair's indices and sets
  the drawer's shaders (`PrimitiveDrawer::SetShaders`,
  [`model-animation.md`](model-animation.md): the device's +12436 and
  +12432, which the renderer reads): kept.
- 15 is Wrapped: `native::g_current_command` set to 15 around it, so the
  wraps of `BeginVertices` and `EndVertices` (`sub_825BD8F0`,
  `sub_825BDDA0`, `functions.md`) hand the inline draw `PrimitiveDrawer::Draw`
  makes to `Renderer_InlineDraw` (the vertices copied into the command
  buffer's data space, `command-buffer.md`); the viewport transform
  switched around it (PA_CL_VTE_CNTL's shadow, device +10444) and the
  stream 0 and texture 0 unset after it are read by the renderer: kept.
- 28 calls the object's vtable slot 0 with the device: in play
  `Movie::RenderFrame` (`sub_820AE958`, overridden in
  `src/hooks/movies.cpp`: our renderer draws the movie's frame there,
  `Passes::DrawMovie`, `../native-movies.md`): kept.
- Not seen in play, by what they'd do: 1-4, 6, 8-12, 16-20 and 23-27 are
  the same library calls as their command table versions, which the
  renderer reads (states, binds, draws, Resolve, Clear), so kept-kind;
  17-19 and 25-27 would be logged by the "outside" wraps
  (`LOG_OUTSIDE_WRAPPER`, `Device::Resolve`'s), and 19's inline draw
  drawn only under `g_current_command` 19, which a single 19 doesn't
  set. 21 skips data; 22 the release list's push (as command 22's
  `sub_82591798`).

Shared, in other files: `ChunkChain::RunSingles` and `PtrVector::PushBack`
(`device-shaders.md`); the device's setters (the render and sampler state setters reached
through the device's tables at +56 and +440, `SetBlendControl`, `SetTexture`, `SetStreamSource`, `SetIndices`,
`SetRenderTarget_External`, `SetDepthStencilSurface`, the shader
constant setters, `GetRenderState_ViewportEnable`,
`SetRenderState_ViewportEnable`: `device-state.md`; `D3DDevice_SetVertexShader`,
`Device::SetPixelShader`, `Device::SetVertexDeclaration`:
`device-shaders.md`); `PrimitiveDrawer::SetShaders` and `Draw` (vtable
`0x820721D4` slots 2 and 3, recompiled, `model-animation.md`);
`Resource::Release` (`graphics-resources.md`); `memcpy` (`c-runtime.md`);
`BeginVertices`, `EndVertices`, `BeginIndexedVertices`, `DrawVertices`,
`DrawIndexedVertices`, `Device::Resolve`, `Device::Clear` and
`OutputDebugStringA` (`functions.md`); `Movie::RenderFrame`
(`movies.md`); `sub_825C8388` (`vehicle-accessors.md`). Not seen in
play, so no rows: the shader constant setters for the vertex shader's
bool constants and both shaders' loop constants (`sub_822A8418`,
`sub_822A84C8`, `sub_822A8518`, as in `device-state.md`) and the
drawer's indexed draw (slot 4, `sub_825A1810`). The drawer's state at
`0x82970BBC`: `../known-data.md`. The rest of the game's functions are
in [`functions.md`](functions.md).

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

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_8258E8E8` | `Single::InlineVertices` (5) | Overridden | Single command 5. r3 = the slot: the drawer's state (`[0x82970BBC]`) +12 = the slot + 16 (the vertices, 24 bytes each, in the slots after) and +16 = the slot's +4 (the vertex count). Returns (count × 24 + 15) >> 4, the slots the vertices take. Leaf, 15 lines. Seen in play: the minimap's player triangle (3 vertices, `Triangle::RecordFlat`, `minimap.md`), a few times a frame. Kept-kind (command 15 draws them). Read in full (B7 loop, 2026-11-06). Ours (B7 loop, 2026-11-06): `graphics::single_commands::InlineVertices` (`src/graphics/single_commands.cpp`), adapter in `src/hooks/single_commands.cpp` (`--native_single_commands`; it stays: reached through the table); `ChunkChain::RunSingles` calls the plain C++ directly (`hooks::SingleCommands`). Harness `single_commands`: 64 cases (the triangle's 3, edge and random counts), 0 differ. A frame dump 10 s into an attract race drew the same draws, shaders and states as with `--native_single_commands=false` (addresses aside). |
| `sub_8258E918` | `Single::InlineIndices` (6) | | Single command 6. r3 = the slot: the drawer's state +20 = the slot + 16 (the indices), +24 = the slot's +4 (the count), +28 = the slot's +8 (32-bit indices when not 0). Returns (count × (4 when +8 isn't 0, else 2) + 15) >> 4. Leaf. Not seen in play (it feeds 16). Read in full (B7 loop, 2026-11-06). |
| `sub_8258EF60` | `Single::SetRenderStates` (1) | | Single command 1. r3 = the slot, +4 the count: for each 16-byte entry from the slot + 16, the device's render state setter at device + 56 + the entry's +0 (the state, a byte offset into the table) called with r3 = the device and r4 = the entry's +4 (the value). Returns the count. A 128-byte frame (r28-r31 saved). Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258EFC0` | `Single::SetSamplerStates` (2) | | Single command 2. r3 = the slot, +4 the count: for each entry from the slot + 16, the sampler state setter at device + 440 + the entry's +4 (the state) called with r4 = the entry's +0 (the sampler) and r5 = its +8 (the value). Returns the count. Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F028` | `Single::SetBlendControls` (3) | | Single command 3. r3 = the slot, +4 the count: for each entry from the slot + 16, `Device::SetBlendControl` (`sub_822A3B30`, `device-state.md`) with r4 = the entry's +0 (the render target) and r5 = its +4 (the blend control). Returns the count. Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F080` | `Single::SetTextures` (4) | | Single command 4. r3 = the slot, +4 the count: for each entry from the slot + 16, `Device::SetTexture` (`sub_822A6008`, `device-state.md`) with r4 = the entry's +0 (the stage), r5 = its +4 (the texture) and r6 = the dirty mask 1 << (43 − stage) (bit 63 shifted right by stage + 20). Returns the count. A 128-byte frame (r27-r31 saved). Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F0F0` | `Single::SetShaderConstants` (7) | | Single command 7. r3 = the slot: +4 the shader (0 the vertex shader, 1 the pixel shader; any other: returns 0), +8 the type (0-4, through jump tables in the code at `0x8258F138` and `0x8258F294`; above 4 nothing), byte +12 the first register, byte +13 the count, the data from the slot + 16. Type 0: `sub_822A84C8` (vertex) or `sub_822A8518` (pixel), the loop constants (r4 the first, r5 the data, r6 the count; not seen in play, no rows); type 1: `Device::SetVertexShaderConstantF` (`sub_822A81F8`) or `SetPixelShaderConstantF` (`sub_822A8308`, `device-state.md`) with r7 = the dirty mask of the 4-register blocks it covers (bit 63 shifted right arithmetically by (first + count − 1) / 4 − first / 4, then right logically by first / 4); type 2: the bool setters `sub_822A8418` (vertex, no row) or `Device::SetPixelShaderConstantB` (`sub_822A8470`, `device-state.md`) with the words in the data; type 3: `OutputDebugStringA("aaaa...\n")` (`sub_82296270`, the string at `0x8206CB64`, 32 a's), nothing set; type 4: the data's +0 a bit mask and +4 a count, expanded into a word a bit (bit i of the mask, 0 or 1) on the stack at r1 + 80, then the bool setters with r6 = that count. Returns byte +13 (the count). A 224-byte frame (r31 saved). Seen in play: the minimap's player triangle (`Primitive::RecordTransform`, `minimap.md`: shader 0, type 1, c0 to c3), a few times a frame; only the float setter's path. Kept-kind (the renderer reads the constants; not seen by interpolation's command 7 wrap). Named in `device-state.md`'s intro as having no row. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F3E0` | `Single::SetVertexShaders` (8) | | Single command 8. r3 = the slot, +4 the count: for each entry from the slot + 16, `D3DDevice_SetVertexShader` (`sub_822A8958`, `device-shaders.md`) with r4 = the entry's +0. Returns the count. Not seen in play (named in `device-shaders.md`'s intro as probably the single version of 8: it is). Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F438` | `Single::SetPixelShaders` (9) | | Single command 9: as 8, with `Device::SetPixelShader` (`sub_822A8618`, `device-shaders.md`). Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F490` | `Single::SetVertexDeclarations` (10) | | Single command 10: as 8, with `Device::SetVertexDeclaration` (`sub_822A8960`, `device-shaders.md`). Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F4E8` | `Single::SetStreamSources` (11) | | Single command 11. r3 = the slot, +4 the count: for each entry from the slot + 16, `Device::SetStreamSource` (`sub_822A3758`, `device-state.md`) with r4 = the entry's +0 (the stream), r5 = +4 (the buffer), r6 = +8 (the offset), r7 = +12 (the stride) and r8 = the dirty mask 1 << (43 − (95 − stream) / 3) (the division as × 21846 >> 16). Returns the count. A 128-byte frame (r27-r31 saved). Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F568` | `Single::SetIndices` (12) | | Single command 12: as 8, with `Device::SetIndices` (`sub_822A3908`, `device-state.md`). Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258E958` | `Single::SetUpDrawer` (13, 14) | Overridden | Single commands 13 and 14 (one function). r3 = the slot: the drawer's state (`[0x82970BBC]`) +0 = the slot's +4 (the primitive type), +4 = +8 (the vertex shader's index), +8 = +12 (the pixel shader's index); then the drawer's (`[0x82970BB8]`) vtable slot 2 (+8, `PrimitiveDrawer::SetShaders` `sub_825A16B0`, `model-animation.md`, recompiled) with r4 = the device, r5 = the vertex shader's index and r6 = the pixel shader's. Returns 0. A 96-byte frame, 31 lines, one indirect call. Seen in play: the minimap's player triangle (`[13, 4, 0, 0]`: a triangle list, shaders 0 and 0), a few times a frame; 14 never seen. Kept-kind (the shaders the renderer reads). Read in full (B7 loop, 2026-11-06). Ours (B7 loop, 2026-11-06): `graphics::single_commands::SetUpDrawer` (`src/graphics/single_commands.cpp`), adapter in `src/hooks/single_commands.cpp` (`--native_single_commands`; it stays: reached through the table): the drawer's slot 2 a guest call from its 96-byte frame; `ChunkChain::RunSingles` calls the plain C++ directly. Harness `single_commands`: 64 cases (the triangle's `[13, 4, 0, 0]`, then edge and random words; the drawer's slot a recorder of its arguments, `sub_825780D0`, or `RenderQueue::Flip`), 0 differ. Frame dump as command 5's. |
| `sub_8258F5C0` | `Single::DrawerDraw` (15) | Wrapped | Same file (`src/native/replay_hooks.cpp`): the single-command items' command 15 (the helper object's draw), marked as command 15 so the native renderer draws its inline vertices. Read in full (B7 loop, 2026-11-06): r31 = `0x82970BB8`; nothing when the drawer's state +16 (the vertex count) is 0. Else `Device::GetRenderState_ViewportEnable` (`sub_822A2470`) kept, `SetRenderState_ViewportEnable` (`sub_822A2430`) with 0 when the state's +4 (the vertex shader's index) is 1, else 1; the drawer's vtable slot 3 (+12, `PrimitiveDrawer::Draw` `sub_825A17A0`, `model-animation.md`, recompiled) with r4 = the device, r5 = the state's +0 (the primitive type), r6 = +12 (the vertices), r7 = +16 (the count); then `Device::SetStreamSource(device, 0, 0, 0, 0, 4096)` and `Device::SetTexture(device, 0, 0, 1 << 43)` (unset), and the viewport setting put back (`device-state.md`). Returns 0. A 112-byte frame (r30, r31 saved), one indirect call. Seen in play: the minimap's player triangle, a few times a frame. Kept-kind (the renderer draws it). |
| `sub_8258F6A0` | `Single::DrawerDrawIndexed` (16) | | Single command 16: as 15, with the drawer's vtable slot 4 (+16, `sub_825A1810`, never ran, no row) and r8 = the state's +20 (the indices), r9 = +24 (their count), r10 = +28 (32-bit). Returns 0. Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F770` | `Single::DrawVertices` (17) | | Single command 17. r3 = the slot: `DrawVertices` (`sub_825BE300`, `functions.md`) with r4 = +4 (the primitive type), r5 = +8 (the start) and r6 = the vertex count from the type's pair at `0x82000950` (8 bytes a type: a × the slot's +12, the primitive count, + b; points 1, 0, lines 2, 0, line strip 1, 1, triangle list 3, 0, fan 1, 2, strip 1, 2). Returns 0. Not seen in play. Kept-kind (would be logged as outside command 17 by `LOG_OUTSIDE_WRAPPER`). Read in full (B7 loop, 2026-11-06). |
| `sub_8258F7C8` | `Single::DrawIndexedVertices` (18) | | Single command 18. r3 = the slot: `DrawIndexedVertices` (`sub_825BE708`, `functions.md`) with r4 = +4 (the type), r5 = +8's high halfword (signed: the base vertex), r6 = its low halfword (the start index), r7 = the count from `0x82000950` as 17's (with +12). Returns 0. Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F828` | `Single::DrawPrimitiveUP` (19) | | Single command 19. r3 = the slot: `BeginVertices` (`sub_825BD8F0`) with r4 = +4 (the type), r5 = the halfword +8 (the vertex count), r6 = the halfword +10 (the stride); when it returns space, `memcpy` (`sub_825A7410`) of count × stride bytes from the address at +12, then `EndVertices` (`sub_825BDDA0`). Returns 0. A 112-byte frame. Not seen in play. Kept-kind (our wrap of `EndVertices` draws only under command 15 or 19, which a single 19 doesn't set: it would be logged, not drawn). Read in full (B7 loop, 2026-11-06). |
| `sub_8258F898` | `Single::ReleaseResources` (20) | | Single command 20. r3 = the slot, +4 the count: for each entry from the slot + 16, `Resource::Release` (`sub_822A67B0`, `graphics-resources.md`) on the entry's +0 when not 0. Returns the count. Not seen in play. Read in full (B7 loop, 2026-11-06). |
| `sub_820911F8` | | | Single command 21: returns r3's +4 (for the slot: its data's slot count, skipped). Leaf, 2 lines. Shared: also +16 of XAudio's source voice's vtable and the codecs' inner vtables (their reference count at +4, `xaudio.md`, `xaudio-objects.md`). Not seen in play. Read in full (B7 loop, 2026-11-06). |
| `sub_825916F0` | `Single::Release` (22) | | Single command 22 (the command table's `sub_82591798`, per entry). r3 = the slot, +4 the count: for each entry, its object at the slot + 20 + 16i: vtable slot 1 (+4), then slot 2 (+8); when that returns 0 in its low byte and the entry's flag (+24 + 16i) is 1, `PtrVector::PushBack([0x8296BD8C], the entry's address)` (the release list, `device-shaders.md`) and the object's slot 3 (+12). Returns the count. A 128-byte frame. Not seen in play. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F8F0` | `Single::SetRenderTargets` (23) | | Single command 23. r3 = the slot, +4 the count: for each entry, `Device::SetRenderTarget_External` (`sub_822A4340`, `device-state.md`) with r4 = the slot + 20 + 16i (the index) and r5 = +24 + 16i (the surface). Returns the count. Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F948` | `Single::SetDepthStencilSurfaces` (24) | | Single command 24. r3 = the slot, +4 the count: for each entry, `Device::SetDepthStencilSurface` (`sub_822A4060`, `device-state.md`) with r4 = the slot + 20 + 16i. Returns the count. Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258F9A0` | `Single::Resolve` (25) | | Single command 25. r3 = the slot; its +16 a word whose bits pick the optional arguments: `Device::Resolve` (`sub_822AFA58`, `functions.md`) with r4 = +20 (the flags), r5 = the slot + 24 (the source rectangle) with bit 0 else 0, r6 = +40 (the destination texture) with bit 1, r7 = the slot + 44 (the destination point) with bit 2, r8 = +52 (the level), r9 = +56 (the slice), r10 = the clear color with bit 3 (the bytes +60 to +63 as floats × 1/255, `[0x82002200]`, on the stack at r1 + 144), f1 = +64 (the clear Z), the stack's +92 = +68 (the stencil) and +100 = the slot + 72 with bit 4 (the parameters). Returns the slot's +4. A 208-byte frame. Not seen in play. Kept-kind (would be logged as a Resolve outside command 25). Read in full (B7 loop, 2026-11-06). |
| `sub_8258FAC8` | `Single::Clear` (26) | | Single command 26. r3 = the slot: `Device::Clear` (`sub_822A04F8`, `functions.md`) with r4 = 0 and r5 = 0 (no rectangles), r6 = +20 (the flags), r7 = +40 (the color), f1 = +44 (Z), r9 = +48 (the stencil), r10 = 0; r8 not set (Z's register slot, as the command table's `sub_82590620` leaves it). Returns the slot's +4. Not seen in play. Kept-kind. Read in full (B7 loop, 2026-11-06). |
| `sub_8258FB20` | `Single::DrawIndexedPrimitiveUP` (27) | | Single command 27. r3 = the slot: `BeginIndexedVertices` (`sub_825BDDC0`, `functions.md`) with r4 = +16 (the type), r5 = +20, r6 = +24 (the vertex count), r7 = +28 (the index count), r8 = +32 (the index format: 1 for 16-bit), r9 = +36 (the stride), r10 = r1 + 96 and the stack's +84 = r1 + 100 (where it returns the index and vertex space, zeroed first); when it returns 0, `memcpy` of the indices (count × 2, or × 4 when +32 isn't 1) from the address at +40 and of the vertices (count × stride) from +44, then `EndVertices`. Returns the slot's +4. A 144-byte frame. Not seen in play. Kept-kind (would be logged as outside command 27). Read in full (B7 loop, 2026-11-06). |
| `sub_8258E9B8` | `Single::Callback` (28) | | Single command 28. r3 = the slot: its +4 an object, whose vtable slot 0 is called with r3 = the object and r4 = the device. Returns 0. A 96-byte frame, 22 lines, one indirect call. Seen in play: the movie (`Movie::OnTick` queues it, `Movie::RenderFrame` `sub_820AE958` its slot 0, `movies.md`, overridden: our renderer draws the movie's frame there), on the boot movies. Kept-kind. Read in full (B7 loop, 2026-11-06). |
