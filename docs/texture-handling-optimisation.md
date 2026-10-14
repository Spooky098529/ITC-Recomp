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

## Who writes textures' memory (2026-10-15)

The hashing exists because the game writes textures with plain CPU
stores and copies, and nothing tells us. The aim is to replace it with
being told: a flag set on a texture when its memory is written.

The library's memcpy (`sub_825A7410`) is already wrapped in `buffers.cpp`
and records every copy into the physical views (range, caller, a
sequence number); the vertex and index mirror uses that record instead of
hashing (see "Buffers" in `native-renderer.md`).
`--native_renderer_log_texture_writes` checks textures against it: on
first load, how much of the texture's memory memcpy wrote and from
where; on a change, whether memcpy wrote it since the last load, and for
changes without memcpy, which bytes changed. With
`--native_renderer_verify_textures` every texture used is hashed every
frame, so every change is found. An 80 s attract race (`itc_663.log`,
`itc_666.log`):

- **First loads**: of 1,465 textures, 1,449 were written whole by memcpy,
  11 partly and 5 not at all; nearly all from the resource loader `sub_82596F68` (`0x82597014`), the
  same loader as vertex and index data. A few 300x150 RGBA textures
  come from `sub_820ED6D0` (`0x820ED910`, `0x820ED920`), and are later
  redrawn by `sub_820ED630` (`0x820ED6B8`), a rectangle copy (a memcpy
  per row): images the game composes on the CPU, also through memcpy.
  They're among the "partly" ones: memcpy writes their rows, not the
  padding past the last row that the range includes (see below).
- Five first loads had no memcpy writes at all (a 256x144, a 1024x1024
  and three 16x16; addresses vary between runs). None of them changed
  afterwards. Their writer isn't known.
- **Changes**: 748 found. 7 were written by memcpy since loading. The
  other 741 were one texture (`18DAD000`, 300x150, untiled, pitch 320) and
  once its neighbour, and **they weren't changes to the texture**: the
  bytes that changed were at offsets 192,517-200,319, past the image's
  last row (150 rows of 1,280 bytes end at 192,000). The hashed range pads
  the row count to 32 (160 rows) as the tiled layout needs; for an
  untiled texture that padding is other data, which the game updates
  every tick. So the cache reloaded an unchanged 200 KB texture whenever
  it rechecked it.

So every real change seen went through memcpy, as for vertex and index
data. A gdb watchpoint on the texture's pixels (all three CPU views of
physical memory) caught nothing, as expected, since its pixels never
changed. (Attaching gdb to a running game isn't allowed here,
`ptrace_scope` 1: start the game under gdb and interrupt it with
`kill -INT` instead.)

### The plan from this

1. Hash only the bytes the texture uses: for untiled textures, up to the
   end of the last row's data, not the padded row count. This removes the
   false changes, today and in the verification below.
2. Mark textures dirty from memcpy's record instead of hashing: once a
   frame, take the new writes and flag the textures they overlap (a map
   of textures by address); a draw only checks the flag. Reload flagged
   textures.
3. Keep hashing only for checking (`--native_renderer_verify_textures`):
   log any texture whose memory changed without a recorded write. Then
   no hashing in normal play, the first-load hash included, and changes
   picked up at once instead of up to 240 frames late.

Open: the five textures with no memcpy writes. They're loaded at first
use and never change in these runs; a race the user drives may differ,
which step 3's check would show.

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

4. **Hash young textures less** (superseded by "The plan from this"
   above, which removes hashing). A new texture is hashed every 4 frames
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
