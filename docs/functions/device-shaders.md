# Known functions: the device's shader setters and the render queue

The graphics device's side under the primitive drawer
(`PrimitiveDrawer::Init`, `SetShaders` and `Draw` in
`model-animation.md`, which these block): the graphics library's (the
360's Direct3D, linked into the game) calls that bind a vertex shader, a
pixel shader and a vertex declaration on the device, and the render
queue (scene + 64, vtable `0x8206CB8C`) whose constructor makes the
primitive drawer, with its two pass lists. Mapped by the B7 loop
(`../loops/b7-loop.md`) from its lead in `../loops/leads.md` (found by
the model animation's mapping agent, 2026-10-23), 2026-11-01.

**The setters.** Each writes the device's shader state the way the
setters in `device-state.md` do: the bound object (device +12436 the
vertex shader, +12432 the pixel shader, +11552 the vertex declaration;
`../known-data.md`), a bit in the shader state's dirty mask (device +16:
bit 51 the vertex shader and declaration, bits 52 and 49 the pixel
shader; the units by bit in `../gpu-functions.md`, "The register shadow
as a whole"), and for a shader, the register patches its object carries
(at the vertex shader's +592 and the pixel shader's +56, where the file's
header was copied; `shader-objects.md`) applied to the register shadow
at device +1024, with the shader's float constant mask ORed into device
+0 (vertex) or +8 (pixel). Their callers in play: the command replay's
commands 8, 9 and 10 (`sub_8258FF50`, `sub_8258FFA0`, `sub_8258FFF0`,
Wrapped, `functions.md`; `../gpu-functions.md`'s command table, which
already named these three), `VideoOutput::UnbindAll` (`sub_8258D9F8`,
`video-output.md`, both shaders set to 0) and the primitive drawer
(`model-animation.md`). Not seen in play: `sub_8229DA28` (calls all
three), `sub_8258F3E0`, `sub_8258F438`, `sub_8258F490` (one each,
probably the single commands' versions of 8, 9 and 10), the drawer's
`sub_825A1708` and `sub_825A1810`; the movie library's
(`sub_824829A0`, `sub_82483FB8`, `sub_824840C8`, `sub_824842C8`) no
longer run since the FFmpeg movies (`movies.md`, "Rules that still hold"
in `../loops/b7-loop.md`).

Graphics: native if our Vulkan renderer needs it, else Dropped (the
user's rule, followed as `device-state.md` did). All three setters are
kept-kind: the renderer reads the bound shaders and the declaration
(`kDevicePixelShader`, `kDeviceVertexShader`,
`kDeviceVertexDeclaration` in `src/native/state.h`), the register
shadow by the dirty masks (`ReadRegisters` in `src/native/state.cpp`),
and the library's draw flush reads them too (`sub_822AED18`, the shader
load, `functions.md`). None writes a packet, waits on a fence or flushes
a cache, so nothing here is Dropped. `Device::SetVertexShader`'s
reference counting goes through `Resource::AddRef` and `Release`
(`graphics-resources.md`, Overridden: lock-free, and skipped for a
rebind of the bound shader, `src/native/refcount.cpp`); the pixel shader
and the declaration take no reference.

**The render queue** (scene + 64, `[0x8296B91C]` and `[0x82970BD4]`,
`../known-data.md`; called "the game mode object" in
`../frame-interpolation.md`): 16 bytes made in place by the scene's
constructor `sub_82098A98` (a lead in `../loops/leads.md`). +0 the vtable
`0x8206CB8C` (slot 2 `sub_8258ED78` clears the list being recorded, slot
6 `sub_8258E5E0` flips the replay index, both Wrapped in `functions.md`;
slot 4 `sub_8258EDC8` queues a chunk, 7 `sub_8258E5F8` and 8
`sub_8258E618` the lists being recorded and replayed: no rows, a lead),
+4 its 88-byte state (+0 the replay index, +4 the primitive drawer, +68
a `std::vector`'s allocator, first, last and end at +72, +76, +80, +84 a
float sequence), +8 and +12 the two pass lists (36 bytes each: vtable
`0x8206CB88`, +4 a byte set to 1, +8 4,096 and +12 1,024 (the counts
asked for), +16 a count, +20 4,096 × 20 bytes, +24 1,024 16-byte
buckets with their next pointers at +8, +28 and +32 counts). How the
pass lists are filled and replayed is the lead's.

Shared rows: `VertexShader::Create` (`sub_822AA018`) and
`PixelShader::Create` (`sub_822A9EE8`), which make the drawer's shaders,
in `shader-objects.md`; `CreateVertexDeclaration` (`sub_822A8980`),
which makes its declaration, in `app-start-up.md`;
`PrimitiveDrawer::PrimitiveDrawer` (`sub_825A1378`, Native) and the
drawer's other methods in `model-animation.md`; `Resource::AddRef` and
`Release` (`sub_822A6738`, `sub_822A67B0`) in `graphics-resources.md`;
`memcpy` (`sub_825A7410`), `operator new` (`sub_825A6880`),
`operator new[]` (`sub_825A6940`) and the frees (`sub_825A71C0`,
`sub_825A70E8`) in `c-runtime.md` and `heap.md`. Not mapped here: the
shader compiler (`sub_822B8F10`, replaced by stored shaders since
2026-10-25, `--use_hardcoded_shaders`, `functions.md`; a library
replaced wholesale isn't mapped), and the inline draw the drawer's
`Draw` makes (`BeginVertices` `sub_825BD8F0`, `EndVertices`
`sub_825BDDA0`, Wrapped in `functions.md`) with the command buffer under
it (`8229DC28`-`8229F228`, already a lead in `../loops/leads.md`).

**Ours** (B7 loop, 2026-11-01): the seven functions seen in play, in
`src/graphics/device_shaders.cpp` behind `src/hooks/device_shaders.cpp`
(`--native_device_shaders`, harness `device_shaders`).
`PassList::Clear`'s adapter stays while the queue's slot 2
`sub_8258ED78` (Wrapped) calls it through `ctx`.

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
|---------|------|--------|-------|
| `sub_822A8958` | `D3DDevice_SetVertexShader` (probably: the entry the callers use) | Overridden | r3 = the device, r4 = a vertex shader or 0: device +13156 = r4, then tail-calls `Device::SetVertexShader` (`sub_822A8808`) with the same r3 and r4. Nothing in `generated/` reads device +13156 (probably the shader the title set, for a state save; the three other stores at that offset are other objects'). Leaf but for the tail call. Callers seen in play: command 8's handler `sub_8258FF50` (as often as command 9, about every draw), `VideoOutput::UnbindAll` (with 0) and `PrimitiveDrawer::SetShaders` (`sub_825A16B0`); not seen: `sub_8229DA28`, `sub_8258F3E0`, `sub_825A1708` and the movie library's (no longer run). Kept-kind (graphics): the renderer reads what it binds. Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::SetVertexShaderEntry` in `src/graphics/device_shaders.cpp` (the store, then `SetVertexShader`'s plain C++), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`). Harness `device_shaders`: 100 cases (the recompiled one run with the flag off, so its tail call reaches the old wrapper), 0 differ. |
| `sub_822A8808` | `Device::SetVertexShader` | Overridden | Read in full (B7 loop, 2026-11-01): r3 = the device, r4 = a vertex shader or 0; a 128-byte frame (r28-r31 saved). `Resource::AddRef(r4)` when r4 isn't 0, then `Resource::Release` on the bound one (device +12436) when that isn't 0 (also when it's the same object, which the wrapper skips); +12436 = r4; device byte +10810 has its top bit (0x80) cleared (what it means isn't known); +16 ORed with 1 << 51. When r4 isn't 0 and its patch offset (r4 + 592 + 12) isn't 0, the patches at p = r4 + 592 + that offset: device +0 ORed with the doubleword p + 0 (the vertex float constants' dirty mask); when the doubleword p + 8 isn't 0, device +24 ORed with 2; then the records from p + 24 to p + 24 + the word at p + 16: first copy records (halfword +0 a byte offset into the register shadow at device +1024, halfword +2 a word count, then that many words, copied with `memcpy` `sub_825A7410`) until one with a count of 0; then masked records (the same header, then count / 2 pairs of words: shadow word = (shadow word AND first) OR second, one shadow word a pair) until another count of 0 or the end. Only caller `sub_822A8958` (its tail call). Kept-kind (graphics): the renderer reads +12436, the shadow and the dirty masks (`src/native/state.h`, `state.cpp`); a native one keeps the wrapper's skip (the count is unchanged either way). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::SetVertexShader` (`src/graphics/device_shaders.cpp`): `refcount::AddRef`'s plain C++, Release through its override (a guest call, the lock-free count; it may destroy the bound shader through `sub_822A6210`), the patches' copies through `native::Buffers_Memcpy` (memcpy through the guest where it refuses), r3 as the original leaves it; the wrapper's rebind skip kept (AddRef and Release not made for the shader already bound; r3 its count, as the wrapper's Release gave) with its 5-second count log (`refcount::NoteSetVertexShader`). The adapter moved from `src/native/refcount.cpp` to `src/hooks/device_shaders.cpp` (`--native_device_shaders`); with the flag off it runs the old wrapper (the skip around `__imp__`), and `--refcount_global_lock` still runs the original there. Harness `device_shaders`: 200 cases (against `__imp__` without the skip: the count comes out the same), 0 differ. |
| `sub_822A8618` | `Device::SetPixelShader` (probably the library's `D3DDevice_SetPixelShader`) | Overridden | r3 = the device, r4 = a pixel shader or 0; a 128-byte frame (r28-r31 saved). Takes no reference (no `AddRef` or `Release`): +12432 = r4; +16 ORed with 1 << 52, then ORed with 1 << 49 (stored after each). When r4 isn't 0 and its patch offset (r4 + 56 + 12) isn't 0, the patches at p = r4 + 56 + that offset, as `Device::SetVertexShader`'s but into device +8 (the pixel float constants' dirty mask) instead of +0: +24 ORed with 2 when the doubleword p + 8 isn't 0, then the copy records (`memcpy`) and the masked records into the shadow at device +1024. 90 lines. Callers seen in play: command 9's handler `sub_8258FFA0`, `VideoOutput::UnbindAll` (with 0), `PrimitiveDrawer::SetShaders`; not seen: `sub_8229DA28`, `sub_8258F438`, `sub_825A1708` and the movie library's. Kept-kind (graphics): the renderer reads +12432 (`kDevicePixelShader`), the shadow and the dirty masks. Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::SetPixelShader` (`src/graphics/device_shaders.cpp`, the patches as `SetVertexShader`'s), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`). Harness `device_shaders`: 200 cases, 0 differ. |
| `sub_822A8960` | `Device::SetVertexDeclaration` (probably the library's `D3DDevice_SetVertexDeclaration`) | Overridden | r3 = the device, r4 = a vertex declaration (or 0): +11552 = r4, +16 ORed with 1 << 51 (the vertex shader's bit: the shader's microcode is patched for the declaration at the draw, `sub_822AE340`, `functions.md`). No reference taken. Leaf, 10 lines. Callers seen in play: command 10's handler `sub_8258FFF0`, `PrimitiveDrawer::Draw` (`sub_825A17A0`, with `[[drawer + 4]]`); not seen: `sub_8229DA28`, `sub_8258F490`, `sub_825A1810` and the movie library's. `Device::SetFVF` (`device-state.md`) sets +11552 itself, to the declaration it builds at +11936. Kept-kind (graphics): the renderer reads +11552 (`kDeviceVertexDeclaration`). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::SetVertexDeclaration` (`src/graphics/device_shaders.cpp`), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`). Harness `device_shaders`: 32 cases, 0 differ. |
| `sub_825915E0` | `RenderQueue::RenderQueue` | Overridden | r3 = the render queue (scene + 64; only caller the scene's constructor `sub_82098A98`, once at boot). +0 = vtable `0x8206CB8C`. The state: `operator new(88)` (`sub_825A6880`); when that isn't 0, `PrimitiveDrawer::PrimitiveDrawer` on state + 4 (`sub_825A1378`, `model-animation.md`) and state +72, +76, +80 = 0 (the vector); +4 = the state (or 0). +8 and +12: `operator new(36)` each, built by `PassList::PassList(list, 4096, 1024)` (`sub_82590B38`; 0 when the allocation fails). Then `PassList::Clear` on +8 and on +12 (`sub_8258EB38`, again after their constructors), and through +4: +0 = 0 (the replay index), +84 = 0.0 (`[0x82000A34]`), +48, +52, +56, +60, +64 = 0 (an allocation failure isn't checked here: the stores would go through 0). Returns nothing read. 91 lines, a 112-byte frame (r29-r31 saved). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::RenderQueueConstruct` in `src/graphics/device_shaders.cpp` (`operator new`'s plain C++, `PrimitiveDrawer::PrimitiveDrawer`'s and `PassList::PassList`'s, the 0.0 through a double as the original's `lfs`/`stfs`), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`, only with the native heap; the original from the start where an `operator new` would take its error path, never in play). Harness `device_shaders`: 4 cases (the recompiled one with the flag off, so its callees are recompiled too; compared by role: pointers set or not, the blocks' written words, the bucket links as offsets), 0 differ. |
| `sub_82590B38` | `PassList::PassList` | Overridden | r3 = a 36-byte pass list, r4 = an item count (4,096 from the render queue), r5 = a bucket count (1,024): +0 = vtable `0x8206CB88`, +4 = 1 (a byte), +8 = r4, +12 = r5, +16 = 0; +20 = `operator new[](r4 × 20)` (`sub_825A6940`; -1 asked for when r4 is above `0x0CCCCCCC`), +24 = `operator new[](r5 × 16)` (-1 above `0x0FFFFFFF`); `PassList::Clear` (`sub_8258EB38`); then each bucket's +8 = the next bucket's address and +12 = 0, the last bucket's +8 and +12 = 0. Returns r3. 2 callers, both in `RenderQueue::RenderQueue`. A 112-byte frame (r29-r31 saved). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::PassListConstruct` (`src/graphics/device_shaders.cpp`, `operator new[]`'s plain C++; a count of 0 would loop as the original's), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`, only with the native heap). Harness `device_shaders`: 48 cases (1 to 300 items, 1 to 64 buckets; by role, as the queue's), 0 differ. |
| `sub_8258EB38` | `PassList::Clear` | Overridden | r3 = a pass list: each bucket's (+12 of them at +24, 16 bytes each) +0 and +4 = 0; then +16, +28 and +32 = 0 (the bucket links at +8 and the items at +20 are left). Leaf, 25 lines. Callers seen in play: `PassList::PassList`, `RenderQueue::RenderQueue` (twice) and the queue's slot 2 `sub_8258ED78` (Wrapped, `functions.md`: every pass from `Scene::Update`, on the list being recorded, with the state's +84 sequence then set to 0.0). `../frame-interpolation.md` already noted it "empties a pass list". Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::PassListClear` (`src/graphics/device_shaders.cpp`), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`; it stays while the queue's slot 2 `sub_8258ED78`, Wrapped, calls it through `ctx`). Harness `device_shaders`: 64 cases (0 to 199 buckets), 0 differ. |
| `sub_82590C48` | `PassList::~PassList` (probably the deleting destructor, vtable `0x8206CB88` slot 0) | | Not seen in play. r3 = a pass list, r4 = flags: +0 = vtable `0x8206CB88`, frees +20 and +24 (`sub_825A71C0`), and the list itself when r4 bit 0 is set (`sub_825A70E8`); returns r3. Called by `RenderQueue::~RenderQueue` through the vtable. Read in full (B7 loop, 2026-11-01). |
| `sub_82590DE0` | `RenderQueue::~RenderQueue` (probably) | | Not seen in play. r3 = the render queue: +0 = vtable `0x8206CB8C`; the pass lists at +8 and +12 destroyed through their vtable's slot 0 with r4 = 1 when not 0; then the state (+4), when not 0: its vector's memory (+72) freed when not 0 (`sub_825A70E8`), +72, +76, +80 = 0, `sub_825A13C0` on state + 4 (probably `PrimitiveDrawer::~PrimitiveDrawer`, not read), and the state freed (`sub_825A70E8`). No direct callers (probably the queue's vtable). Read in full (B7 loop, 2026-11-01). |
