# Known functions: shader objects and memory blocks

The graphics library's shader objects and the game's memory blocks they
are made in. A shader file (`../known-data.md`, "shader file") is made
into an object in two ways. The model and boot path (`Mesh::LoadShaders`
`sub_82594D20` and `Shaders::LoadAll`'s callback `sub_820A06D0`, both in
`models.md`) parses the file (`sub_822A8568`), takes a block for the
object (`Memory::AllocVirtual`), copies the file's header after the
object's fields (`sub_822A9FD8` for a vertex shader, 592 bytes of
fields; `sub_822A9EA8` for a pixel shader, 56; both in `models.md`),
copies the microcode into physical memory (`Memory::AllocPhysicalEx`,
then `memcpy`) and sets the object's fields (`VertexShader::Init`,
`PixelShader::Init`). The other path is the library's own create calls
(`VertexShader::Create`, `PixelShader::Create`, called by the font, the
primitive drawer and the app's set-up) that do the same with
`XMemAlloc`. Both copy the header with XAPI's vectorized copy
(`XMemCpy` `sub_82295890` and its block copy `sub_82296FE0`, shared:
their rows are in `c-runtime.md`, beside `XMemSet`).

The memory blocks (`../known-data.md`, "memory block") are allocations
with a 4 KiB `XLMM` header in front, taken under the critical section
`0x8296BD38` through `ScopedLock` (`video-output.md`). Three makers set
the header's kind: 0 `Memory::AllocVirtual` (`VirtualAlloc`), 1
`Memory::AllocPhysicalEx` (`XPhysicalAllocEx`), 2 `sub_8258E138`
(`XMemAlloc`; its row is in `models.md`). They are freed one pass late:
`Memory::FreeLater` and `FreePhysicalLater` (`models.md`, native) queue
the header on the pending frees (`0x8296BD54`), and
`PendingFrees::FreeDue` frees it at a later pass's start by its kind.
By the user's call of 2026-10-25 (`models.md`, followed here), the lock
is a host mutex for every user of it (`models::MemoryBlocksMutex` in
`src/models/memory_blocks.h`): the native versions of these take that
mutex directly. None of these functions calls a kernel import directly.
The kernel's work under them is done by functions that are already ours:
`VirtualAlloc`, `VirtualFree`, `XPhysicalAllocEx`, `XPhysicalFree`,
`XMemAlloc` and `XMemFree` (`heap.md`).

The graphics rule (native if the renderer needs it, else Dropped):
the shader objects are kept-kind, since the renderer reads them to find
a draw's shaders ("Shader lookup" in `../native-renderer.md`), and so
are the microcode and the copied fetch instructions.

These rows are the B7 loop's lead "the shader objects' set-up" from
`../loops/leads.md` (`../loops/b7-loop.md`), mapped 2026-10-25 by its
mapping agent from our `generated/` code, so that `Model::Load`
(`sub_82596920`, `models.md`) can be made ready. Its subtree (20
functions left in play) now has rows: these, the two `XMemCpy` rows,
and the rest in `models.md`. Ready now: `VertexShader::Init`,
`PixelShader::Init`, `XMemCpy` (with its block copy) and the three
memory block functions, beside `models.md`'s `sub_822A8568`,
`sub_8258E138`, `TextureHeader::Relocate` (with `sub_822A5B68`) and
`sub_8259A9E0` (with `sub_825A21A8`). Once those are ours, the next
layer becomes ready: `sub_822A9FD8`, `sub_822A9EA8`, the two create
calls, `sub_8235A9F8`, `sub_82599A20` and `XPR::Create`. After that come
`Mesh::LoadShaders`, `Mesh::Parse` and `Model::Load`. `Model::Load` also
has an indirect call (its texture provider's slot 4). Its unseen
callees are debug prints (`sub_825A1318`, `sub_825ABFE8`).

Made native 2026-10-25 by the B7 loop's migration (all seven rows, the
two `XMemCpy` rows and the chain up to `Model::Load` in `models.md`):
plain C++ in `src/shader_objects/` (the shader objects, the shader file
parse, `Mesh::LoadShaders`), `src/models/memory_blocks.cpp` (the memory
blocks), `src/models/xpr.cpp` and `model_load.cpp` (`XPR::Create`,
`Mesh::Parse`, `Model::Load`), behind `--native_shader_objects` for this
file's rows and `--native_models` for `models.md`'s; harness
`shader_objects`. The copies these make with the library's `memcpy`
(`sub_825A7410`) go through `native::Buffers_Memcpy`
(`src/native/buffers.cpp`): the same native copy and the same work
memcpy's override does around it (copies into the physical views, the
microcode and the vertex data, recorded for the renderer's mirrors and
texture cache).

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

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_822AA018` | `VertexShader::Create` (probably the library's `CreateVertexShader`) | Overridden | r3 = a shader file (no other argument read). The object (`XMemAlloc(56 + 536 + the header size, 0x24800000)`, the 536 when the magic's low 6 bits are set, as `sub_822A8568` works it out) is cleared for 592 bytes (`memset` `sub_825A71D0`). The file's header (as many bytes as the file's +4 gives) is copied to +592 (`XMemCpy`). The microcode (the file's +8 bytes, from the file + the header size) goes into `XMemAlloc(size, 0xB5800000)`, physical, through `memcpy` `sub_825A7410`. If that allocation fails, the object is freed (`XMemFree(object, 0x24800000)`) and 0 is returned. Then `VertexShader::Init(object, microcode)` runs, +0 gets bit `0x00100000` (made by the library, as `Device::CreateSurface` sets it, `graphics-resources.md`), and the object is returned (0 when the first allocation fails). From `Font::CreateShaders` (`strings-and-font.md`), `PrimitiveDrawer::Init` (`model-animation.md`), `sub_8209BE88` (the app's set-up, under the device lock) and `sub_824846D8` (`functions.md`). Not seen how often; at set-up only, probably. Waits on `XMemCpy` and `VertexShader::Init`. Ours (B7 loop, 2026-10-25): `shader_objects::VertexShaderCreate` (`src/shader_objects/shader_objects.cpp`) behind the adapter in `src/hooks/shader_objects.cpp` (`--native_shader_objects`, with `--native_heap`; falling back to the original), the allocations through `memory::crt::XMemAlloc` and `XMemFree` (the user's call of 2026-10-22), the header through `XMemCpy`'s plain C++, and the microcode copy as memcpy's override makes it (`native::Buffers_Memcpy`, new in `src/native/buffers.cpp`: the override's work around a copy for native callers, the copy into the physical views recorded for the renderer's mirrors and texture cache, return address `0x822AA0C0` in its logs). `VertexShader::Init` is its plain C++; where that declines (never in play), its original is a guest call from the adapter. A header copy `XMemCpy` would hand to the original, or `--native_memcpy` off, sends the whole call to the original before anything is done. Harness `shader_objects`: 150 cases (headers of 280-1,000 bytes with 0-32 fetches, and 40, microcode of 12 bytes to 6 KB), the same object, microcode and r3. Runs at boot. |
| `sub_822A9EE8` | `PixelShader::Create` (probably the library's `CreatePixelShader`) | Overridden | As `VertexShader::Create` with 56 bytes of fields: the object cleared for 56 bytes, the header copied to +56 (`XMemCpy`), the microcode into `XMemAlloc(size, 0xB5800000)` through `memcpy`. Then the 56 bytes are cleared again and `PixelShader::Init`'s fields are set inline (it isn't called): +0 = 7, +4 = 1, +20 = `0xFFFF0000`, +24 = the microcode. +0 gets bit `0x00100000`, and the object is returned (0 on either allocation's failure, the object then freed with `XMemFree(object, 0x24800000)`). From the same four callers (`sub_824846D8` twice). Waits on `XMemCpy`. Ours (B7 loop, 2026-10-25): `shader_objects::PixelShaderCreate` (`src/shader_objects/shader_objects.cpp`), as `VertexShader::Create`'s (`--native_shader_objects`, with `--native_heap`; the microcode copy's return address `0x822A9F90`). Harness `shader_objects`: 150 cases (magics with and without the low bits), the same. Runs at boot. |
| `sub_822A8740` | `VertexShader::Init` | Native | r3 = a vertex shader object whose file header has already been copied to +592 (`sub_822A9FD8`, `models.md`), r4 = its microcode (physical memory). Clears the 592 bytes (`memset`), then sets +0 = 6 (the type), +4 = 1 (the reference count), +20 = `0xFFFF0000`, +24 = -1 and +48 = r4. Its table: h = the object + 592 + the header's +20 (`[object + 612]`), +60 = h's +28 (the count), and the words at h + 4 × (h's +24 + 10). For each word i: +64 + 4i = the word, and the 12 bytes at r4 + 12 × (the word's low 12 bits) are copied to +192 + 12i (`memcpy` `sub_825A7410`, skipped when the two addresses are the same). These are probably the vertex fetch instructions (one 12-byte Xenos instruction each, at most 32 by the room: 64 + 4 × 32 = 192, 192 + 12 × 32 = 576), kept for patching by the vertex declaration. No return value. From `Mesh::LoadShaders`, `sub_820A06D0` (`models.md`) and `VertexShader::Create`. Leaf apart from `memset` and `memcpy` (both ours). Ours (B7 loop, 2026-10-25): `shader_objects::VertexShaderInit` (`src/shader_objects/shader_objects.cpp`) behind the adapter in `src/hooks/shader_objects.cpp` (`--native_shader_objects`, falling back to the original); the memset as a host memset and the 12-byte copies as host copies (the object outside the physical views, so memcpy's override would record nothing for the renderer). Where the original's order would decide (a count over 32, the fetch table or a copy's source sharing memory with the 592 bytes, an object in the physical views; none seen in play) it hands back to the original before writing anything. Harness `shader_objects`: 600 cases (counts 0-32, microcode in virtual and physical memory, the fallbacks), the same. Runs at boot (the stored shaders, `sub_820A06D0`) and in race loads. Native since 2026-10-31 (the adapter removed, B7 loop): every caller seen in play is ours (`Mesh::LoadShaders`, `Shaders::OnPairLoaded` `sub_820A06D0` and `VertexShader::Create`'s plain C++ call `VertexShaderInit` directly, with the original as a guest call where it declines); a counter showed the adapter not called in an attract race; `--native_shader_objects` no longer switches it (its callers' flags do: `--native_models` and, for the create, `--native_shader_objects`). |
| `sub_822A85B8` | `PixelShader::Init` | Native | r3 = a pixel shader object (its file header copied to +56 by `sub_822A9EA8`, `models.md`), r4 = its microcode (physical memory). Clears 56 bytes (`memset`), then sets +0 = 7 (the type), +4 = 1 (the reference count), +20 = `0xFFFF0000` and +24 = r4. No return value. From `Mesh::LoadShaders` and `sub_820A06D0` (`models.md`); `PixelShader::Create` does the same inline. Ours (B7 loop, 2026-10-25): `shader_objects::PixelShaderInit` (`src/shader_objects/shader_objects.cpp`) behind the adapter in `src/hooks/shader_objects.cpp` (`--native_shader_objects`). Harness `shader_objects`: 200 cases, the same. Runs at boot and in race loads. Native since 2026-10-31 (the adapter removed, B7 loop): every caller seen in play is ours (`Mesh::LoadShaders` and `Shaders::OnPairLoaded` `sub_820A06D0` call `PixelShaderInit` directly); a counter showed the adapter not called in an attract race; `--native_shader_objects` no longer switches it (its callers' flag, `--native_models`, does). |
| `sub_8258DFF0` | `Memory::AllocVirtual` | Overridden | r3 = bytes. Under the memory blocks' lock (`ScopedLock` over `0x8296BD38`; the host mutex since 2026-10-25, see the intro) it calls `VirtualAlloc(0, (r3 + 8191) & ~0xFFF, 0x801000, 4)` (`heap.md`: MEM_COMMIT with MEM_NOZERO, read and write). The 4 KiB header gets +0 `XLMM` (`0x584C4D4D`), +4 = 0 (the kind), +8 = 1 (the count down to the free), +12 = 0. Returns the block + 4096, or 0 (the lock is let go on both paths). From `Mesh::LoadShaders` (the shader objects), `Mesh::Parse`, `XPR::Create` (the header block), `sub_820A06D0` (`models.md`) and `sub_82589E78` (the scene's constructor lead in `../loops/leads.md`). Freed by `Memory::FreeLater` (`models.md`). Ours (B7 loop, 2026-10-25): `models::AllocVirtual` (`src/models/memory_blocks.cpp`, beside the lock) behind the adapter in `src/hooks/shader_objects.cpp` (`--native_shader_objects`, with `--native_heap` and the memory blocks' host lock, as the model class's lifetimes; falling back to the original), under `models::MemoryBlocksMutex` (the user's call of 2026-10-25), the pages from the allocation layer (`memory::AllocPages`, as VirtualAlloc's adapter gives them; the user's call of 2026-10-22). Harness `shader_objects`: 10 sizes, the same header and page offset. Runs at boot and in race loads. |
| `sub_8258E098` | `Memory::AllocPhysicalEx` | Overridden | r3 = bytes, r4 to r7 passed to `XPhysicalAllocEx` (`sub_82296550`, `heap.md`: the lowest and highest address, the alignment and the protection) with the size `(r3 + 8191) & ~0xFFF`, under the memory blocks' lock as `Memory::AllocVirtual`. The header gets +0 `XLMM`, +4 = 1 (the kind), +8 = 1, +12 = 0. Returns the block + 4096, or 0. The calls seen: `(size, 0, -1, 32, 0x404)` for the shaders' microcode (`Mesh::LoadShaders`, `sub_820A06D0`, both twice), `(size, 0, -1, 4, 0x404)` for `Mesh::Parse`'s vertex data, `(size, 0, r30, 4, 4)` from `sub_82589E78`, and `(512, 0, r31, 0, 4)` and `(3 MiB, 0, r31, 2048, 4)` from `sub_82110258` (X3DAudio's lead in `../loops/leads.md`). Freed by `Memory::FreeLater` (`models.md`). Ours (B7 loop, 2026-10-25): `models::AllocPhysicalEx` (`src/models/memory_blocks.cpp`) behind the adapter in `src/hooks/shader_objects.cpp` (`--native_shader_objects`, with `--native_heap` and the host lock), under `models::MemoryBlocksMutex`, through `memory::crt::XPhysicalAllocEx` (`src/memory/c_runtime_heap.cpp`, new: XPhysicalAllocEx's adapter as plain C++, the large pages cleared when `[0x829537B0]` isn't 0). Harness `shader_objects`: 60 cases (10 sizes, the calls seen and large pages), the same. Runs at boot and in race loads. |
| `sub_8258E1D8` | `PendingFrees::FreeDue` | Overridden | No arguments. Under the memory blocks' lock it walks the pending frees (the `std::list` at `0x8296BD54`, `../known-data.md`) from the first node. For a header whose +8 is 0, +0 is cleared (the `XLMM` mark) and the header is freed by its kind: 0 `VirtualFree(header, 0, 0x8000)` (`sub_82295330`), 1 `XPhysicalFree(header)` (`sub_822965C8`), 2 `XMemFree(header, +12)` (`sub_82297790`), 3 and above nothing. The node is then unlinked (`prev->next = next`, `next->prev = prev`; a check against the list's head node never holds) and freed (`operator delete` `sub_825A70E8`), and the count (+8) goes down by 1. Any other header's +8 goes down by 1 and it stays. Since the makers set +8 = 1, a block queued by `Memory::FreeLater` is freed at the second pass start after it. From `FrameStart::OnTick` (`framework.md`), every pass. Every callee is ours (`heap.md`); `FrameStart::OnTick` waits on it. Ours (B7 loop, 2026-10-25): `models::FreeDue` (`src/models/memory_blocks.cpp`) behind the adapter in `src/hooks/shader_objects.cpp` (`--native_shader_objects`, with `--native_heap` and the host lock), under `models::MemoryBlocksMutex`; the frees through the allocation layer (`memory::FreePages`, `FreePhysical`, `memory::crt::XMemFree`, `OperatorDelete`), as their adapters do. A due block the layer can't free natively (kind 0 not its pages, kind 2 not physical and not a heap block), a node not a heap block, or a header queued twice (none seen in play) hands the whole call to the original first. Harness `shader_objects`: 120 cases on a list of the case's own swapped in for the game's under the lock (0 to 11 blocks of kinds 0-5, 0 to 2 passes left), the same. Runs every pass. |
