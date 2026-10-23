# Known functions: the car models

Loading a car's models: the outside model (set a or b, by a mode), the cockpit body (set c) and the interior, their parts by the car's tuning choices, the wheels (rims, brakes, tires and calipers), all through 24-byte load slots, most into the shared load buffer; the nodes the models are placed at (the tires, seats, accessory); and the vinyl sheet: each car's decal TGAs decoded and drawn into a 1024 x 1024 canvas by a software rasterizer on the CPU, then compressed to DXT5 and bound in place of the paint. The scene pools the models live in are in `vehicle-accessors.md` (`ScenePools::`); the model format and the draws in `models.md`.

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
| `sub_820D5EC8` | `CarModel::LoadExterior` | | Scene pool 0's set-up (from `ScenePools::SetUp`): r3 = the car model, r4 = its set-up: copies 3,696 bytes to +32, makes the vinyl sheet (`Texture::Create` `sub_820ECF08`, 0x18280086, the size at `0x82602570`, into +6684/+6688), for blocks 18 and up sets sticker group 7's first record to the team emblem (`sub_820D3CF8`: +592 the id, +600 = 9), loads set a (`sub_820D59D8`) or set b (`sub_820D5AC0`) by `[[+7256] + 8]` (0, 1; neither otherwise), then the decals (`sub_820D5B50`). Name from SBX-Recomp. |
| `sub_820D59D8` | `CarModel::LoadSetA` | | r3 = the car model: the node file 893 + 226 × block (+32) into the slot at +3728, the body XPR2 891 + 226 × block into +3952, each in its own `VirtualAlloc` of the asset's size (`sub_820C3CA0`, `sub_820A0678`) and `Archive::Load` (callback `sub_820D47A8`); +3972 = 1 when the body's size is 0. Only caller `sub_820D5EC8`. Name from SBX-Recomp. |
| `sub_820D5AC0` | `CarModel::LoadSetB` | | r3 = the car model: the node file 11063 + 197 × block through `sub_820D07C8` into +3728; the body XPR2 11061 + 197 × block into the shared load buffer (`[0x826C2E44] + 5056`) with no size check (+3960 the size, not compared). Only caller `sub_820D5EC8`. Name from SBX-Recomp. |
| `sub_820D47A8` | `CarModel::OnFileLoaded` | | `Archive::Load`'s callback (no direct callers; r3 = the file, r6 = the car model), probably set a's (its format strings differ from `sub_820D4F80`'s). Texture sets (+3956, +4000, +4048; `sub_820C3CE0([+7256], ...)`); the node file (+3732): its skeleton at +3760, and seven nodes found by name (the strings at `0x82004FAC` to `0x82004FDC`): the tires' matrices to +6720, +6784, +6848, +6912 and their positions (scaled by `0x82001FBC`, probably 0.1, and rotated) to +6976 to +7024, the seats' to +7040, +7104 and +7168, +7184 (only when all seven are found), a pointer to the accessory node's matrix at +7200 and its position at +7216; +7232 to +7252 lookups by a formatted name (`sub_820D09B8`). |
| `sub_820D4F80` | `CarModel::OnFileLoadedB` | | Probably set b's: as `sub_820D47A8` without `sub_820C3CE0`. |
| `sub_820D5B50` | `CarModel::LoadDecals` | | r3 = the car model: the six vinyl layers (+208, 64 bytes; asset index + 163 for layers 0-2, + 189 for 3-5) into the slots at +4092 + 48i and the 48 stickers (eight groups, +592 to +3663; index + 383, or the index itself when the record's +8 is 9) into +4380 + 48j, through `sub_820D0860` with the callback `sub_820D5750`; a negative index leaves the slot empty (+20 = 1). +4088 = 2, or 3 when nothing was asked for. Name from SBX-Recomp. |
| `sub_820D5750` | `CarModel::OnDecalLoaded` | | `Archive::Load`'s callback (r3 = the request, r6 = the car model): marks the slot arrived (+20 = 3), draws it (`sub_820D4078`), clears it (`sub_820D0730`); once no slot is loading, clears the corner (`sub_820D3C80`), compresses the sheet (`sub_820ED090`) and sets +4088 = 3. |
| `sub_820D5D20` | `CarModel::ReloadDecals` | | r3 = the car model: queues the old sheet for release (`sub_820C3F50`), +4088 = 0, clears 3 layer and 6 sticker slots, makes a new sheet, draws an image from `sub_820C4E68` into region 8 when +3724 is 1, sets the emblem, then `sub_820D5B50`. Called from `sub_820D9D48`. Not read in detail. Not seen in play. |
| `sub_820D4508` | | | Frees the car model's loads and queues both sheets (+6684.., +6700..) for release (`sub_820C3F50`), then `sub_820D3E98`. Not read in detail. |
| `sub_820C3F50` | | | Puts 4 words with state 2 in the first free 20-byte record of `[r3 + 52]` (probably a deferred release). Not read in detail. |
| `sub_820D3CF8` | | | r3 = the car model, r4 = the block (18-44): the team emblem's asset id (168-188), or -1. r5 not read. From `sub_820D5EC8`, `sub_820D5D20`. |
| `sub_820D0860` | `LoadSlot::LoadShared` | | r3 = a 24-byte load slot, r4 = an asset id, r5 = a context, r6 = a callback: the size (`sub_820A0678`); when it's below the shared load buffer's (`[0x826C2E44]` +5060), `Archive::Load` into it (+5056), state 2; otherwise state 1 (empty). Every car part goes through it: the decals, the interior's and the cockpit's parts, the wheels, tires and calipers. Name from SBX-Recomp. |
| `sub_820D07C8` | `LoadSlot::Load` | | As `sub_820D0860`, into its own `VirtualAlloc` of the asset's size. |
| `sub_820D0730` | `LoadSlot::Reset` | | Clears a load slot (the request -1). |
| `sub_820D0758` | `LoadSlot::ReleaseBuffer` | | Frees +16 (`VirtualFree`) unless +12 (shared) is set; clears +16. |
| `sub_820D67E8` | `CarInterior::LoadSetCBody` | | Scene pool 2's set-up: copies 144 bytes to +28; the node file 19928 + 55 × block (`[+24] + 32`) through `sub_820D07C8` into +176 (`sprintf` on failure); the body 19926 + 55 × block into the shared load buffer with no size check (+400); six parts into +536 + 24i (`sub_820D0860`), with N the node file's id: N + 1 + [+28] (bonnet), N + 14 + [+32] (door), N + 17 + [+36] (mirrors), N + 29 + [+40] (headlights), N + 34 + [+44] (fenders), N + 33 (the cabin). Name from SBX-Recomp. |
| `sub_820D79B0` | `CarInterior::Load` | | Scene pool 3's set-up: copies 120 bytes to +28; by `[[+3092] + 8]` (0, 1) and +3088 (the cockpit set) calls `sub_820D7730` with (891, 226, 219), (11061, 197, 190) or (19926, 55, 49); nothing for other values. Then vtable +4 calls on `sub_820F0E50`'s object. Not read in detail past the loads. Name from SBX-Recomp. |
| `sub_820D7730` | `CarInterior::LoadParts` | | r4 = the base, r5 = the stride, r6 = the offset: six assets base + block × stride + offset + i into +160 + 24i (`sub_820D0860`). Name from SBX-Recomp. |
| `sub_820D9110` | | | Scene pool 5's set-up (the car's wheels): copies 156 bytes to +28, the rims (`sub_820D8BA8`) or brakes (`sub_820D8C50`) by `[[+3268] + 8]`, then for the front and rear the tire 582 or 584 (by +144, +148), the part (+1168), the XPR2 (+2144) and the calipers 887 (+2280), through `sub_820D0860`. |
| `sub_820D8BA8` | | | The rims: by the wheel numbers (+44, +60): -1 gives XPR2 587 + 3b and rim 588 + 3b for blocks up to 19 (signed; the block from `[+24] + 32`), else 695 and 696 (wheel 36); n gives 695 + 3n and 696 + 3n. Into +2144, +2168 and +1168, +1192. Leaf. |
| `sub_820D8C50` | | | The brakes: as `sub_820D8BA8` with 589 and 697; traffic (+28 = 3) wheel 20 + `[[+3264]]`. Leaf. |
| `sub_820E70C0` | | | The car's wheels' set-up: copies the 156-byte record to +32 (+32 the type, +48, +64 the front and rear wheel numbers), fills +112 to +152 from the table at `0x820044B0` (by block, or 45 + the traffic index), gives blocks 18 and up their fixed wheels when both numbers are -1 (`sub_820D39D8`), tire 584 for aftermarket wheels (+148, +152), claims pool 5 of `[0x826C2E40]` (`ScenePools::ClaimA`), links the wheels to the car model (+28; the wheels' +24) and calls `ScenePools::SetUp`. From `sub_820C5B28`. |
| `sub_820D39D8` | | | r3 = the block, r4, r5 = the front and rear wheel numbers (out): a fixed wheel per block 18-44 (a jump table of XPR2 ids; n = (id − 587) / 3 − 36): 18 and 35 −18 (wheel 18, the CON_X's own), 19-22 0 (AD00), 23 18, the rest their own (38: 28 front, 11 rear). Leaf. Only caller `sub_820E70C0`, when both wheel numbers are -1 for blocks 18 and up. Not seen in play. |
| `sub_820C8DE8` | | | r3 = `[0x826C2E44]`, r4 = a record: copies it, sets the shared load buffer at +5056, +5060 (from `0x826A5134`, `0x826A5130`, or the record's +36, +40 when +36 isn't 0), then fills the second set's pools. Not read in detail past +5060. |
| `sub_820D4078` | `Vinyl::DrawDecals` | | r3 = the car model: for each arrived slot, decodes it (`TGA::CreateImage`) and draws it (`sub_820EE638`): a 1024 × 1024 layer as region 9 (no rotation, scale 1); layers 0-2 into the region `[+216]` (layer 0's +8, for all three); layers 3-5 into regions 1 and 2; stickers into their +8, mirrored by +4. Frees the pixels (`VirtualFree`) and the buffer (`sub_820D0758`). Name from SBX-Recomp. |
| `sub_820ED020` | `TGA::CreateImage` | | r3 = a 20-byte header (out), r4 = the size, r5 = the buffer's address, r6 = the ARGB pixels' address (out, `VirtualAlloc`): `sub_820EEB58`, `sub_820EEA78`, `sub_820EEF08`, the header copied out, `sub_820EEAE8` (the buffer is kept). Name from SBX-Recomp. |
| `sub_820EEB58` | | | Sets up a TGA reader: +28 the size, +32 the buffer, +36 the header, +56 the image. Not read in detail. |
| `sub_820EEA78` | | | The TGA reader's load: 5 when busy; `TGA::ParseHeader`, then `TGA::Decode`. |
| `sub_820EEAE8` | | | Frees the TGA reader. Not read in detail. |
| `sub_820EEBC0` | | | Clears the 20-byte TGA header. |
| `sub_820EEBF8` | `TGA::ParseHeader` | | The 18-byte header into a 20-byte struct, its u16s swapped from little-endian. Name from SBX-Recomp. |
| `sub_820EF168` | `TGA::Decode` | | 24 or 32 bits only (else 4); w × h × 3 or 4 bytes (`VirtualAlloc`); type 2 copied, type 10 RLE (`sub_820EF0B8`), other types `OutputDebugStringA` (`0x82006694`) and 4. The pixels from buffer + 18. Name from SBX-Recomp. |
| `sub_820EF0B8` | | | TGA RLE (image type 10): bit 7 a run (`sub_820EEDA8`), else raw (`sub_820EEE58`), up to w × h − 1 pixels, from buffer + 18. The callees not read. Not seen in play. |
| `sub_820EEF08` | `TGA::ToARGB` | | The rows always flipped (row y from h − 1 − y); ARGB words; 24-bit pixels get alpha 255. Name from SBX-Recomp. |
| `sub_820EE638` | `Vinyl::DrawDecal` | | r3 = the region (0-9), r4 = mirrored, r5 to r7 the TGA header, r8, r9 the record's +16 to +31, f1 the rotation, on the stack the record's +48 to +63, the pixels and the canvas: a quad the TGA's size, each corner through `sub_820ED1E8`, clipped to the region (`0x82602A50`), drawn as two triangles (`sub_820EE0C8`). Region 9 is `sub_820EE698`. Name from SBX-Recomp. |
| `sub_820EE698` | | | Region 9's case of `sub_820EE638`'s switch, split off by the recompiler (the whole sheet, `0x82602AE0`). Not seen in play. |
| `sub_820ED1E8` | | | A decal's corner: p' = R(f1)((p − c) s) + c + t, with c the TGA's center, s the scale and t the offset plus the region's center less c. |
| `sub_820EE0C8` | `SoftRaster::DrawTriangle` | | Fills a triangle on the CPU, row by row (`sub_820EDE00`). Not read in detail. Name from SBX-Recomp. |
| `sub_820EDE00` | `SoftRaster::DrawSpan` | | One row, clipped to the region, `sub_820ED498` a pixel. Name from SBX-Recomp. |
| `sub_820ED498` | `SoftRaster::BlendPixel` | | Skips alpha 0; A, R, G = (src × a + dst × (256 − a)) >> 8; B from two products shifted separately (so it can come out one lower). Name from SBX-Recomp. |
| `sub_820ED2A0` | | | A decal's texel: reads four (probably bilinear). Not read in detail. |
| `sub_820D3C80` | `Vinyl::ClearCorner` | | Zeroes 16 rows of 16 pixels at the sheet's bottom left (the sizes at `0x82602570`). Name from SBX-Recomp. |
| `sub_820ED090` | `Vinyl::Compress` | | A DXT5 texture (0x1A200054, `sub_820ECF08`) converted from the sheet (0x18280086, `sub_8235AAC0`); the old one freed (`sub_820ECFC8`, probably); both the texture and its data replaced (r5, r6: car model +6684, +6688). Name from SBX-Recomp. |
| `sub_820ECF08` | `Texture::Create` | | r3 = the format, r4, r5 = the size, r6 = the header's address (52 bytes, `VirtualAlloc`), r7 = the data's (`XPhysicalAlloc(size, -1, 0, 0x404)`, zeroed); `sub_8235A920` sets up the header, `sub_8235A9F8` its data. Name from SBX-Recomp. |
| `sub_820D5F88` | `CarModel::SetVinylTextures` | | Hands the vinyl sheet to the car's six models (+688, 464 bytes apart): `sub_820CE9F8(model, [m + 6684], [m + 6692])`, with m = `[+24]`. From `sub_820D9B68`. Name from SBX-Recomp. |
