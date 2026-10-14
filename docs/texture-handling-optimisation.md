# Texture handling: making streaming cheaper

While driving, the game streams in new track sections and their textures.
Our renderer (`itc_recomp/src/native/textures.cpp`, see milestone 3 in
`native-renderer.md`) loads each texture on the render thread in the
frame that first uses it, so a frame that meets many new textures takes
longer: a hitch. On the Steam Deck the render thread is already at ~98%
of a core in races (16-24 ms a frame), so these hitches show there first.

This file is what's been measured, what's done, and what's left to try.

## How a texture is loaded

`TextureCache::GetView`, per draw, per texture binding:

1. Look the texture up by its fetch constant's data fields.
2. First use: hash its guest memory (`State::Hash`, XXH3 over the base
   and mip ranges), then `State::Upload`:
   - for every level and layer, row by row: gather the row from the 360's
     tiled layout (`GatherTiledRow`) into a row buffer, byte-swap it
     (`SwapEndian`), convert if needed (`4_4_4_4` to RGBA8) and copy it
     into this frame's 64 MB staging buffer;
   - record a barrier, `vkCmdCopyBufferToImage` and a barrier into the
     upload command buffer (submitted before the frame's drawing).
3. Later uses: hash again now and then to catch the game rewriting the
   texture in place: every 4 frames for the first 240 frames after
   loading, then every 240 frames (8 a frame at most). A changed hash
   reloads it.

A texture that doesn't fit in what's left of staging is deferred to a
later frame; meanwhile draws get a white 1x1 placeholder.

## Measuring

Pinned to one CPU (`taskset -c 0`, a stand-in for the Deck), an
automated attract race (see `CLAUDE.md`). The log gives:

- "long frame" lines for any frame of 40 ms or more: textures loaded,
  MB, time loading and time hashing (`frame_costs.h`);
- every 5 s, in the "textures:" line: textures loaded and reloaded, MB,
  the time spent loading and hashing, and the worst frame.

These need info logging (`run-debug.sh`); `run.sh` turns it off and the
timing with it (`log_enabled.h`).

## Done

**Untiling by runs, SIMD byte swaps** (2026-10-15, `71fb4f0`). The
gather used to work block by block: a tiled address computed and a
`memcpy` for every 8- or 16-byte block. The tiled layout keeps 16 bytes
contiguous, aligned in x (8 bytes for 1-byte formats: the micro tile's 8
columns), so `GatherTiledRow` now copies a run per address. The byte swap
uses SSSE3 shuffles, 16 bytes at a time (the build's `-march=x86-64-v2`
has SSSE3; other targets keep the scalar loop). Checked equal to the old
gather on 180,224 tiled rows of a race.

| Pinned to one CPU | Before | After |
|---|---|---|
| Race start: 44 textures, 11 MB | 33.2 ms | 9.8 ms (1.5 ms of it hashing) |
| Worst frame in each 5 s of the race | not measured | 3-11 ms |

**Diagnostics only when logging** (`cf31e51`): the 5 s stats, the frame
cost timing and the dimension mismatch descriptions do nothing with
logging off.

**Decided against: a per-frame time budget** (the user's call). Capping
texture work at a few ms a frame and deferring the rest would bound the
hitch, but deferred first loads show the white placeholder for several
frames: visible pop-in.

## What's left to try

In the order suggested. Gains are estimates until measured.

1. **Profile what's left.** The ~8 ms of non-hashing time for 11 MB
   isn't split yet between computing tiled addresses, the gather, the
   byte swap and writing to staging. A `perf` profile of a pinned race
   says which of 2 and 3 matters.

2. **Untile whole 32x32 macro tiles.** `TiledOffset2D` is still computed
   for every 16-byte run (about 20 integer operations). Within a macro
   tile the layout repeats, so a precomputed offset table per bytes per
   block could replace it, and walking tile by tile reads guest memory in
   order instead of jumping about. Perhaps up to 2x on the untiling.

3. **Write straight into staging.** Rows go to a row buffer and are then
   copied into staging. Whether that copy can go depends on the staging
   memory type: if it's write-combined (likely, as the first
   `HOST_VISIBLE | HOST_COHERENT` type is picked), the byte swap must not
   read it back, so the swap would have to happen on the way in. Check
   the memory type first; the gain is likely small.

4. **Hash young textures less.** A new texture is hashed every 4 frames
   for 240 frames: about 60 hashes each, 36 a second for its first 1.7 s
   at 144 fps. This is there because the game can still be writing a
   texture when it's first drawn ("Textures replaced in memory" in
   `native-renderer.md`). Options: lengthen the interval as the texture
   ages, or hash a sample of its memory. Split the hashing time out of
   the 5 s line first to see what it costs.

5. **A worker thread for the CPU work.** The biggest win for the Deck,
   which has spare cores (~30% of the machine busy). The worker hashes,
   reads guest memory, untiles and converts into its own buffer; the
   render thread copies finished textures into the frame's staging and
   records the copy (staging belongs to frames in flight). Things to mind:
   - guest memory can change under the worker (textures rewritten in
     place): hash again after converting and requeue if it changed;
   - locked and atomic operations on the render thread are expensive
     (Known issues in `native-renderer.md`): hand work over once a frame,
     not per draw;
   - first uses still get the white placeholder until the worker is done,
     a frame or two: the same pop-in as the budget, but much shorter. A
     cheap smallest mip first could cover it.

6. **Untile on the GPU.** Copy the raw tiled data (or use it from the
   guest memory mirror, if that covers it) and untile and byte-swap in a
   compute shader, as Xenia does. Takes the most off the CPU; the most
   work.
