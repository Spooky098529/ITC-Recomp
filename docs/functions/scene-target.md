# Known functions: the scene target's passes

The scene target (`[0x8266D08C]`, a 32-byte object made once at boot by
`SceneTarget::Create` and `SceneTarget::SceneTarget`, rows in
[`app-start-up.md`](app-start-up.md)) holds the surfaces the 3D scene is
drawn into and the 1280 × 720 texture it's resolved into: +0 a depth
surface and +4 a color surface, both 1024 × 576; +8 and +12 the same
pair at the size of the display's largest tile (1280 × 256 in play,
multisampled), for the menus' predicated tiling; +24 the 1280 × 720
texture; +28 the mode `Modes::SetSceneTargetMode` sets
([`mode-tick.md`](mode-tick.md)). The functions here record the chunks
that bind those surfaces, clear them, resolve them into +24 and draw +24
back over the screen (the race's 1024 × 576 view scaled up to 1280 × 720,
the "upscale draw" in `../conventional-renderer.md`), and the textured
screen rectangle that last draw goes through. `SceneTarget::RecordPasses`
runs once a frame from `sub_82096418` (the 44-byte frame passes object's
vtable slot, a lead in `../loops/leads.md`, which stays there); `Glow::Record`
(`post-effects.md`, shared) puts the scene target's surfaces back after the
glow's passes through `SceneTarget::RecordBoundTargets`. Mapped by the B7
loop (`../loops/b7-loop.md`) from its lead in `../loops/leads.md`
(2026-11-05).

They are command list recorders: the list and its allocators are in
[`command-list.md`](command-list.md), which keeps recorders out of its
rows, so they get this file. The commands and their items are in
`../gpu-functions.md` ("Command table", "Pass command items", "Predicated
tiling"). Every command recorded here is replayed and read by our
renderer: 23 and 24 (their handlers store the surfaces at device +12184
to +12196 and +12200, which `src/native/passes.cpp` reads to pick each
pass's target and depth), 29 (render targets 0-3 and depth back from the
saved set at `0x82970BC0`: mask 143 = `0x8F` everywhere here), 25 (the
resolve the renderer turns into a copy into +24's image by its fetch
constant, `State::Resolve`), 26 (Clear: flags 49 = render target 0, Z and
stencil; color `0xFF000000`; Z 0.0), 32 and 33 (BeginTiling and
EndTiling, whose per-tile resolves go through `Device::Resolve`'s wrap),
1, 2 and 4 (render states, sampler states, the texture), 13, 5 and 15
(the helper object's inline quad, drawn by the renderer's inline draw)
and 36 (a chain's end). So by the graphics call ("native if the renderer
needs it, else Dropped", `../loops/b7-loop.md`'s "Precedent first"),
nothing here is Dropped; EDRAM isn't emulated, but these are the binds and
resolves the renderer works from. Chunks are queued with
`RenderQueue::Add` (`device-shaders.md`; r5 the bucket, f1 0.0 from
`0x82000A34`) through the scene's render manager `[0x826BABA0]` (slot 8
`sub_820944D8`, no row; its answer's slot 2 the render queue), and the
textured rectangle's on the list at `[0x826D83D0 + 4]` with the queue at
`+0` (the color rectangles' and the minimap's, `mode-tick.md`,
`minimap.md`). `0x82000A30` is 1.0, `0x82001070` 0.8 (1024 / 1280 and
576 / 720). Not in play, so no rows: the scene target's destructor
`sub_8209BCB8` (from `App::ShutDownServices`, `boot-mode.md`) and
`sub_82134698`, the other caller of `SceneTarget::RecordBoundTargets`
(named in `post-effects.md`'s rows). Fields: `../known-data.md`
(`0x8266D08C`). The rest of the game's functions are in
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
| `sub_8209B3D0` | `SceneTarget::RecordPasses` (probably) | | Not read. Records a resolve (`CommandList::AllocateResolve`, calls at `0x8209B8AC` and `0x8209BAD8`, checked) into the scene target's 1280x720 texture (`SceneTarget` + 24, `src/app_start_up/start_up.h`); probably the main view's resolve into `1ECB9000` that the upscale draw samples. Turns the scene target's mode into pass bits (`mode-tick.md`). Read in full (B7 loop, 2026-11-05): r3 = the scene target (`[0x8266D08C]`), r4 = the command list, r5, r6, r7 = three render queue buckets (150, 602 and 121 from its only caller `sub_82096418`, once a frame, return address `0x82098620`); a 656-byte frame, r5 and r6 also stored into the caller's frame (r1 + 692 and + 700 from its own, the parameter save area). The mode (+28) gives the bits at r1 + 80: 0 gives 7, 1 4, 2 0, 3 1, 4 8, 5 and up 0. Each block a chunk (`CommandList::OpenChunk`, a header slot: halfword +2 = 5, +4 = 0; `CloseChunk`; `RenderQueue::Add` with the header, the bucket and f1 = 0.0, through `[0x826BABA0]`'s slot 8 then slot 2, slot 4). Bit 8 (the menus' tiling): `SceneTarget::RecordTileTargets`; [32, 1, 0] with [0, the tile count `[0x8260034C]` + 12, the tiles `[0x8260034C]` + 16] (BeginTiling); [26, 1, 0] and 280 bytes (`AllocateBytes`, no header: +0 = +4 = 0, +8 = 49, +268 = `0xFF000000`, +272 = 0.0, +276 = 0, the rest not written); [36, 0, 0]; bucket 108. Then a second chunk: [33, 1, 0] with [771 (`0x303`), +24] (EndTiling into the texture); [29, 1, 0] with [143]; [36, 0, 0]; bucket 594; and `TexturedRect::Record` (r4 = 0, r5 = 594) of a rectangle at r1 + 96: 0, 0, 1280 × 720, texture coordinates 0.0, 0.0 to 1.0, 1.0, texture +24, blending off (+116 = 0); its +0 to +63 a 4 × 4 block built from vector constants (+12, +28, +44, +60 then = 1.0), +64 = +68 = 0, +72 = 2, +76 not written, none of which the rectangle's recorder reads. Bit 1 (modes 0 and 3): [29, 1, 0] with [143], the same clear, [36, 0, 0]; bucket r7. Bits 2 or 4 (modes 0 and 1, the race): with bit 2 first [25, 1, 0] and a resolve item (`AllocateResolve`, no header): +0 = 2 (a destination only), +4 = 0, +24 = +24, +36 = +40 = 0, +44 to +47 the bytes 0, 0, 0, 255, +48 = 0.0, +52 = 0, +56 = `0x18280186`, +60 = 0, +64 = `0x1A220197` (the rect +8 to +20 and the point +28, +32 not written); then `SceneTarget::RecordBoundTargets`'s work (`SceneTarget::RecordTargets(r3, r4, 0)`: +4 and +0 bound); the same clear; [1, 1, 0] with [40, 0] (Z enable off); [36, 0, 0]; bucket r5; with bit 2, `TexturedRect::Record` (r5) of a rectangle at r1 + 224: 0, 0, 1024 × 576, 0.0, 0.0 to 1.0, 1.0, texture +24 (the frame drawn so far shrunk into the 1024 × 576 target). Bit 4: [25, 1, 0] and a resolve item, +0 = 3 (a rect and a destination), the rect 0, 0, 1024, 576, the rest as above (the point not written); [29, 1, 0] with [143]; [1, 1, 0] with [40, 0]; [36, 0, 0]; bucket r6; `TexturedRect::Record` (r6) of a rectangle at r1 + 352: 0, 0, 1280 × 720, 0.0, 0.0 to 0.8, 0.8 (`0x82001070`), texture +24 (the race's 1024 × 576 view scaled to the screen: the upscale draw, resolve 24 in `../conventional-renderer.md`'s table). Calls `CommandList::OpenChunk`, `AllocateSlots`, `AllocateBytes`, `AllocateResolve`, `CloseChunk` (`command-list.md`), `SceneTarget::RecordTargets`, `RecordTileTargets`, `TexturedRect::Record`, and the render manager's slots (16 indirect calls). Its group (with `RecordTileTargets`, `TexturedRect::Record` and `RecordFlat`, 1,157 lines) waits only on `SceneTarget::RecordTargets`. Every command it records is replayed and read by the renderer (the intro): native, nothing Dropped. |
| `sub_8209AE48` | `SceneTarget::RecordTargets` (probably) | Overridden | Read in full (B7 loop, 2026-11-05): r3 = the scene target, r4 = the command list, r5 not read (143 from `Glow::Record` through `RecordBoundTargets`, 0 from `RecordPasses`). In the open chunk, slot by slot (`CommandList::AllocateSlots(list, 1)` each): [23, 4, 0] (SetRenderTarget, four items); [0, +4], [1, +4], [2, +4], [3, +4] (render targets 0-3 all the 1024 × 576 color surface +4, as `PostEffect::RecordTargets` does; the items' +8 and +12 not written); [24, 1, 0] (+4 = 1, +8 = 0, its +12 not written); [+0] (the depth surface; +4 to +12 not written). Leaf apart from `AllocateSlots`; a 128-byte frame. From `SceneTarget::RecordBoundTargets` (its tail call) and `RecordPasses` (return address `0x8209B8FC`), every frame in races. Its group (1 function, 76 lines) is ready. The renderer reads what commands 23 and 24 bind (the intro). Native (2026-11-05): `src/scene_target/scene_target.cpp`, `--native_scene_target` (adapter in `src/hooks/scene_target.cpp`; the original where the list hasn't room for the 7 slots, and with the command list's diagnostics on; harness `scene_target`). Runs in attract races. |
| `sub_8209AF40` | `SceneTarget::RecordTileTargets` (probably) | Overridden | Read in full (B7 loop, 2026-11-05): as `SceneTarget::RecordTargets` with the tile-sized pair: render targets 0-3 all +12, then depth +8 (the same slots and the same words left unwritten). Only caller `SceneTarget::RecordPasses` (bit 8, the menus' predicated tiling; return address `0x8209B4C0`); seen in play. Its group (1 function, 76 lines) is ready. Native (2026-11-05): `src/scene_target/scene_target.cpp`, `--native_scene_target` (adapter in `src/hooks/scene_target.cpp`, as `RecordTargets`; harness `scene_target`). Not seen in attract races (the menus). |
| `sub_8209B058` | `SceneTarget::RecordBoundTargets` (probably) | Overridden | Read in full (B7 loop, 2026-11-05): r3 = the command list, r4 = a value passed on as r5 (143 from both callers); tail-calls `SceneTarget::RecordTargets([0x8266D08C], list, r4)`, which doesn't read it. From `Glow::Record` (`post-effects.md`, return address `0x821382E0`, after the star streaks: the scene target's surfaces bound again after the glow's own) and `sub_82134698` (never ran). Its group (1 function, 9 lines) waits only on `SceneTarget::RecordTargets`; `Glow::Record` (Wrapped by frame interpolation) waits on it. Native (2026-11-05): `src/scene_target/scene_target.cpp` (`RecordTargets`' plain C++ on `[0x8266D08C]`), `--native_scene_target` (adapter in `src/hooks/scene_target.cpp`, kept while `Glow::Record` is recompiled; harness `scene_target`). Runs in attract races. |
| `sub_8217C368` | `TexturedRect::Record` (probably) | Overridden | Read in full (B7 loop, 2026-11-05): a branch to `TexturedRect::RecordFlat` with the same registers. Only caller `SceneTarget::RecordPasses` (three calls, return addresses `0x8209B738`, `0x8209BA80`, `0x8209BCA4`). Its group (with `RecordFlat`, 2 functions, 340 lines, 1 indirect call: `RenderQueue::Add`) is ready. Native (2026-11-05): its adapter (`src/hooks/scene_target.cpp`, `hooks::SceneTargetTexturedRect`, `--native_scene_target`) runs `TexturedRect::RecordFlat`'s plain C++ and hands the chunk to the render queue at `[0x826D83D0]` (its slot 4, a guest call from a 272-byte frame like the original's, return address `0x8217AEA0`); the original where the plain C++ declines and with the command list's diagnostics on (harness `scene_target`). Runs in attract races. |
| `sub_8217AA98` | `TexturedRect::RecordFlat` (probably) | Native | Read in full (B7 loop, 2026-11-05): `ColorRect::RecordFlat`'s textured sibling (`mode-tick.md`; the rectangle's layout as `ScreenRect::Record`'s, `sprites.md`). r3 = a rectangle (+80 x, +84 y, +88 width, +92 height, integers; +96, +100, +104, +108 the texture coordinates u0, v0, u1, v1; +112 the texture; +116 blending on), r4 not read, r5 = the bucket. On the command list at `[0x826D83D0 + 4]` a chunk: `OpenChunk`, the header (halfword +2 = 5, +4 = 0); [4, 1, 0] with [0, +112] (SetTexture, sampler 0); [1, 5, 0] and, with +116 set, [56, 6], [96, 0], [60, 1], [72, 6], [76, 7], else [56, 6], [96, 0], [60, 0], [72, 1], [76, 0]; [2, 2, 0] with [0, 20, 1] and [0, 16, 1] (sampler 0's min and mag filters); [13, 1, 0] with [6, 1, 2]; [5, 4, 0] and four 24-byte vertices (`AllocateVertices(4, 0)`): x, y, 1.0, `0xFFFFFFFF` (white), u, v for the corners (x, y; x + width, y; x, y + height; x + width, y + height, as floats of the integers, with u0 or u1 and v0 or v1 to match); [15, 1, 0] and one slot after it allocated and not written (its item); [36, 0, 0]; `CloseChunk`; `RenderQueue::Add` through `[0x826D83D0]`'s slot 4 with the header, the bucket and f1 = 0.0 (`0x82000A34`). Its 272-byte frame: 1.0 stored four times at r1 + 124 to + 172 (16 apart, never read) and the integers' conversions at r1 + 80 to + 104. Only caller `TexturedRect::Record`. The renderer draws the quad through the helper object's inline vertices (command 15, `../gpu-functions.md`). Native (2026-11-05): `src/scene_target/scene_target.cpp`, no adapter (only `TexturedRect::Record`'s adapter calls it; `--native_scene_target` switches that). Declines (the original runs) where a chunk is open or the list hasn't room for its 24 slots; its frame's words (+80 to +111, the 1.0s at +124 to +172) written as the original's; blending on is +116 not 0 (`cmpwi`, not above 0 as `ColorRect::RecordFlat`'s). Harness `scene_target` (the texture coordinates compared as floats). Frame dumps against `--native_scene_target=false`: the same 2,430 draws with the same states. Runs in attract races. |
