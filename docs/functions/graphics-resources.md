# Known functions: graphics resources

The graphics library's resources as seen in play (the 360's Direct3D,
linked into the game, with the XGraphics helpers beside it): texture and
surface headers (the resource's common word at +0, its reference count
at +4, its fences at +8 and +12, its locked ranges at +20 and +24, and
for textures the GPU's texture fetch constant at +28 to +48), what's
worked out from them (a format's block size, a texture's format, type,
base size, mip levels, a level's size, layout and offset, a level's
description), making textures, render target and depth surfaces (with
their EDRAM tiles), a texture's surface levels, vertex and index buffers,
the device's current render target, depth surface and stream source,
locking and unlocking resources, and their reference counts and
destruction. The layouts are in `../known-data.md` ("texture header",
"surface header", "buffer header", "level description"); how the game
drives the GPU is in `../gpu-functions.md`. Mapped by the B7 loop
(`../loops/b7-loop.md`) from its lead in `../loops/leads.md` (the
texture description `sub_822A5B78`), 2026-10-24.

Graphics: what our Vulkan renderer needs stays native, the rest is
Dropped (the user's rule). The rows say which kind each looks like; none
is decided here. Kept-kind: the format math and descriptions (the game
reads their answers: the font keeps its page texture's width and
height, `sub_8211BEE8` its quad's texture's), the headers (the renderer reads the fetch constants and the
surfaces' EDRAM bases, `src/native/passes.cpp`), the memory made for
textures and buffers (the game writes it, the renderer reads it) and the
lock's address. Dropped-kind, probably: the GPU waits and cache work
around locks, unlocks and destruction (`Resource::Lock`'s fence wait and
its cache flush packet, which the renderer skips, `src/native/gpu_link.cpp`;
`Resource::Unlock`'s data cache flush; the destroy's fence waits). The
EDRAM allocator holds the kernel's critical section
(`RtlEnterCriticalSection` and `RtlLeaveCriticalSection`).

Callers in other files: `Texture::GetLevelDesc` (`sub_822A5B78`) from
`UiMovie::OnLoaded` and `sub_8211BEE8` (`ui-movies-game.md`),
`Font::OnPageLoaded` (`strings-and-font.md`), `sub_822135B0`,
`sub_82228E88`, `sub_82228FC8` and others (a lead); the textures and
surfaces made by `CameraPasses::MakeRenderTarget`, its case 0
`sub_8209FC7C` and `CameraPass::SetUpSurfaces` with its chunk
`sub_820A2F94` (`cameras.md`), `Device::SetUpDisplay`,
`VideoOutput::SetUpDisplay` and `sub_82482B18` (`functions.md`), the
vinyl sheet's (`Texture::Create` and `Vinyl::Compress`, `car-models.md`);
`Device::Resolve` (`sub_822AFA58`, `functions.md`) calls
`Texture::LevelOffset`. Shared: `sub_822A5B68` (a texture header's mip
level count) and the XPR2's relocations `sub_8235A9F8` and
`TextureHeader::Relocate` (`sub_8235A990`) are in `models.md`;
`XMemAlloc` and `XMemFree` (`sub_822976F8`, `sub_82297790`) in
`heap.md`; the command buffer's wait and segment end (`sub_8229E908`,
`sub_8229EF48`) and the data cache flush `sub_822AC470` in
`functions.md`. The device's state setters beside these
(`sub_822A3450` to `sub_822A4340`, `sub_822A3758`, `sub_822A3908`,
`sub_822A6008`) are another subsystem (a lead), as is the gamma's
`sub_822A3A20` (`functions.md`). Not in play, so no rows: the cube
map's surface level `sub_822A5C50` (`GetSurfaceLevel` with a face),
the packed mip tail's offset `sub_822A4A40`, the EDRAM free
`sub_822B5EA8`, and the gamma's `sub_822A3AA8`.

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
| `sub_822A5B78` | `Texture::GetLevelDesc` | | Jumps to `Texture::LevelDesc` (r3 = the texture, r4 = the level, r5 = the 32-byte level description: its width and height at +24, +28). From `UiMovie::OnLoaded`, `sub_8211BEE8`, `Font::OnPageLoaded`, `sub_822135B0`, `sub_82228E88`, `sub_82228FC8`, and `sub_822116A0`, `sub_8218D8E8` (not seen in play). Kept-kind. |
| `sub_822A4F60` | `Texture::LevelDesc` | | r3 = a texture header, r4 = the level, r5 = the description (`D3DSURFACE_DESC`, probably): +0 the format (`Texture::Format`), +4 the type (`Resource::GetType`), +8, +12, +16, +20 = 0, +24 the width and +28 the height (`Texture::LevelLayout`'s). From `Texture::GetLevelDesc` and `Surface::GetDesc`. Kept-kind. |
| `sub_822A5F50` | `Surface::GetDesc` | | r3 = a surface header, r4 = the description: a texture's level (+0 bit `0x40000000`) asks `Texture::LevelDesc(parent +24, level (+28 >> 28))`; a render target or depth surface fills it from its own header: +0 the format (+40), +8, +12, +20 = 0, +24 the width ((+36 >> 18 & 0x3FFF) + 1), +28 the height ((+36 >> 3 & 0x7FFF) + 1), +16 the multisample type (+24's low 2 bits). +4 = 4 (a surface) either way. From `Vinyl::Compress` (`car-models.md`). Kept-kind. |
| `sub_822A6180` | `Resource::GetType` | Overridden | r3 = a resource header; returns its type from +0's low 4 bits: a texture (3) by its fetch constant's dimension (+48 bits 9-10): 18 cube, 17 volume, 20 one-dimensional, 19 an array (2D with +32 bit `0x400`), else 3; a surface (4) with +0 bit `0x40000000` whose parent's (+24) dimension is 3D is 16 (a volume's slice); anything else its low 4 bits. Leaf. Probably D3D's `D3DRTYPE` values. From `Texture::LevelDesc`, `Texture::LevelLayout`, `Texture::LevelOffset` and `sub_8235A9F8` (`models.md`). Kept-kind. Native in `src/graphics/resources.cpp` behind an adapter (`src/hooks/graphics_resources.cpp`, `--native_graphics_resources`); harness `graphics_resources`. |
| `sub_822A44A8` | `Texture::Format` | Overridden | r3 = a texture header; returns its format word (a `D3DFORMAT`, probably), rebuilt from the fetch constant: +32's low bits (the GPU format, the low 6 bits kept), +28's bits (probably the sign and endian fields) and +40's swizzle (12 bits). Leaf. From `Texture::LevelDesc`. Not read in detail. Kept-kind. Native in `src/graphics/resources.cpp` behind an adapter (`src/hooks/graphics_resources.cpp`, `--native_graphics_resources`); harness `graphics_resources`. |
| `sub_822A4410` | `GpuFormat::BlockSize` | Overridden | r3 = a GPU texture format (6 bits), r4, r5 = the block's width and height out: 4 × 4 for 18-20 (DXT1-DXT5, probably), 49, 51-53 and 58-61 (the other compressed formats); 2 × 1 for 11, 12 and 40; 4 × 1 for 39 and 44; 1 × 1 otherwise. Leaf. From `Texture::LevelLayout`, `Texture::LevelOffset`, `Texture::LockLevel`, `TextureLayout::LevelSize`, `TextureLayout::Sizes`, `XGSetTextureHeaderEx`, and `sub_8236D3C8` (the DXT compressor's lead in `../loops/leads.md`). Kept-kind. Native in `src/graphics/resources.cpp` behind an adapter (`src/hooks/graphics_resources.cpp`, `--native_graphics_resources`); harness `graphics_resources`. |
| `sub_822A4348` | `Texture::BaseSize` | Overridden | r3 = a texture header, r4, r5, r6 = width, height and depth out, from the fetch constant's size word (+36) by the dimension (+48 bits 9-10): 1D the low 24 bits; 2D and cube 13 bits each (depth 1); 3D 11, 11 and 10 bits; each + 1, plus 2 when +40's top bit is set (probably the border). Leaf. From `Texture::LevelLayout` and `Texture::LevelOffset`. Kept-kind. Native in `src/graphics/resources.cpp` behind an adapter (`src/hooks/graphics_resources.cpp`, `--native_graphics_resources`); harness `graphics_resources`. The plain C++ reads +36 once (the original reloads it after each store; the outputs never point into the header). |
| `sub_822A4530` | `TextureLayout::LevelCount` | Overridden | r3, r4, r5 = width, height, depth, r6 = the border: the full mip chain's level count, 1 + log2 of the largest of them (less twice the border, at least 1). Leaf. From `TextureHeader::Set`. Kept-kind. Native in `src/graphics/resources.cpp` behind an adapter (`src/hooks/graphics_resources.cpp`, `--native_graphics_resources`); harness `graphics_resources`. |
| `sub_822A45B8` | `TextureLayout::LevelSize` | | r3, r4, r5 = width, height and depth (pointers, aligned in place), r6 = bits a pixel, r7 = the GPU format, r8 = the dimension (2, 3D: depth aligned to 4), r9 = nonzero when tiled (the fetch constant's +28 top bit; the width aligned to 32 blocks, or when linear to 256 bytes a row, at least 32 blocks); the height aligned to 32 blocks; returns the level's bytes, aligned to 4 KiB (per slice, times the depth, for 2D). Calls `GpuFormat::BlockSize`. From `Texture::LevelLayout`, `Texture::LevelOffset` and `TextureLayout::Sizes`. Not read in detail. Kept-kind. |
| `sub_822A46C8` | `TextureLayout::Sizes` | | The base level's and the mip chain's sizes for `TextureHeader::Set` (r3-r10 and the stack: width, height, depth, levels, the type, the format, the border and the outputs; returns a size). Loops over the levels with `TextureLayout::LevelSize` and `GpuFormat::BlockSize`; cube maps' faces aligned. Not read in detail. Kept-kind. |
| `sub_822A4B98` | `Texture::LevelLayout` | | r3 = a texture header, r4 = the level, r5-r9 = out: width, height, depth, row pitch and slice size. Level 0 from `Texture::BaseSize` (a surface, type 4, from the header's pitch); others halved per level (at least 1), packed levels (+48 bit `0x800`) below 16 texels laid out in the mip tail; `TextureLayout::LevelSize` for the aligned extent; bits a pixel from the format table at `0x82013468`. Calls `Resource::GetType`, `GpuFormat::BlockSize`. From `Texture::LevelDesc`. Not read in detail. Kept-kind. |
| `sub_822A4FF0` | `Texture::LevelOffset` | | r3 = a texture header, r4 = the face or array slice, r5 = the level, r6-r10 and the stack = out: where the level starts (the face's offset, the face's size times r4, plus the level's), its row pitch, slice size, width, height, depth and an offset inside the packed mip tail (`sub_822A4A40`, not seen in play). Calls `Resource::GetType`, `Texture::BaseSize`, `TextureLayout::LevelSize`, `GpuFormat::BlockSize`. From `Texture::GetSurfaceLevel`, `Texture::LockLevel` and `Device::Resolve` (`functions.md`). Not read in detail. Kept-kind. |
| `sub_822A5608` | `TextureHeader::Set` | | Fills a texture's 52-byte header: r3 = the type (3 2D, 17 volume, 18 cube, 19 array, 20 1D), r4, r5, r6 = width, height, depth, r7 = the levels (0: the full chain, `TextureLayout::LevelCount`), r8 = the usage, r9 = the format, r10 and the stack the border, the base and mip offsets, the header and the base and mip sizes out (`TextureLayout::Sizes`). +0 the type, +4 the count 1, +8, +20, +24 cleared, +28 to +48 the fetch constant. From `Device::CreateTexture` and `XGSetTextureHeaderEx`. Not read in detail. Kept-kind. |
| `sub_8235A720` | `XGSetTextureHeaderEx` | | XGraphics' header set-up: from width, height, depth, levels, usage, format and the stack's base and mip offsets, fills a header on the stack with `TextureHeader::Set` (the block size checked with `GpuFormat::BlockSize`), sets its +20, +24 to `0xFFFF0000`, its +48 mip address, and copies the 52 bytes out (`sub_825A7410`, memcpy); the base and mip sizes out. From `XGSetTextureHeader`. Not read in detail. Kept-kind. |
| `sub_8235A920` | `XGSetTextureHeader` | | Calls `XGSetTextureHeaderEx` with depth 1, the type 3 and its other arguments moved along. From `Texture::Create` (`sub_820ECF08`, `car-models.md`) and `sub_821330D0`. Kept-kind. |
| `sub_822A5910` | `SurfaceHeader::Set` | | Fills a 48-byte surface header: r3 width, r4 height, r5 format, r6 the multisample type, r7 the parameters (+0 the EDRAM base, +4 the hi-z base, +8; 0 for none), r8 the header, r9, r10 out (the EDRAM tiles, `Surface::EdramTiles`, and the hi-z size, for depth formats 22 and 23). +0 = 4, +4 = 1, +20 = `0xFFFF0000`, +24 the pitch in 80-pixel tiles (40 with 4× samples) and the samples, +28 the EDRAM base and format bits, +32 the hi-z, +36 the size, +40 the format, +44 the tiles × 5120 bytes. From `Device::CreateSurface`. Not read in detail. Kept-kind (the renderer reads the EDRAM base from the surface's registers). |
| `sub_8209AD68` | `Surface::EdramTiles` | Overridden | r3 width, r4 height, r5 format, r6 the multisample type (1 doubles the height, 2 the width too): the EDRAM tiles a surface takes, the width aligned to 80, the height to 16, 4 bytes a sample (8 for formats 21, 32, 37), over 5,120 bytes a tile. Leaf. From `SurfaceHeader::Set`, `sub_8209B070` and `sub_825D99D0`. Kept-kind. Native in `src/graphics/resources.cpp` behind an adapter (`src/hooks/graphics_resources.cpp`, `--native_graphics_resources`); harness `graphics_resources`. |
| `sub_822A5D08` | `Device::CreateTexture` | | Probably creates a texture: r3 width, r4 height, r5 the depth, r6 the levels, r7 the usage, r8 the format, r9 (0), r10 the type (3); returns it or 0. A 52-byte header (`XMemAlloc(52, 0x64800000)`) filled by `TextureHeader::Set`, then the base and mip data in physical memory (4 KiB aligned, its attributes by r7's `0x2`), their addresses in +32 and +48; all freed if one fails. From `Device::SetUpDisplay` for the front buffer (skipped: the game sets parameters +60), and twice from the display set-up `sub_8258D568` for the video output's two front buffers (1280x720, `0x28280106`). Also from `CameraPasses::MakeRenderTarget` and its case 0 (`cameras.md`; 512 × 128, `0x182801B6`), `sub_8209B070`, `sub_82132CE0` and `sub_824840C8`. Kept-kind. |
| `sub_822A5E28` | `Device::CreateSurface` | | Probably creates a surface: r3 width, r4 height, r5 format, r6 parameters +16, r7 0; returns it or 0. A 48-byte header (`XMemAlloc`) filled by `SurfaceHeader::Set`, +0 bit `0x00100000` set; with r7 0, its EDRAM tiles from the EDRAM allocator (`Edram::Alloc`; base plus tiles past 2,048 fails), +0 bit `0x80000000` and the base in +28's low 12 bits; the first depth surface (formats 22, 23) takes the hi-z (`0x829537C4` = 1). From `Device::SetUpDisplay` (render target, depth surface), `CameraPass::SetUpSurfaces` (`cameras.md`), `sub_8209B070`, `sub_82134410`, `sub_82135AA8`, `sub_82136130`, `sub_82138610`. Kept-kind. |
| `sub_822A5B80` | `Texture::GetSurfaceLevel` | | r3 = a texture, r4 = the level: a 48-byte surface (`XMemAlloc(48, 0x64800000)`; 0 if it fails) for the level: +0 the texture's word with type 4 and bit `0x40000000` (a child), +4 = 1, +20 = `0xFFFF0000`, +24 the texture, +28's top 4 bits the level (bits 22-27 the face, 0); `Resource::AddRef` on the texture. Calls `Texture::LevelOffset` first (its answers unused, probably). From `CameraPass::SetUpSurfaces` (`cameras.md`), `Vinyl::Compress` (`car-models.md`), `sub_820ED528` (not seen in play) and `sub_820ED6D0`. Kept-kind. |
| `sub_822A6830` | `Device::CreateVertexBuffer` | | Probably CreateVertexBuffer: r3 bytes, r4 and r5 0; allocates like `sub_822A6968`. From `sub_82482B18` (120 bytes). Not read in detail. 32-byte header: +0 type 1 (`0x00100001`, or `0x00300001` with usage `0x4`; bit `0x00400000` with usage `0x200`), +4 = 1, +20 = `0xFFFF0000`, +24 the data's address \| 3, +28 its size and fetch bits; the data physical, its attributes by usage `0x4`. The caller is overridden (`src/hooks/video_mode.cpp`) and calls it. Kept-kind. |
| `sub_822A6968` | `Device::CreateIndexBuffer` | | Probably CreateIndexBuffer: r3 bytes, r4 usage, r5 format (1), r6 0. Allocates a 32-byte header (`sub_822976F8`) and the data, freeing the header if the data fails; returns the header (+24 the data) or 0. From `sub_82482B18`. Not read in detail. +0 type 2, `0x00100002` with the format shifted to the top 3 bits (`0x00300002` with usage `0x4`), +4 = 1, +20 = `0xFFFF0000`, +28 the size. Called from `sub_82482B18`'s override (`src/hooks/video_mode.cpp`). Kept-kind. |
| `sub_822A3998` | `Device::GetRenderTarget` | | r3 = the device, r4 = the index: returns the device's render target (+12184 + 4 × index), `Resource::AddRef`ed when set. From `sub_820A2F94` (`cameras.md`), `sub_82590A68` (`Command::*`, `functions.md`) and `sub_8258ED00`. Kept-kind. |
| `sub_822A39E0` | `Device::GetDepthStencilSurface` | | r3 = the device: returns its depth surface (+12200), `Resource::AddRef`ed when set. Same callers as `Device::GetRenderTarget`. Kept-kind. |
| `sub_822A3880` | `Device::GetStreamSource` | | r3 = the device, r4 = the stream, r5, r6 = the offset and stride out: returns the stream's buffer (+12204 + 4 × stream, `Resource::AddRef`ed); the offset from the stream's fetch address (device + 8 × (223 − stream)) less the buffer's, the stride from byte +12272 + stream, × 4. From `sub_82482CA8` (the boot movie's player, a lead). Kept-kind. |
| `sub_822A54B0` | `Texture::LockLevel` | | r3 = a texture, r4 the face, r5 the level, r6 the flags, r7-r10 out: the level's place (`Texture::LevelOffset`), the pitch times the block height, then `Resource::Lock` (code 14) over the base or mip data; the address out is the lock's answer plus the level's offset. From `Texture::LockRect`. Not read in detail. Kept-kind (the address); the lock's GPU work Dropped-kind, probably. |
| `sub_822A5578` | `Texture::LockRect` | | r3 = a texture, r4 the face, r5 the level, r6 = the locked rect out (+0 the pitch, +4 the bits), r7 = a rectangle or 0, r8 the flags: `Texture::LockLevel`, the bits moved to the rectangle's top-left (top × pitch + left × the format's bits a pixel / 8, the table at `0x82013468`). From `Texture::LockRectLevel` and `Surface::LockRect`. Kept-kind. |
| `sub_822A5C38` | `Texture::LockRectLevel` | | r3 = a texture, r4 the level, r5 the locked rect, r6 the rectangle, r7 the flags: `Texture::LockRect` with face 0. From `sub_824842C8` (the boot movie's draw, `../gpu-functions.md` "Movies"). Kept-kind. |
| `sub_822A5FE8` | `Surface::LockRect` | | r3 = a texture's surface, r4 the locked rect, r5 the rectangle, r6 the flags: `Texture::LockRect(parent +24, face (+28 >> 22 & 0x3F), level (+28 >> 28), ...)`. From `Vinyl::Compress` (`car-models.md`), `sub_820ED528` (not seen in play) and `sub_820ED6D0`. Kept-kind. |
| `sub_822A43E0` | `Texture::Unlock` | | r3 = a texture: `Resource::Unlock(texture, base +32, mip +48)` (their upper 20 bits). From `sub_824842C8`. Kept-kind as a call; its work Dropped-kind, probably (see `Resource::Unlock`). |
| `sub_822A43F8` | `Surface::Unlock` | | r3 = a texture's surface: `Texture::Unlock` on its parent (+24), inlined. From `Vinyl::Compress` (`car-models.md`), `sub_820ED528` (not seen in play) and `sub_820ED6D0`. As `Texture::Unlock`. |
| `sub_822A68F8` | `VertexBuffer::Lock` | | r3 = a vertex buffer, r4 the offset, r5 the size (0: all, from +28), r6 the flags: `Resource::Lock` (code 10) over +24's data. From `sub_82482CA8`. Kept-kind (the address). |
| `sub_822A6958` | `VertexBuffer::Unlock` | | r3 = a vertex buffer: `Resource::Unlock(buffer, +24's data, 0)`. From `sub_82482CA8`. As `Resource::Unlock`. |
| `sub_822A6A18` | `IndexBuffer::Lock` | | Probably a buffer's Lock: r3 the buffer, r4-r6 0; returns the data's address. Calls `sub_822A63F8`. From `sub_82482B18`. Not read in detail. An index buffer's: r4 the offset, r5 the size (0: all, +28), r6 the flags (bit 2 added); `Resource::Lock` (code 12) over +24's data. Called from `sub_82482B18`'s override (`src/hooks/video_mode.cpp`). Kept-kind (the address). |
| `sub_822A6A78` | `IndexBuffer::Unlock` | | Probably a buffer's Unlock: r3 the buffer; calls `sub_822A6630`. From `sub_82482B18`. Not read in detail. An index buffer's: `Resource::Unlock(buffer, +24, 0)`. Called from `sub_82482B18`'s override. As `Resource::Unlock`. |
| `sub_822A63F8` | `Resource::Lock` | | r3 = the resource (a texture's surface stands for its parent), r4 = a code (10 vertex buffer, 12 index buffer, 14 texture), r8 = the data's address, r9 = its size, r10 the flags: waits for its fence (+8, or the read fence +12 with flags `0x1010`) through `sub_8229E908` (`functions.md`); unless flags `0x12`, writes a cache flush packet (type 3, `0xA31` registers, 10 words) into the device's command buffer (+40, `sub_8229EF48` when full), or from another thread than the device's (+10760 against `sub_825B9A60`) queues it (`sub_8229DDD8`); notes the locked range in +20 or +24 (128-byte units); adds 256 to +0 (the lock count); returns the address (in another view with flag `0x10`). Uses the device through `0x820007B0`. Not read in detail. The address Kept-kind; the fence wait and the packet Dropped-kind, probably (the renderer skips the coherency packets, `src/native/gpu_link.cpp`). |
| `sub_822A6630` | `Resource::Unlock` | | r3 = the resource, r4, r5 = the base and mip addresses: takes 256 from +0 (the lock count); at the last lock, flushes the data cache over each locked range (+20, +24, then reset to `0xFFFF0000`) with `sub_822AC470` (`functions.md`); `sync`. From `Texture::Unlock`, `Surface::Unlock`, `VertexBuffer::Unlock`, `IndexBuffer::Unlock`. Dropped-kind, probably (no data cache to flush on PC), but the lock count is the game's. |
| `sub_822A6738` | `Resource::AddRef` | Overridden | `src/native/refcount.cpp`: reference counts without the library's global lock, skipped for the vertex shader already bound; the original with `--refcount_global_lock`. |
| `sub_822A67B0` | `Resource::Release` | Overridden | `src/native/refcount.cpp`: reference counts without the library's global lock, skipped for the vertex shader already bound; the original with `--refcount_global_lock`. |
| `sub_822A6210` | `Resource::Destroy` | | r3 = the resource, from `Resource::Release` at 0 (the override in `src/native/refcount.cpp` calls it): by type (+0's low 4 bits, a table at `0x82013670`), waits for its fence (`sub_8229E908`, with a code per type) and frees its data (`XMemFree`: a texture's base +32 and mip +48, other types' +24 or +48; one type through `sub_822AD9D8`; which type is which not worked out), then the header (`XMemFree(r3, 0x24800000)`). Surfaces go through `sub_822A6268`. Not read in detail. Kept-kind (the memory); the fence waits Dropped-kind, probably. |
| `sub_822A6268` | | | `Resource::Destroy`'s surface case (split off): a texture's surface (+0 bit `0x40000000`) frees only its header; a render target's EDRAM (+0 bit `0x80000000`) is freed (`sub_822B5EA8`: base +28's low 12 bits, tiles +44 / 5120); one holding the hi-z (+32 bit 0) clears `0x829537C4`. Kept-kind. |
| `sub_822B5EF8` | `Edram::Alloc` | | r3 = tiles, r4 = the base out; returns 1 or 0. The EDRAM allocator at `[0x82953E20]`, made on first use (804 bytes, `XMemAlloc`, its critical section `RtlInitializeCriticalSection`, +28 = 6,144 free, +800 = `0x55555555`, set with a compare-and-swap), under its critical section: the smallest free run that fits (`sub_822B5C90`), marked used (`sub_822B5D70`), +28 less the tiles. From `Device::CreateSurface`. Kept-kind (the renderer places targets by EDRAM base), but the kernel's critical section would go. |
| `sub_822B5C90` | `Edram::NextRun` | | r3 = the allocator, r4 = a position (in and out): the next free run's length from the position in the bitmap at +32 to +800 (bit set = used), 0 at its end. Leaf. From `Edram::Alloc`. Not read in detail. Kept-kind. |
| `sub_822B5D70` | `Edram::Mark` | | r3 = the allocator, r4, r5 = a run's start and end, r6 = 1 to set (used) or 0 to clear its bits in the bitmap at +32. Leaf. From `Edram::Alloc` and `sub_822B5EA8` (not seen in play). Kept-kind. |
