# Known functions: the device's shader setters and the render queue

The graphics device's side under the primitive drawer
(`PrimitiveDrawer::Init`, `SetShaders` and `Draw` in
`model-animation.md`, which these block): the graphics library's (the
360's Direct3D, linked into the game) calls that bind a vertex shader, a
pixel shader and a vertex declaration on the device, and the render
queue (scene + 64, vtable `0x8206CB8C`) whose constructor makes the
primitive drawer, with its two pass lists. Mapped by the B7 loop
(`../loops/b7-loop.md`) from its lead in `../loops/leads.md` (found by
the model animation's mapping agent, 2026-10-23), 2026-11-01. The render
queue's methods (its vtable's slots, the pass list's `Add`, the replay's
sort and the chunk walker under it) were added the same day by the B7
loop's mapping agent from their own lead (found by the device shaders'
mapping agent), read in our `generated/` code, with the vtables and
constants read from the image.

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
constructor `sub_82098A98` (a lead in `../loops/leads.md`). The game's
own name for it, in `RenderQueue::Add`'s warning, is `OT` (an ordering
table: `warning!!! : call OT::Add( %x, %u, %f )`). +0 the vtable
`0x8206CB8C` (ten slots, below), +4 its 88-byte state (+0 the replay
index, +4 the primitive drawer, +12 a field whose address the replay
publishes at `0x82970BBC` (probably the drawer's state, as
`../gpu-functions.md` calls it; not read further), +24 to
+36 four words the replay zeroes, +44 the video output the replay was
given, +48 to +60 render targets 0-3 and +64 the depth surface
(`RenderQueue::Init`), +68 the release list, a `std::vector` of object
pointers (its allocator, first, last and end at +72, +76, +80), +84 a
float sequence), +8 and +12 the two pass lists (36 bytes each: vtable
`0x8206CB88`, one slot, the destructor; +4 a byte, the replay's "bucket
done" flag, +8 4,096 and +12 1,024 (the item and bucket counts asked
for), +16 the items used, +20 4,096 items of 20 bytes (+0 the next item,
+4 the chunk, +8 the key, +12 the sequence; +16 never written here),
+24 1,024 16-byte buckets (+0 the first item, +4 the last, +12 a count
`PassList::Clear` never resets, which nothing here reads; +8 untouched
here), +28 zeroed by `Clear` and the replay, +32 the replay's current
item).

**How it's used.** The queue is double-buffered: the pass list being
recorded is +12 when the replay index is 0 and +8 when it's 1, the one
replayed the other (slots 7 and 8). Every pass, `Scene::Update`
(`sub_820988C8`, `functions.md`) flips the index (slot 6) and empties
the new recording list, the sequence put back to 0.0 (slot 2); the
recorders then hand each closed chunk of the scene's command list
(`command-list.md`) to slot 4 with a bucket (the layer, 0-1,023) and a
float key, which appends it to that bucket and moves the sequence on by
0.1. On the render thread, `Scene::Draw` (`sub_82098940`,
`functions.md`) calls slot 5 with the device and the video output: the
replay walks the list being replayed bucket by bucket, sorts each
bucket's entries by key (ascending) and, among equal keys, by sequence
(submission order), and runs each chunk's item chain
(`ChunkChain::Run`), whose kinds dispatch the render commands through
the tables `0x8265EDE0` (command lists, the `Command::*` handlers,
Wrapped in `functions.md` for the native renderer;
`../gpu-functions.md`'s command table) and `0x8265ED48` (single
commands, "Single commands" in `../gpu-functions.md`). It ends by
destroying what command 22 (`sub_82591798`, `functions.md`) put on the
release list during the replay. `RenderQueue::Init` (slot 0) runs once,
from the scene's constructor under the device lock. The callers of slot
4 seen in play are the recorders in `scenery-lists.md`, `mode-tick.md`,
`minimap.md` and `sprites.md` (the last through `[0x826C5D5C]`, set by
`Overlays::SetRenderTargets` in `race-overlays.md`), among others.

The vtable `0x8206CB8C` (read from the image; the pass list's
`0x8206CB88` holds only `PassList::~PassList` before it): 0
`RenderQueue::Init` `sub_8258ED00`, 1 `sub_8258E5C8` (not seen in play),
2 `RenderQueue::ClearRecording` `sub_8258ED78`, 3 the empty destructor
`sub_82106540` (`c-runtime.md`), 4 `RenderQueue::Add` `sub_8258EDC8`, 5
`RenderQueue::Replay` `sub_82591238`, 6 `RenderQueue::Flip`
`sub_8258E5E0`, 7 `RenderQueue::Recording` `sub_8258E5F8`, 8
`RenderQueue::Replaying` `sub_8258E618`, 9 the deleting destructor
`sub_82590EA0` (not seen in play).

Where the indirect calls go: `RenderQueue::Init` calls the drawer's
slot 0, `PrimitiveDrawer::Init` (`sub_825A1438`, recompiled,
`model-animation.md`), and slot 1 the drawer's slot 1 (`sub_825A1610`,
never ran); `ClearRecording` and `Add` call the queue's own slot 7; the
replay calls its own slot 8, the sort's key getters (`RigidBody::Mass`
`sub_820B6708`, a float at +4, Overridden in `vehicle-accessors.md`,
for the key; `QueueEntry::Sequence` `sub_8258E5C0` for the sequence),
the release list's objects' slot 0 with r4 = 1 (probably their deleting
destructors; which objects command 22 hands over isn't read), and,
through the chain walker, the two command tables (above).

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
`PassList::Clear`'s adapter went once `RenderQueue::ClearRecording`
was made native (below): it is Native.

**The render queue's methods, ours** (B7 loop, 2026-11-01): in
`src/graphics/render_queue.cpp` behind `src/hooks/render_queue.cpp`
(`--native_render_queue`, its own flag so the queue can be switched apart
from the setters; harness `render_queue`). `Flip` and `ClearRecording`
keep `--find_menu_selection`'s wrapper in their adapters, around both
paths. Calls to the queue's own slots go to their plain C++ when the
vtable holds their address (always in play), else through `ctx`; calls
into code still recompiled (the drawer's `Init`) are guest calls from a
frame made where the original makes its own.

For the migration of the render queue's methods (`b7_groups.py`,
2026-11-01): ready are `RenderQueue::Init`, `ClearRecording`, `Add`
(with `PassList::Add`), `Flip`, `Recording`, `Replaying`, `Replay`
(with the sort and the chunk walker's kinds), `QueueEntry::Sequence`
and `PtrVector::PushBack` (with `Insert`). Their indirect calls reach
code still recompiled: `PrimitiveDrawer::Init` (`Init`'s), the render
command handlers of both tables and the release list's objects
(`Replay`'s); the replay runs on the render thread, where the native
renderer's hooks (`src/native/replay_hooks.cpp`) see each command. The
slots 2 and 6 keep `--find_menu_selection`'s wrappers
(`src/debug/menu_selection_scan.cpp`). Not seen in play: slot 1, slot 9,
the walker's kinds 1 and 2, the debug print and the vector's subscript
error.

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
| `sub_822A8808` | `Device::SetVertexShader` | Native | Read in full (B7 loop, 2026-11-01): r3 = the device, r4 = a vertex shader or 0; a 128-byte frame (r28-r31 saved). `Resource::AddRef(r4)` when r4 isn't 0, then `Resource::Release` on the bound one (device +12436) when that isn't 0 (also when it's the same object, which the wrapper skips); +12436 = r4; device byte +10810 has its top bit (0x80) cleared (what it means isn't known); +16 ORed with 1 << 51. When r4 isn't 0 and its patch offset (r4 + 592 + 12) isn't 0, the patches at p = r4 + 592 + that offset: device +0 ORed with the doubleword p + 0 (the vertex float constants' dirty mask); when the doubleword p + 8 isn't 0, device +24 ORed with 2; then the records from p + 24 to p + 24 + the word at p + 16: first copy records (halfword +0 a byte offset into the register shadow at device +1024, halfword +2 a word count, then that many words, copied with `memcpy` `sub_825A7410`) until one with a count of 0; then masked records (the same header, then count / 2 pairs of words: shadow word = (shadow word AND first) OR second, one shadow word a pair) until another count of 0 or the end. Only caller `sub_822A8958` (its tail call). Kept-kind (graphics): the renderer reads +12436, the shadow and the dirty masks (`src/native/state.h`, `state.cpp`); a native one keeps the wrapper's skip (the count is unchanged either way). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::SetVertexShader` (`src/graphics/device_shaders.cpp`): `refcount::AddRef`'s plain C++, Release through its override (a guest call, the lock-free count; it may destroy the bound shader through `sub_822A6210`), the patches' copies through `native::Buffers_Memcpy` (memcpy through the guest where it refuses), r3 as the original leaves it; the wrapper's rebind skip kept (AddRef and Release not made for the shader already bound; r3 its count, as the wrapper's Release gave) with its 5-second count log (`refcount::NoteSetVertexShader`). Its only caller, `D3DDevice_SetVertexShader`'s adapter, calls the plain C++, so its override (the wrapper in `src/native/refcount.cpp`) is gone (B7 loop, 2026-11-01); `--native_device_shaders` switches only that caller, whose fallback runs the original with the wrapper's skip around it as before (`--refcount_global_lock` the original alone). Harness `device_shaders`: 200 cases (against `__imp__` without the skip: the count comes out the same), 0 differ. |
| `sub_822A8618` | `Device::SetPixelShader` (probably the library's `D3DDevice_SetPixelShader`) | Overridden | r3 = the device, r4 = a pixel shader or 0; a 128-byte frame (r28-r31 saved). Takes no reference (no `AddRef` or `Release`): +12432 = r4; +16 ORed with 1 << 52, then ORed with 1 << 49 (stored after each). When r4 isn't 0 and its patch offset (r4 + 56 + 12) isn't 0, the patches at p = r4 + 56 + that offset, as `Device::SetVertexShader`'s but into device +8 (the pixel float constants' dirty mask) instead of +0: +24 ORed with 2 when the doubleword p + 8 isn't 0, then the copy records (`memcpy`) and the masked records into the shadow at device +1024. 90 lines. Callers seen in play: command 9's handler `sub_8258FFA0`, `VideoOutput::UnbindAll` (with 0), `PrimitiveDrawer::SetShaders`; not seen: `sub_8229DA28`, `sub_8258F438`, `sub_825A1708` and the movie library's. Kept-kind (graphics): the renderer reads +12432 (`kDevicePixelShader`), the shadow and the dirty masks. Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::SetPixelShader` (`src/graphics/device_shaders.cpp`, the patches as `SetVertexShader`'s), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`). Harness `device_shaders`: 200 cases, 0 differ. |
| `sub_822A8960` | `Device::SetVertexDeclaration` (probably the library's `D3DDevice_SetVertexDeclaration`) | Overridden | r3 = the device, r4 = a vertex declaration (or 0): +11552 = r4, +16 ORed with 1 << 51 (the vertex shader's bit: the shader's microcode is patched for the declaration at the draw, `sub_822AE340`, `functions.md`). No reference taken. Leaf, 10 lines. Callers seen in play: command 10's handler `sub_8258FFF0`, `PrimitiveDrawer::Draw` (`sub_825A17A0`, with `[[drawer + 4]]`); not seen: `sub_8229DA28`, `sub_8258F490`, `sub_825A1810` and the movie library's. `Device::SetFVF` (`device-state.md`) sets +11552 itself, to the declaration it builds at +11936. Kept-kind (graphics): the renderer reads +11552 (`kDeviceVertexDeclaration`). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::SetVertexDeclaration` (`src/graphics/device_shaders.cpp`), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`). Harness `device_shaders`: 32 cases, 0 differ. |
| `sub_825915E0` | `RenderQueue::RenderQueue` | Overridden | r3 = the render queue (scene + 64; only caller the scene's constructor `sub_82098A98`, once at boot). +0 = vtable `0x8206CB8C`. The state: `operator new(88)` (`sub_825A6880`); when that isn't 0, `PrimitiveDrawer::PrimitiveDrawer` on state + 4 (`sub_825A1378`, `model-animation.md`) and state +72, +76, +80 = 0 (the vector); +4 = the state (or 0). +8 and +12: `operator new(36)` each, built by `PassList::PassList(list, 4096, 1024)` (`sub_82590B38`; 0 when the allocation fails). Then `PassList::Clear` on +8 and on +12 (`sub_8258EB38`, again after their constructors), and through +4: +0 = 0 (the replay index), +84 = 0.0 (`[0x82000A34]`), +48, +52, +56, +60, +64 = 0 (an allocation failure isn't checked here: the stores would go through 0). Returns nothing read. 91 lines, a 112-byte frame (r29-r31 saved). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::RenderQueueConstruct` in `src/graphics/device_shaders.cpp` (`operator new`'s plain C++, `PrimitiveDrawer::PrimitiveDrawer`'s and `PassList::PassList`'s, the 0.0 through a double as the original's `lfs`/`stfs`), adapter in `src/hooks/device_shaders.cpp` (`--native_device_shaders`, only with the native heap; the original from the start where an `operator new` would take its error path, never in play). Harness `device_shaders`: 4 cases (the recompiled one with the flag off, so its callees are recompiled too; compared by role: pointers set or not, the blocks' written words, the bucket links as offsets), 0 differ. |
| `sub_82590B38` | `PassList::PassList` | Native | r3 = a 36-byte pass list, r4 = an item count (4,096 from the render queue), r5 = a bucket count (1,024): +0 = vtable `0x8206CB88`, +4 = 1 (a byte), +8 = r4, +12 = r5, +16 = 0; +20 = `operator new[](r4 × 20)` (`sub_825A6940`; -1 asked for when r4 is above `0x0CCCCCCC`), +24 = `operator new[](r5 × 16)` (-1 above `0x0FFFFFFF`); `PassList::Clear` (`sub_8258EB38`); then each bucket's +8 = the next bucket's address and +12 = 0, the last bucket's +8 and +12 = 0. Returns r3. 2 callers, both in `RenderQueue::RenderQueue`. A 112-byte frame (r29-r31 saved). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::PassListConstruct` (`src/graphics/device_shaders.cpp`, `operator new[]`'s plain C++; a count of 0 would loop as the original's), called by `RenderQueue::RenderQueue`'s plain C++, its only caller; no adapter (removed, B7 loop, 2026-11-01): `--native_device_shaders` switches only that caller. Harness `device_shaders`: 48 cases (1 to 300 items, 1 to 64 buckets; by role, as the queue's), 0 differ. |
| `sub_8258EB38` | `PassList::Clear` | Native | r3 = a pass list: each bucket's (+12 of them at +24, 16 bytes each) +0 and +4 = 0; then +16, +28 and +32 = 0 (the bucket links at +8 and the items at +20 are left). Leaf, 25 lines. Callers seen in play: `PassList::PassList`, `RenderQueue::RenderQueue` (twice) and the queue's slot 2 `sub_8258ED78` (Wrapped, `functions.md`: every pass from `Scene::Update`, on the list being recorded, with the state's +84 sequence then set to 0.0). `../frame-interpolation.md` already noted it "empties a pass list". Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::device_shaders::PassListClear` (`src/graphics/device_shaders.cpp`), its adapter removed (B7 loop, 2026-11-01): its callers in play, `RenderQueue::RenderQueue` and `PassList::PassList` (`--native_device_shaders`) and `RenderQueue::ClearRecording` (`--native_render_queue`), call the plain C++, and their flags now switch only them. Harness `device_shaders`: 64 cases (0 to 199 buckets), 0 differ. |
| `sub_82590C48` | `PassList::~PassList` (probably the deleting destructor, vtable `0x8206CB88` slot 0) | | Not seen in play. r3 = a pass list, r4 = flags: +0 = vtable `0x8206CB88`, frees +20 and +24 (`sub_825A71C0`), and the list itself when r4 bit 0 is set (`sub_825A70E8`); returns r3. Called by `RenderQueue::~RenderQueue` through the vtable. Read in full (B7 loop, 2026-11-01). |
| `sub_82590DE0` | `RenderQueue::~RenderQueue` (probably) | | Not seen in play. r3 = the render queue: +0 = vtable `0x8206CB8C`; the pass lists at +8 and +12 destroyed through their vtable's slot 0 with r4 = 1 when not 0; then the state (+4), when not 0: its vector's memory (+72) freed when not 0 (`sub_825A70E8`), +72, +76, +80 = 0, `sub_825A13C0` on state + 4 (probably `PrimitiveDrawer::~PrimitiveDrawer`, not read), and the state freed (`sub_825A70E8`). No direct callers (probably the queue's vtable). Read in full (B7 loop, 2026-11-01). |
| `sub_8258ED00` | `RenderQueue::Init` | Overridden | Slot 0 of `0x8206CB8C` (no direct callers): r3 = the render queue, r4 = the device. Calls the drawer's (state + 4) vtable slot 0, `PrimitiveDrawer::Init` (`sub_825A1438`, recompiled, `model-animation.md`), with r3 = the drawer and r4 still the device; then state +48 + 4 × i = `Device::GetRenderTarget(device, i)` (`sub_822A3998`, `graphics-resources.md`, AddRef'ed) for i = 0 to 3, and state +64 = `Device::GetDepthStencilSurface(device)` (`sub_822A39E0`). A 128-byte frame (r27-r31 saved). Once at boot, from the scene's constructor `sub_82098A98` with the device `VideoOutput::LockDevice` returned. Seen in play. Read in full (B7 loop, 2026-11-01; it was in the graphics resources' users lead in `../loops/leads.md`, "reads the device's render target and depth surface"). Ours (B7 loop, 2026-11-01): `graphics::render_queue::Init` (`src/graphics/render_queue.cpp`), adapter in `src/hooks/render_queue.cpp` (`--native_render_queue`): the drawer's slot 0 a guest call from its 128-byte frame (return address `0x8258ED2C`), `GetRenderTarget` and `GetDepthStencilSurface` their plain C++ (`graphics-resources.md`). Harness `render_queue`: 24 cases (the drawer's slot 0 a stand-in, `RenderQueue::Flip`), 0 differ. |
| `sub_8258E5C8` | | | Not seen in play. Slot 1 of `0x8206CB8C` (no direct callers): r3 = the render queue: tail-calls the drawer's (state + 4) vtable slot 1 (`sub_825A1610`, never ran, `model-animation.md`) with r3 = the drawer, the other registers passed on. 6 instructions. Read in full (B7 loop, 2026-11-01). |
| `sub_8258ED78` | `RenderQueue::ClearRecording` | Overridden | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. Slot 2 of `0x8206CB8C` (no direct callers): r3 = the render queue: the list being recorded (its own slot 7, `RenderQueue::Recording`) emptied (`PassList::Clear` `sub_8258EB38`, above, with the list the slot returned), then state +84 (the sequence) = 0.0 (the float at `0x82000A34`). A 96-byte frame (r31 saved). Every pass, from `Scene::Update` (`sub_820988C8`, `functions.md`) after slot 6. Read in full (B7 loop, 2026-11-01; the row moved from `functions.md`). Ours (B7 loop, 2026-11-01): `graphics::render_queue::ClearRecording`, adapter in `src/hooks/render_queue.cpp` (`--native_render_queue`), which keeps `--find_menu_selection`'s wrapper around both paths (moved from `src/debug/menu_selection_scan.cpp`); the queue's slot 7 the plain C++ `Recording` when it's `0x8258E5F8` (always in play), else a guest call; `PassList::Clear` its plain C++. Harness `render_queue`: 48 cases (a fake vtable now and then), 0 differ. |
| `sub_8258EDC8` | `RenderQueue::Add` (the game's `OT::Add`) | Overridden | Slot 4 of `0x8206CB8C` (no direct callers): r3 = the render queue, r4 = a closed chunk (or 0), r5 = the bucket (the layer, unsigned), f1 = the key. When the bucket is above 2,056, or the key above 10,000.0 (`0x82002814`) or below -10,000.0 (`0x82001C00`; a NaN key passes), a warning: `DebugPrintf` (`sub_825A1318`, below) with `warning!!! : call OT::Add( %x, %u, %f )\n` (`0x8206CB38`), the chunk, the bucket and the key as a double; never ran. Nothing more when r4 is 0. Otherwise, in a 144-byte frame (r29-r31 and f31 saved): the doubleword at r1 + 80 = {the chunk, the key as a single}, at r1 + 88 = {the sequence (state +84), the frame's word at r1 + 92, a stack leftover `PassList::Add` doesn't read}; the list being recorded (its own slot 7) and `PassList::Add(list, r4 = r1 + 80's, r5 = r1 + 88's, r6 = the bucket)` (`sub_8258EB88`), its answer not read; then state +84 += 0.1 (`0x82001FBC`, single precision), whether the item fitted or not. Callers in play: the recorders in `scenery-lists.md`, `mode-tick.md`, `minimap.md`, `sprites.md` and others (through the vtable; some native callers make the guest call, `src/hooks/mode_tick.cpp`, `minimap.cpp`, `sprites.cpp`). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::render_queue::Add` (`src/graphics/render_queue.cpp`), adapter in `src/hooks/render_queue.cpp` (`--native_render_queue`; it stays: reached through the vtable): the warning's path (a bucket above 2,056 or a key past ±10,000.0) runs the original from the start; the frame's words at +80 to +91 written as the original's (+92 left), the queue's slot 7 the plain C++ `Recording` when it's `0x8258E5F8`, else a guest call. Harness `render_queue`: 96 cases (the warning's path, a chunk of 0, NaN and infinite keys, a fake vtable now and then; Add's frame words compared), 0 differ. |
| `sub_8258EB88` | `PassList::Add` | Native | r3 = a pass list, r4 = the doubleword {chunk, key}, r5 = {sequence, unused}, r6 = the bucket. No frame of its own: r4 and r5 are stored into its caller's frame at r1 + 32 and r1 + 40 (`RenderQueue::Add`'s; memory it leaves written) and read back from there as two words and two floats. Returns 0 when r6 isn't below the bucket count (+12) or the items used (+16) aren't below the item count (+8). Otherwise the item at +20 + 20 × (+16): +0 = 0, +4 = the chunk, +8 = the key, +12 = the sequence (+16 not written); appended to the bucket at +24 + 16 × r6 (its +0 = the item when that's 0, else the last item's (+4) +0 = the item; +4 = the item; +12 += 1); +16 += 1; returns 1. Leaf. Only caller `RenderQueue::Add`. Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::render_queue::PassListAdd`, no adapter (only `RenderQueue::Add`, whose plain C++ calls it); its stores at its caller's +32 and +40 kept. Harness `render_queue`: 64 cases, 0 differ. |
| `sub_82591238` | `RenderQueue::Replay` | | Slot 5 of `0x8206CB8C` (no direct callers): r3 = the render queue, r4 = the device, r5 = the video output. Once a frame on the render thread, from `Scene::Draw` (`sub_82098940`, `functions.md`). In a 192-byte frame: `[0x8296BD90]` = the device (where the command handlers find it, `../gpu-functions.md`), state +44 = r5, state +24, +28, +32, +36 = 0, `[0x82970BB8]` = state + 4 (the drawer), `[0x82970BBC]` = state + 12, `[0x82970BC0]` = state + 48 (the render targets `Init` kept), `[0x8296BD8C]` = state + 68 (the release list), which is emptied (`TextureProvider::Clear` `sub_82590D68`, `models.md`, shared). Then the list being replayed (its own slot 8); for each of its buckets (+12 of them, at +24): list +28 = 0, +32 = the bucket's first item, the byte +4 = 1 when there's none (else 0); the bucket's items (by their +0) put in the array at `0x8296BD98` as item + 4 (room for 5,000 before `0x82970BB8`, not checked; a list holds at most 4,096 items), +32 and +4 following along. With n entries: when n > 1 sorted by key (`RenderQueue::Partition` over 0 to n - 1, then `RenderQueue::QuickSort` on each side, with `RigidBody::Mass` `sub_820B6708` as the getter: the float at the entry's +4, the item's key; `vehicle-accessors.md`); then each run of equal keys (compared inline, `fcmpu`) longer than one sorted the same way by `QueueEntry::Sequence` (`sub_8258E5C0`); then `ChunkChain::Run(queue, the entry's chunk)` (`sub_8258EE98`) for each entry in order. After the buckets: the release list's size taken once; for each element (the vector read again from `[0x8296BD8C]` each time; `sub_825944B0`, the subscript error, when its first is 0 or the index isn't below its size, never), when not 0 its vtable slot 0 with r4 = 1 (probably its deleting destructor) and the element = 0 (the size left; the next replay's `Clear` empties it). Then `[0x8296BD90]` = 0. Read in full (B7 loop, 2026-11-01). |
| `sub_8258EC38` | `RenderQueue::Partition` | | Quicksort's partition over an array of pointers: r3 = the array, r4 = lo, r5 = hi, r6 = a key getter (r3 = an element, returns f1). The pivot is a[hi]; i from lo - 1 up while key(a[i]) < the pivot's key (no bound: the pivot stops it), j from hi down while j > i and the pivot's key < key(a[j]); when j > i, a[i] and a[j] swapped and both scans go on (the pivot's key asked again each round); at the end a[i] and a[hi] swapped; returns i. A 160-byte frame (r24-r31 and f31 saved). From `RenderQueue::Replay` (twice) and `RenderQueue::QuickSort`. Read in full (B7 loop, 2026-11-01). |
| `sub_82590CB0` | `RenderQueue::QuickSort` | | r3 = the array, r4 = lo, r5 = hi, r6 = the key getter: while lo < hi, p = `RenderQueue::Partition(array, lo, hi, getter)`, `QuickSort(array, lo, p - 1, getter)` and lo = p + 1 (recursive on the left, a loop on the right). A 128-byte frame. From `RenderQueue::Replay` (four calls: each side of its two partitions) and itself. Read in full (B7 loop, 2026-11-01). |
| `sub_8258E5C0` | `QueueEntry::Sequence` | Overridden | r3 = a queue entry (an item + 4): returns the float at +8 (the item's sequence). Leaf, 2 instructions. Reached only through its address (`0x8258E5C0`), the replay's sort getter for equal keys. Seen in play. Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::render_queue::Sequence`, adapter in `src/hooks/render_queue.cpp` (`--native_render_queue`; it stays: reached through its address). Harness `render_queue`: 32 cases (NaNs and infinities among them), 0 differ. |
| `sub_8258EE98` | `ChunkChain::Run` (`../gpu-functions.md`'s chain walker) | | r3 = the render queue (passed on, not read by the kinds), r4 = an item (a chunk). While r4 isn't 0: the kind is the first word's low halfword, sign-extended (bytes +2 and +3; `../gpu-functions.md` said the first halfword); kinds 0-5 go to the six walkers below (a jump table at `0x8258EEDC`, recompiled as a switch), each returning the next item (r4); kind 6 ends the walk; any other kind (above 6 unsigned) loops on the same item for ever (never seen). A 96-byte frame (r31 saved). Only caller `RenderQueue::Replay`, once an entry. Read in full (B7 loop, 2026-11-01). |
| `sub_8258E630` | `ChunkChain::RunSingles` (kind 0) | | r4 = the item: +8 the count of single commands; slot i (from 0) at item + 16 × (i + 1), its handler from the table `0x8265ED48` by the slot's +0 (the id; "Single commands" in `../gpu-functions.md`), called with r3 = the slot; i += its answer + 1 while below the count. Returns 0 (the chain ends). A 128-byte frame. From `ChunkChain::Run`. Read in full (B7 loop, 2026-11-01). |
| `sub_8258E698` | `ChunkChain::RunInline` (kind 1) | | Not seen in play. r4 = the item: commands inline from item + 16 ("Command format" in `../gpu-functions.md`): each slot's handler from the table `0x8265EDE0` by its +0 (the id), with r3 = the payload (slot + 16), r4 = the slot's +4, r5 = its +8; the next slot (answer + 1) × 16 on; until id 36. Returns the slot after the end one (the next item follows inline). A 112-byte frame. From `ChunkChain::Run`. Read in full (B7 loop, 2026-11-01). |
| `sub_8258E718` | `ChunkChain::RunInlineLinked` (kind 2) | | Not seen in play. As kind 1 (commands inline from item + 16 through `0x8265EDE0`, until id 36), but returns the item's +4 (the next item by pointer). From `ChunkChain::Run`. Read in full (B7 loop, 2026-11-01). |
| `sub_8258E780` | `ChunkChain::RunOutOfLine` (kind 3) | | r4 = the item: the commands at the item's +4 (through `0x8265EDE0`, until id 36, as kind 1); returns item + 16 (the next item inline). A 112-byte frame. From `ChunkChain::Run`. Read in full (B7 loop, 2026-11-01). |
| `sub_8258E7E8` | `ChunkChain::RunOutOfLineLast` (kind 4) | | r4 = the item: the commands at the item's +4 (as kind 3); returns 0 (the chain ends). A 112-byte frame. From `ChunkChain::Run`. Read in full (B7 loop, 2026-11-01). |
| `sub_8258E868` | `ChunkChain::RunInlineLast` (kind 5) | | r4 = the item: commands inline from item + 16 (as kind 1); returns 0 (the chain ends). A 112-byte frame. From `ChunkChain::Run`. Read in full (B7 loop, 2026-11-01). |
| `sub_8258E5E0` | `RenderQueue::Flip` | Overridden | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. Slot 6 of `0x8206CB8C` (no direct callers): r3 = the render queue: the state's +0 (the replay index) = 1 when it was 0, else 0. Leaf, 5 instructions. Every pass, from `Scene::Update` (`sub_820988C8`, `functions.md`), before slot 2. Read in full (B7 loop, 2026-11-01; the row moved from `functions.md`). Ours (B7 loop, 2026-11-01): `graphics::render_queue::Flip`, adapter in `src/hooks/render_queue.cpp` (`--native_render_queue`), which keeps `--find_menu_selection`'s wrapper around both paths (moved from `src/debug/menu_selection_scan.cpp`). Harness `render_queue`: 32 cases, 0 differ. |
| `sub_8258E5F8` | `RenderQueue::Recording` | Overridden | Slot 7 of `0x8206CB8C` (no direct callers): r3 = the render queue: returns the pass list being recorded, the word at r3 + 4 × (2 + (index == 0)) (+12 when the replay index, state +0, is 0, else +8). Leaf, 7 instructions. From `RenderQueue::ClearRecording` and `Add` through the vtable (other callers not traced). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::render_queue::Recording`, adapter in `src/hooks/render_queue.cpp` (`--native_render_queue`; it stays: reached through the vtable). Native callers call the plain C++ when the slot is its address. Harness `render_queue`: 32 cases, 0 differ. |
| `sub_8258E618` | `RenderQueue::Replaying` | Overridden | Slot 8 of `0x8206CB8C` (no direct callers): r3 = the render queue: returns the pass list being replayed, the word at r3 + 4 × (2 + index) (+8 for 0, +12 for 1). Leaf, 5 instructions. From `RenderQueue::Replay` through the vtable (other callers not traced). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::render_queue::Replaying`, adapter in `src/hooks/render_queue.cpp` (`--native_render_queue`; it stays: reached through the vtable). Harness `render_queue`: 32 cases, 0 differ. |
| `sub_82590EA0` | `RenderQueue::~RenderQueue` (probably the deleting destructor) | | Not seen in play. Slot 9 of `0x8206CB8C` (no direct callers): r3 = the render queue, r4 = flags: `RenderQueue::~RenderQueue` (`sub_82590DE0`, above), then the queue freed (`sub_825A70E8`, `c-runtime.md`) when r4 bit 0 is set; returns r3. Read in full (B7 loop, 2026-11-01). |
| `sub_82591540` | `PtrVector::PushBack` | Overridden | `std::vector<T*>::push_back`: r3 = the vector (+4 first, +8 last, +12 end), r4 = the value's address: with room (first not 0 and the size below the capacity) the value stored at last and +8 moved on by 4; otherwise `PtrVector::Insert(r1 + 80, vector, last, value address)` (`sub_825911C8`). A 96-byte frame. From command 22 (`sub_82591798`, `functions.md`; the release list `[0x8296BD8C]`, `../gpu-functions.md`) and the single commands' 22 (`sub_825916F0`, not seen in play). Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::render_queue::PushBack` (`src/graphics/render_queue.cpp`), adapter in `src/hooks/render_queue.cpp` (`--native_render_queue`): the path that inserts natively only while `hooks::ModelsNativeLifetime()` (`TextureProvider::InsertN` allocates), else and where InsertN refuses the original from the start. Not run in a 40-second attract race (the coverage list saw it elsewhere). Harness `render_queue`: 64 cases (with room, empty, full), 0 differ. |
| `sub_825911C8` | `PtrVector::Insert` | Native | `std::vector<T*>::insert(where, value)`: r3 = where the answer goes, r4 = the vector, r5 = where, r6 = the value's address: the index (where - first, 0 for an empty vector), `TextureProvider::InsertN(vector, where, 1, value)` (`sub_82590EF0`, `models.md`, shared), then [r3] = first + 4 × index (the new element). A 112-byte frame. Only caller `PtrVector::PushBack`. Read in full (B7 loop, 2026-11-01). Ours (B7 loop, 2026-11-01): `graphics::render_queue::PtrVectorInsert`, no adapter (only `PtrVector::PushBack`, whose plain C++ calls it); `TextureProvider::InsertN`'s plain C++ (`models.md`). Harness `render_queue`, through `PushBack`'s empty and full cases, 0 differ. |
| `sub_825A1318` | `DebugPrintf` (probably) | | Not seen in play. r3 = a format, r4-r10 its arguments (stored into its caller's frame at r1 + 24 to + 79 before its own 4,208-byte frame, with a stack probe): formats into a buffer at r1 + 96 (`sub_825A71B0`, probably `vsprintf`, not read) and passes it to `OutputDebugStringA` (`sub_82296270`, `functions.md`, which drops it in the retail game). From `RenderQueue::Add`'s warning and the command list's overflow and nesting checks (`command-list.md`). Read in full (B7 loop, 2026-11-01). |
| `sub_825944B0` | `std::_Xran` (probably) | | Not seen in play. Builds a string from `invalid vector<T> subscript` (`0x8200220C`, 27 characters; `sub_82091808`, `sub_820919B8`, not read), probably a `std::out_of_range`, and throws it. Not read in detail. From `RenderQueue::Replay`'s release list loop (its check can't fail there) and the texture provider (`scenery-lists.md`, which called it the provider's resize). |
