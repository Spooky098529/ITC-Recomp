# Known functions: models, textures and shaders

The graphics library's model loading and recording: a part file (`00 15 00 00`) parsed into a model (its head copied, its vertex data into physical memory, its shaders made), its node file's skeleton, and each shape's draws recorded with their shaders, constants, render states and textures (`Model::RecordShape`; `Model::RecordMaterials` for car parts, with the shared material shaders and the vinyl sheet); the XPR2 texture bundles (`XPR::`); and the boot's shader pairs (`Shaders::LoadAll`). How the recorded commands are replayed is in `../gpu-functions.md`.

The rows were drafted on 2026-10-23 from SBX-Recomp's `docs/game_data.md` (the Japanese release's recomp; `../referencing-sbx-recomp.md`), each fact checked against our `generated/` code: offsets and asset ids are our build's, which differ from SBX-Recomp's in places. "Name from SBX-Recomp" marks the names taken from it; the others are ours. The data layouts are in `../known-data.md`.

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
| `sub_82596920` | `Model::Load` | | r3 = a model, r4 = a part file, r5 = an XPR2 or 0: frees what it had (`sub_82594970`), parses the file (`sub_825963F0`), loads r5 as its texture set at +56 (`XPR::Create`) into the provider at +80 (vtable slot 4, index 0; +48 = +80), then normalizes the skinned shapes' weights (the last = 1 − the rest). 12 callers (car parts, `sub_820D8D20`, `sub_820C52A0` and others). Name from SBX-Recomp. |
| `sub_825963F0` | `Mesh::Parse` | | r3 = a model, r4 = a part file: the version 0x00150000 (else a debug print, `sub_825ABFE8`), the shaders (`sub_82594D20`), the bytes before the vertex data copied to +16 (return address `0x82596448`), the offsets made pointers, the vertex data copied to physical memory (+20; `0x8259676C`), the vertex and index buffer headers relocated (`sub_8235A9F8`), the sections to +112 to +384 (`sub_82599A20`, `sub_825997E0`, `sub_825A1BE8`, `sub_825A1218`, `sub_825A1A38`, `sub_825A1E08`). Only caller `sub_82596920`. Name from SBX-Recomp. |
| `sub_82594D20` | `Mesh::LoadShaders` | | r3 = a model, r4 = the shader section: +420 the pair count, +24 16 bytes a pair; each shader parsed (`sub_822A8568`), made (`sub_822A9FD8`, `sub_822A9EA8`), its microcode copied to physical memory and set (`sub_822A8740`, `sub_822A85B8`). Not read in detail. Name from SBX-Recomp. |
| `sub_82594970` | `Model::Release` | | r3 = a model: frees +16, +20, each shader pair's four words and +24 (`sub_8258E410`), and clears them. Not read in detail. |
| `sub_82594AD8` | `Model::MaterialCount` | | r3 = a model, r4 = a shape, r5 = the group: the material block's +8 (group 0) or +12. |
| `sub_82594B20` | `Model::Material` | | r3 = a model, r4 = a shape, r5 = an index, r6 = the group: the block + 16 + 512 × index (group 1 from index [block + 8]). |
| `sub_82595550` | `Model::RecordShape` | | r3 = a model, r4 = a shape, r5 = the group, r6 = whether to set the shaders, r7, r8 optional constants: records the shape's vertex declaration, streams and each draw's shaders, constants (vertex c9 to c13, pixel c63 to c67), blend (the table at `0x8265EEB4`), render states, textures (provider slot 5 by +0x90, +0xB0), indices and draw (command 18). From `sub_820EF790` (3), `sub_82596B18` (2). Name from SBX-Recomp. |
| `sub_820CECA0` | `Model::RecordMaterials` | | As `Model::RecordShape`, for car parts: the shared pairs 1000-1005 (1005 forced by +440, 1004 drawn with 1000's shaders); +444 bound as sampler 0 for 1000 and 1004 and sampler 1 for 1002, the provider's for 1001 and 1002, the +436 → +460 replacement for 1003, no textures for 1005; with pair 0 by the category (+0x198): 3 `[+452]`, 5 `sub_8209F7A8(0)`, else `[+456]` when +432 and +456 are set; sampler 6 from `0x8296B8F0` or the provider's slot 6; the cull mode's 2 and 6 swapped when `0x826C5100` is set; render state 48 by `0x826C5104`; the blend table at `0x826018A4`. Called by `Model::Draw` (`sub_820D04E8`) for groups 0 and 1. Name from SBX-Recomp. |
| `sub_820CE928` | | | Probably a car part model's constructor: the base `sub_825948C0`, vtable `0x8200338C`, +432 to +460 cleared (+436 = -1). 8 callers. |
| `sub_820CE9C8` | | | Probably clears a car part model's textures: +432 to +460 (+436 = -1). |
| `sub_820CE990` | | | Probably a car part model's destructor: its fields cleared, then `sub_82594F58`. |
| `sub_820CE9F8` | `Model::SetVinylTextures` | | r3 = a car part model: +444 = r4 (the vinyl sheet), +448 = r5. From `sub_820D5F88`, `sub_820D72D8`, `sub_820D7D38`. Name from SBX-Recomp. |
| `sub_825A21A8` | | | A node file's view: r3 = the view, r4 = the file: +0 the file, +4 to +40 its +0x10 to +0x30 offsets made pointers (+36 = the tail + 16), +44 = [+4] − [+8]. |
| `sub_8259A9E0` | | | Probably sets up a skeleton: r3 = it, r4 = a node file, r5 = matrices or 0, r6 = the instance count (at +160, at least 1): frees the old (`SkeletonBase::Free`, `model-animation.md`), +128 a 20-byte cursor an instance (`SkeletonCursor::Clear`), the view at +80 (`sub_825A21A8`), 144 bytes a node at +136, 64 a node at +132 (each 16-byte row's +12 = 1.0) unless r5 is given, 64 for each of [+8] at +144 (+52 = 1), +140 node [+0xC]'s entry, +148 the matrices (r5 or +132). Not read in detail. |
| `sub_82599A20` | | | Binds a node file's skeleton: `sub_8259A9E0` (r6 = 1), +176 a float4 a node (x, y, z set to 0.0; corrected 2026-10-23, it was given as 1.0; `Skeleton::NodeCount`, `model-animation.md`). From `sub_825963F0` and the car loaders. |
| `sub_82599AF0` | | | Resets a skeleton's pose from its file: each node's translation, angles, scale, and its +0x40 when its +0x84 has bit 0x40. |
| `sub_82599C58` | | | Builds each node's local matrix: the scale, RotX, RotY, RotZ by its angles (wrapped to ±π; `sub_820A65F0`, `sub_820A67F8`, `sub_820A2D30`), × the file's +0x24 matrix, the translation. Bones (bit 0x80) after. Not read in detail. |
| `sub_8259C3A8` | | | Each node's matrix × its parent's (+0x80), in node order; +0x5C's translation copied in; types 8 and 16 to `sub_8259B748`, `sub_8259BE40`. Through `sub_825998A0`. |
| `sub_8235A9F8` | | | Probably relocates a graphics resource's address: by its type (`sub_822A6180`): 1 adds r4 to +24 keeping the low 2 bits; 2 and 8 add r4 to +24; 3, 6 and 17-20 `sub_8235A990`. Not read in detail. |
| `sub_82589728` | | | A vertex declaration from three stream types (tables `0x8265EC00`, `0x8296B7F4`). Not read in detail. |
| `sub_825896D8` | | | A shared vertex shader by pair: `[0x8296B7B4]` + 4 × r3 (0 when unset). |
| `sub_82589700` | | | A shared pixel shader by pair, from `[0x8296B7B8]`. |
| `sub_825896C0` | | | Stores r3 and r4 (the vertex and pixel shader tables) at `0x8296B7B4` and `0x8296B7B8`. Leaf. |
| `sub_82596E68` | `XPR::XPR` | | Probably the constructor: clears the 24-byte XPR object. Leaf. |
| `sub_82596F68` | `XPR::Create` | | r3 = an XPR object, r4 = an XPR2 file: 0x80004005 after a debug print (`sub_825A1318`, `0x8206CE20`) when +0 isn't `XPR2`; else +4 and +12 the header and data sizes, the header block (`sub_8258DFF0`) gets header-size bytes from +12 and the data block (`sub_8258E138`, physical; a debug print at `0x8206CDFC` when that fails, then it carries on) the data; each record's name offset made a pointer and `TX` records' textures relocated (`sub_8235A990`). Returns 0. 25 call sites (UI movies, car overlays, course data). Name from SBX-Recomp. |
| `sub_82596E88` | `XPR::FindTexture` | | r3 = an XPR object, r4 = a name: the first record whose name matches (`_stricmp`, so case-insensitive) gives its resource header; 0 when none does, with no records, or when r4 is null. From `sub_8258C0B8`, `sub_82594230`. Not seen in play. Name from SBX-Recomp. |
| `sub_8235A990` | `TextureHeader::Relocate` | | r3 = a texture header, r4, r5 = the base and mip base: r4 added to the upper 20 bits of +0x20; when `sub_822A5B68` gives more than one level, r5 to +0x30's. Name from SBX-Recomp. |
| `sub_822A5B68` | | | A texture header's mip level count: ((+44 >> 6) & 0xF) + 1. Leaf. |
| `sub_8258E138` | | | Probably `Memory::AllocPhysical`: r3 bytes, r4 the `XMemAlloc` attributes (`sub_822976F8`), with a 4 KiB `XLMM` prefix (+4 = 2); returns the block + 4096, or 0. Not read in detail. |
| `sub_820A08A0` | `Shaders::LoadAll` | | Once at boot (from `sub_82106618`): `[0x826C1CD8]` = 0, then the shader pairs (`sub_820A07F0`): assets 3 to 69 by twos as pairs 0-33, 22801 to 22811 as 1000-1005, 24258 to 24368 (`BUILD_CRS`) as 2000-2055; each the vertex shader, with its pixel shader the next asset. Then gives the tables to the graphics library (`sub_825896C0`). Name from SBX-Recomp. |
| `sub_820A07F0` | | | r3 = the vertex shader's asset, r4 = the pair: both sizes (`sub_820A0678`), placed back to back at `[0x826A5134]` + `[0x826C1CD8]` (which moves on); a 12-byte context (+0 the vertex file, +4 the pixel file, +8 the pair); two `Archive::Load`s, the second with the callback `sub_820A06D0`. |
| `sub_820A06D0` | | | `Archive::Load`'s callback (r6 = the context): for each file, parses it (`sub_822A8568`), makes its object (`sub_8258DFF0`) into `0x826BFCB8[pair]` (vertex, `sub_822A9FD8`) or `0x826BDC98[pair]` (pixel, `sub_822A9EA8`), copies the microcode into `sub_8258E098(size, 0, -1, 32, 0x404)` (probably physical, write-combined), then `sub_822A8740` or `sub_822A85B8` (not read). Frees the context. |
| `sub_822A8568` | | | Parses a shader file (r3) into r4: +0 the file, +4 the object's size (592 for a vertex shader, the magic's low 6 bits set; 56 for a pixel shader) + the header size, +8 the microcode, +12 its size. Leaf. |
| `sub_822A9FD8` | | | Probably makes a vertex shader object: clears 592 bytes and copies the header after them (`sub_82295890`). Not read in detail. |
| `sub_822A9EA8` | | | Probably makes a pixel shader object: the same with 56 bytes. Not read in detail. |
