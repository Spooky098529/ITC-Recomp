# Known functions: the render commands

The handlers the render queue's replay (`RenderQueue::Replay`,
`device-shaders.md`) runs for each command of a pass's list, on the
render thread, read in our `generated/` code (2026-11-01, for step 1 of
`../conventional-renderer.md`), with both tables read from the image.
Each command list slot is 16 bytes `[id, arg1, arg2, 0]` followed by its
payload ("Command format" in `../gpu-functions.md`); the chain walker
calls the handler from the table at `0x8265EDE0` with r3 = the payload,
r4 = arg1 (the count of items), r5 = arg2, and moves on by (its answer +
1) × 16 bytes. The single commands (`ChunkChain::RunSingles`, kind 0)
are one 16-byte slot each (`[id, arguments]`) and their handlers, from
the table at `0x8265ED48` with the same ids, take r3 = the slot; ids 0
and 29-36 there are `sub_825C8388` (returns 0, `functions.md`). The
device is `[0x8296BD90]` (set by the replay), the primitive drawer
`[0x82970BB8]` and its state `[0x82970BBC]`, the saved render target set
`[0x82970BC0]`, the release list `[0x8296BD8C]` (`../known-data.md`).
The library calls they reach are in `device-state.md`,
`device-shaders.md`, `graphics-resources.md` and `functions.md`
(`Device::Resolve`, `Device::Clear`, the inline vertex draws and the
tiling). Names are ours. "Not seen in play" is by the coverage list
(`../coverage.md`). Statuses as defined at the top of `functions.md`. Since 2026-11-01, `--conventional_renderer` (off by default until checked) runs our own handlers instead (`src/render/commands.cpp`, decoding into `src/render/device.h`; `../conventional-renderer.md`, step 1); the statuses below change when it goes on by default.

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_8258FBD0` | `Command::SetRenderStates` (1) | Wrapped | Read in full (2026-11-01). r3 the payload, r4 the count; for each 16-byte item `[state, value]`, the device's (`[0x8296BD90]`) setter at device + 56 + state, called with (device, value) (`device-state.md`'s render state setters). Returns the count (the slots its payload took, as every handler here returns). |
| `sub_8258FC30` | `Command::SetSamplerStates` (2) | Wrapped | Read in full. Items `[sampler, state, value]`: the setter at device + 440 + state with (device, sampler, value). Returns the count. |
| `sub_8258FC90` | `Command::SetBlendControls` (3) | Wrapped | Read in full. Items `[target, value]`: `Device::SetBlendControl` (`sub_822A3B30`). Returns the count. |
| `sub_8258FCE0` | `Command::SetTextures` (4) | Wrapped | Read in full. Items `[stage, texture]`: `Device::SetTexture` (`sub_822A6008`) with the dirty mask bit 63 >> (stage + 20). Returns the count. |
| `sub_8258E9F8` | `Command::SetDrawerVertices` (5) | Wrapped | Read in full. Leaf. The primitive drawer's state (`[0x82970BBC]`) +12 = the payload, +16 = the count (24-byte vertices). Returns (count × 24 + 15) / 16. |
| `sub_8258EA20` | `Command::SetDrawerIndices` (6) | Wrapped | Read in full. Leaf. Not seen in play. The drawer's state +20 = the payload, +24 = the count, +28 = r5 (32-bit indices when not 0). Returns (count × (r5 ? 4 : 2) + 15) / 16. |
| `sub_8258FD48` | `Command::SetShaderConstants` (7) | Wrapped | Read in full (2026-11-01). r3 the payload, r4 the count of registers, r5 = stage << 24 \| type << 16 \| first << 8. Stage 0 (vertex): type 0 `sub_822A84C8` (loop constants, not seen), 1 `Device::SetVertexShaderConstantF` (`sub_822A81F8`, with a mask of one bit per 4-register group: all ones from bit 63 for ((first + count − 1) / 4 − first / 4 + 1) groups, shifted right by first / 4), 2 `sub_822A8418` (bool, not seen), 3 `OutputDebugStringA` of `0x8206CB64` and nothing set. Stage 1 (pixel): `sub_822A8518`, `Device::SetPixelShaderConstantF` (`sub_822A8308`, the same mask), `Device::SetPixelShaderConstantB` (`sub_822A8470`), the debug string. Each called with (device, first, payload, count[, mask]). Returns the count; 0 for other stages. Wrapped by interpolation (`BlendState_ReplayConstants`, `src/interpolation/blend_state.cpp`): blends substituted into the payload before the upload and put back after. |
| `sub_8258FF50` | `Command::SetVertexShaders` (8) | Wrapped | Read in full. Items `[shader]`: `D3DDevice_SetVertexShader` (`sub_822A8958`). Returns the count. |
| `sub_8258FFA0` | `Command::SetPixelShaders` (9) | Wrapped | Read in full. Items `[shader]`: `Device::SetPixelShader` (`sub_822A8618`). Returns the count. |
| `sub_8258FFF0` | `Command::SetVertexDeclarations` (10) | Wrapped | Read in full. Items `[declaration]`: `Device::SetVertexDeclaration` (`sub_822A8960`). Returns the count. |
| `sub_82590040` | `Command::SetStreamSources` (11) | Wrapped | Read in full. Items `[stream, buffer, offset, stride]`: `Device::SetStreamSource` (`sub_822A3758`) with the dirty mask bit 63 >> ((95 − stream) / 3 + 20) (the division as (x × 21846) >> 16). Returns the count. |
| `sub_825900B8` | `Command::SetIndices` (12) | Wrapped | Read in full. Items `[buffer]`: `Device::SetIndices` (`sub_822A3908`). Returns the count. |
| `sub_8258EA58` | `Command::SetUpDrawer` (13, 14) | Wrapped | Read in full. Items `[a, b, c]`: the drawer's state (`[0x82970BBC]`) +0, +4, +8 = a, b, c, then the drawer's (`[0x82970BB8]`) vtable slot 2 with (drawer, device). Returns the count. |
| `sub_82590108` | `Command::DrawDrawer` (15) | Wrapped | Read in full. Count times, the payload unread: when the drawer's state +16 (its vertex count) isn't 0, `Device::GetRenderState_ViewportEnable` (`sub_822A2470`, its answer unused), `Device::SetRenderState_ViewportEnable` (`sub_822A2430`) with 0 when state +4 is 1, else 1; the drawer's slot 3 with (drawer, device, state +0, +12, +16) (in a race `PrimitiveDrawer::Draw`, `sub_825A17A0`, `model-animation.md`); then the flag set to 1. Returns the count. |
| `sub_825901B0` | `Command::DrawDrawerIndexed` (16) | Wrapped | Read in full. Not seen in play. As 15 with the drawer's slot 4 and (drawer, device, state +0, +12, +16, +20, +24, +28), and the flag put back as it was. Returns the count. |
| `sub_82590268` | `Command::DrawVertices` (17) | Wrapped | Read in full. Not seen in play. Items `[type, start, count]`: `DrawVertices` (`sub_825BE300`) with (device, type, start, count × a + b), a and b the type's words in the table at `0x82000950` (8 bytes a type). Returns the count. |
| `sub_825902E0` | `Command::DrawIndexedVertices` (18) | Wrapped | Read in full. 12-byte items `[type, base << 16 \| start, count]`: `DrawIndexedVertices` (`sub_825BE708`) with (device, type, base (signed), start, count × a + b) from the same table. Returns the count. |
| `sub_82590360` | `Command::DrawVerticesUP` (19) | Wrapped | Read in full. 12-byte items: +0 the type, +4 the vertex count and +6 the stride (halfwords), +8 the data: `BeginVertices` (`sub_825BD8F0`) with (device, type, count, stride); when it returns a pointer, `memcpy` of count × stride bytes into it and `EndVertices` (`sub_825BDDA0`). Returns the count. |
| `sub_825903D8` | `Command::Release` (20) | Wrapped | Read in full. Not seen in play. Items `[object]`: `Resource::Release` (`sub_822A67B0`) when it isn't 0. Returns the count. |
| `sub_8258EAD0` | `Command::Skip` (21) | Wrapped | Read in full. Leaf. Returns (count + 15) / 16: the payload is count bytes of data. |
| `sub_82591798` | `Command::RetireObjects` (22) | Wrapped | Read in full. Items: +4 an object, +8 a flag. The object's vtable slot 2; when its answer's low byte is 0 and the flag is 1: slot 1, `PtrVector::PushBack` (`sub_82591540`) of the item's +4 onto the release list (`[0x8296BD8C]`, emptied at the replay's end), slot 3. Returns the count. |
| `sub_82590428` | `Command::SetRenderTargets` (23) | Wrapped | Read in full. Items `[index, surface]`: `Device::SetRenderTarget_External` (`sub_822A4340`). Returns the count. |
| `sub_82590478` | `Command::SetDepthStencilSurfaces` (24) | Wrapped | Read in full. Items `[surface]`: `Device::SetDepthStencilSurface` (`sub_822A4060`). Returns the count. |
| `sub_825904C8` | `Command::Resolve` (25) | Wrapped | Read in full. 68-byte items: +0 which fields are given (bit 0 the rect at +8, 1 the destination at +24, 2 the point at +28, 3 the clear color, 4 the parameters at +56), +4 the flags, +36 the level, +40 the slice, +44 to +47 the clear color's bytes (each times the float at `0x82002200`, probably 1/255, into four floats on the stack), +48 the clear Z (f1), +52 the stencil: `Device::Resolve` (`sub_822AFA58`) with (device, flags, rect, destination, point, level, slice, color, Z, stencil, parameters), each absent one 0. Returns (count × 68 + 15) / 16. |
| `sub_82590620` | `Command::Clear` (26) | Wrapped | Read in full. 280-byte items: +4 the rect count, +12 the rects (passed only when the count isn't 0), +8 the flags, +268 the color, +272 Z (f1), +276 the stencil: `Device::Clear` (`sub_822A04F8`) with (device, count, rects, flags, color, Z, stencil, 0). Returns (count × 280 + 15) / 16. |
| `sub_82590698` | `Command::DrawIndexedVerticesUP` (27) | Wrapped | Read in full. Not seen in play. 32-byte items: `BeginIndexedVertices` (`sub_825BDDC0`) with (device, the item's +0 to +20, and where to put the index and vertex pointers); when it returns 0, `memcpy` of +12 × (2 when +16 is 1, else 4) bytes from +24 to the index pointer and of +20 × +8 bytes from +28 to the vertex pointer, then `EndVertices`. Returns (count × 32 + 15) / 16. |
| `sub_8258EAE0` | `Command::Callback` (28) | Wrapped | Read in full. Not seen in play. Items: +4 an object: its vtable slot 0 with (object, device). Returns the count. |
| `sub_82590768` | `Command::RestoreTargets` (29) | Wrapped | Read in full. 4-byte items, a mask: bits 0-3 render targets 0-3 set from the saved set at `[0x82970BC0]` +0 to +12 (`Device::SetRenderTarget_External`), bit 7 the depth surface from +16 (`Device::SetDepthStencilSurface`). Returns the count. |
| `sub_82590860` | `Command::SetViewports` (30) | Wrapped | Read in full. Items: +0 X, +2 Y, +4 width, +6 height (halfwords, widened to words on the stack), +8 min Z, +12 max Z: `Device::SetViewport` (`sub_822A3518`). Returns the count. |
| `sub_825908E0` | `Command::SetScissorRects` (31) | Wrapped | Read in full. 8-byte items: left, top, right, bottom (signed halfwords, widened): `Device::SetRenderState_ScissorTestEnable` (`sub_822A3D20`) with 1, then `Device::SetScissorRect` (`sub_822A3450`). Returns the count. |
| `sub_82590968` | `Command::BeginTiling` (32) | Wrapped | Read in full. Count times with the same payload `[flags, tile count, tile rects]`: `sub_825BED08` (BeginTiling) with (device, flags, count, rects, a clear color of four times the float at `0x82000A34`, Z the same float, stencil 0). Returns the count. |
| `sub_825909E8` | `Command::EndTiling` (33) | Wrapped | Read in full. Count times with the same payload `[flags, destination]`: `Device::EndTiling` (`sub_825BF200`) with (device, flags, 0, destination, the same clear color, Z, 0, 0). Returns the count. |
| `sub_82590A68` | `Command::SaveTargets` (34) | Wrapped | Read in full. The payload unread: into the save area at `0x8296BD60` (the device from its +48, `[0x8296BD90]`), `Device::GetViewport` (`sub_822A3740`) at +0, `Device::GetRenderTarget` (`sub_822A3998`) 0-3 at +24 to +36, `Device::GetDepthStencilSurface` (`sub_822A39E0`) at +40. Returns the count. |
| `sub_82590AD0` | `Command::RestoreSaved` (35) | Wrapped | Read in full. What 34 saved: render targets 0-3, the depth surface, the viewport (`SetRenderTarget_External`, `SetDepthStencilSurface`, `SetViewport`). Returns the count. |
| `sub_82304258` | | | Read in full. Leaf: returns r4. Not seen in play. The command table's 0, 36 and 37 (the walkers stop at 36 before calling it), and other callers not looked at. |
| `sub_8258EF60` | `SingleCommand::SetRenderStates` (1) | | Read in full. Not seen in play. r3 a single command's slot: +0 the id, +4 the count, the items from +16 (16 bytes each, as for every single command here). As command 1. Returns the count (the slots after this one its items took, as every single command returns). |
| `sub_8258EFC0` | `SingleCommand::SetSamplerStates` (2) | | Read in full. Not seen in play. As command 2. Returns the count. |
| `sub_8258F028` | `SingleCommand::SetBlendControls` (3) | | Read in full. Not seen in play. As command 3. Returns the count. |
| `sub_8258F080` | `SingleCommand::SetTextures` (4) | | Read in full. Not seen in play. As command 4. Returns the count. |
| `sub_8258E8E8` | `SingleCommand::SetDrawerVertices` (5) | | Read in full. Leaf. The drawer's state +12 = the slot + 16, +16 = the count. Returns (count × 24 + 15) / 16. |
| `sub_8258E918` | `SingleCommand::SetDrawerIndices` (6) | | Read in full. Leaf. Not seen in play. State +20 = the slot + 16, +24 = the count, +28 = the slot's +8. Returns (count × (+8 ? 4 : 2) + 15) / 16. |
| `sub_8258F0F0` | `SingleCommand::SetShaderConstants` (7) | | Read in full. +4 the stage, +8 the type (0-4), +12 the first register and +13 the count (bytes), the data from +16. Types 0-3 as command 7's; type 4 (bool constants from a bitmask: +16 the bits, +20 how many) each bit made a word on the stack, then `sub_822A8418` (vertex) or `Device::SetPixelShaderConstantB` (pixel). Returns the count byte; 0 for other stages. Not wrapped by interpolation. |
| `sub_8258F3E0` | `SingleCommand::SetVertexShaders` (8) | | Read in full. Not seen in play. As command 8. Returns the count. |
| `sub_8258F438` | `SingleCommand::SetPixelShaders` (9) | | Read in full. Not seen in play. As command 9. Returns the count. |
| `sub_8258F490` | `SingleCommand::SetVertexDeclarations` (10) | | Read in full. Not seen in play. As command 10. Returns the count. |
| `sub_8258F4E8` | `SingleCommand::SetStreamSources` (11) | | Read in full. Not seen in play. As command 11. Returns the count. |
| `sub_8258F568` | `SingleCommand::SetIndices` (12) | | Read in full. Not seen in play. As command 12. Returns the count. |
| `sub_8258E958` | `SingleCommand::SetUpDrawer` (13, 14) | | Read in full. The drawer's state +0, +4, +8 = the slot's +4, +8, +12, then the drawer's slot 2 with (drawer, device). Returns 0. |
| `sub_8258F5C0` | `SingleCommand::DrawDrawer` (15) | Wrapped | Read in full (2026-11-01). When the drawer's state +16 isn't 0: the viewport-enable flag read and kept, set as command 15 sets it, the drawer's slot 3 with (drawer, device, state +0, +12, +16), then `Device::SetStreamSource` (device, 0, 0, 0, 0, mask `0x1000`) and `Device::SetTexture` (device, 0, 0, mask bit 43) unbind stream 0 and texture 0, and the flag is put back as it was. Returns 0. Seen in a race (the minimap's player triangle). Wrapped in `src/native/replay_hooks.cpp`: marked as command 15 so the native renderer draws its inline vertices. |
| `sub_8258F6A0` | `SingleCommand::DrawDrawerIndexed` (16) | | Read in full. Not seen in play. As single 15 with the drawer's slot 4 and (drawer, device, state +0, +12, +16, +20, +24, +28). Returns 0. |
| `sub_8258F770` | `SingleCommand::DrawVertices` (17) | | Read in full. Not seen in play. +4 the type, +8 the start, +12 the count: as command 17. Returns 0. |
| `sub_8258F7C8` | `SingleCommand::DrawIndexedVertices` (18) | | Read in full. Not seen in play. +4 the type, +8 base << 16 \| start, +12 the count: as command 18. Returns 0. |
| `sub_8258F828` | `SingleCommand::DrawVerticesUP` (19) | | Read in full. Not seen in play. +4 the type, +8 the count and +10 the stride (halfwords), +12 the data: as command 19. Returns 0. |
| `sub_8258F898` | `SingleCommand::Release` (20) | | Read in full. Not seen in play. As command 20. Returns the count. |
| `sub_820911F8` | | | Read in full. Leaf: returns the word at r3 + 4. Not seen in play. The single command table's 21 (a skip of count slots); other callers not looked at. |
| `sub_825916F0` | `SingleCommand::RetireObjects` (22) | | Read in full. Not seen in play. Items: +20 an object, +24 a flag (each slot's +4 and +8): its vtable slot 1, then slot 2; when that answers 0 (low byte) and the flag is 1, `PtrVector::PushBack` onto the release list and slot 3. The order differs from command 22's (slot 1 there only after the check). Returns the count. |
| `sub_8258F8F0` | `SingleCommand::SetRenderTargets` (23) | | Read in full. Not seen in play. Items: +20 the index, +24 the surface: as command 23. Returns the count. |
| `sub_8258F948` | `SingleCommand::SetDepthStencilSurfaces` (24) | | Read in full. Not seen in play. Items: +20 the surface: as command 24. Returns the count. |
| `sub_8258F9A0` | `SingleCommand::Resolve` (25) | | Read in full. Not seen in play. One 68-byte item at +16, as command 25's. Returns the count. |
| `sub_8258FAC8` | `SingleCommand::Clear` (26) | | Read in full. Not seen in play. `Device::Clear` with (device, 0, no rects, flags +20, color +40, Z +44, stencil +48, 0). Returns the count. |
| `sub_8258FB20` | `SingleCommand::DrawIndexedVerticesUP` (27) | | Read in full. Not seen in play. One 32-byte item at +16, as command 27's. Returns the count. |
| `sub_8258E9B8` | `SingleCommand::Callback` (28) | | Read in full. The object at +4: its vtable slot 0 with (object, device). Returns 0. |
