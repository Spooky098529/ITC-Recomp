# Known functions: audio

The game's functions that call audio kernel imports, and the game code
around them met along the way: the audio library's render driver client
(`XAudio*RenderDriver*`), its XMA layer (the contexts made with
`XMACreateContext` and `XMAReleaseContext`, the helpers that set them up,
kick and poll them through the XMA decoder's registers at `7FEA18xx`,
and the source voice code that uses them) and the voice category volume
(`XAudioGetVoiceCategoryVolume`). Worked on as B2 in
`docs/rexglue-removal.md`; how the game's sound works and the native
audio output are in `../native-audio.md`. Rows are sorted by address.
The rest of the game's functions, and the prose sections, are in
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
| `sub_82106540` | | | Called first in the audio library's hardware reset (r3 = the object). Not read. |
| `sub_8245B808` | | | Calls `XAudioGetVoiceCategoryVolume` and `XAudioGetVoiceCategoryVolumeChangeMask`, `sub_8245B480`, `sub_8245A940`, `sub_8245ACE0`, `KeReleaseSemaphore`. From `sub_8245BE08`, `sub_8245BED0` and `sub_8245BF80` (the mixer callback's, `8245FBE0`); ~250 calls a second. Audio library (B2). Not read in detail. |
| `sub_8245C638` | | Dropped | `src/hooks/xma.cpp`: does nothing (no XMA hardware; nothing reads `82954F0C` since `sub_8245D138` is ours). The recompiled one reads the XMA register `ContextArrayAddress` (`7FEA1800`) into `82954F0C`, from which `sub_8245D138` works out context ids. Once at boot, from `sub_82459D60`. |
| `sub_8245C650` | `XmaObject::Create` | Overridden | `src/hooks/xma.cpp`: an `Object` with a stream per description (sample rate +0, output ring samples +4, channels +8), "contexts made" unless r5 bit 0; its handle (a 16-byte system heap block) to [r6]; nothing else in guest memory. The recompiled one: (r3 the stream count, r4 the streams' descriptions, 12 bytes each; r5 flags, r6 out) allocates the object with its output and work buffers (`sub_8245EF80`), +0 the count, +4 flags \| 0x30000, +8 the 96-byte entries; each entry's output buffer (+68), its address and block count in the context, sample rate (24, 32, 44.1 or 48 kHz), stereo bit and work buffer (+72); then `sub_8245D138` (contexts), or `sub_8245C880` if that fails. From the voice's set-up `sub_8246AEE0`. |
| `sub_8245C880` | `XmaObject::Destroy` | Overridden | `src/hooks/xma.cpp`: frees the object's decoded buffers and its handle. The recompiled one releases the contexts (`sub_8245C8D0`) and frees the object (`sub_8245EF90`). From `sub_8245C650` (on failure) and the voice's release `sub_8246A758`. Not read in detail. |
| `sub_8245C8D0` | `XmaObject::ReleaseContexts` | Overridden | `src/hooks/xma.cpp`: clears the object's "contexts made" state only. The recompiled one calls `XMAReleaseContext` on each entry's hardware context (+64), clears it, clears flag 0x40000. From `sub_8245C880` and `sub_8246BFD8`. |
| `sub_8245C948` | | Overridden | `src/hooks/xma.cpp`: the object's "contexts made" state. The recompiled one: r3 = an XMA object: 1 if its contexts are made (flag 0x40000). |
| `sub_8245C958` | `XmaObject::SetInputBuffer` | Overridden | `src/hooks/xma.cpp`: starts the sound: a system heap buffer for all its samples (`audio::xma::Stream::BufferSamples`), the decoder started on the game's data where it lies, a second's samples decoded; a second sound given while one plays waits its turn (warned; not seen in play). The recompiled one: (object, stream, address, size in bytes): into the first free input buffer of the entry's copy: its physical address, packet count (size / 2048) and valid bit, the address at +84 or +88; for the first buffer, the input read offset from the first packet's header (`sub_8245D118`). Returns `0x80070005` if both are in use. Once a sound, from `sub_8246B480`: a sound's whole data is one buffer. |
| `sub_8245CA30` | | Overridden | `src/hooks/xma.cpp`: 1 while the sound at that address is still playing (or waiting). The recompiled one: (object, stream, address): 1 if a valid input buffer of the stream's copy is at that address (+84, +88). |
| `sub_8245CA90` | | Overridden | `src/hooks/xma.cpp`: 1 unless a sound plays and another waits. The recompiled one: (object, stream): 1 unless both input buffers are valid (one free); reads the copy with flag 0x20000, else the hardware context. |
| `sub_8245CAE0` | | Overridden | `src/hooks/xma.cpp`: 1 if no sound plays or waits. The recompiled one: (object, stream): 1 if neither input buffer is valid; the copy or the hardware context as `sub_8245CA90`. |
| `sub_8245CB38` | | Overridden | `src/hooks/xma.cpp`: samples decoded ahead of the position (across a loop jump), at most the ring size the voice asked for. The recompiled one: (object, stream): samples ready in the output buffer: the blocks from the read offset to the write offset (around the ring), × 256 bytes, less the position in the current block (+82), / 2 (mono) or 4 (stereo). |
| `sub_8245CBB0` | | Overridden | `src/hooks/xma.cpp`: takes up to r5 samples from the position (to the loop end at most; the position then jumps to the loop start), the read address to [r6]. The recompiled one: (object, stream, samples, out address): consumes up to that many samples from the output buffer: the address read from to r6, the block position (+82) and read offset moved on around the ring, the output valid bit set; adds the count to +76; returns the samples consumed. From the voice's update. |
| `sub_8245CD68` | | Overridden | `src/hooks/xma.cpp`: the position's address in the decoded buffer to [r5], the samples there in one go (to the loop end, at most the ring size). The recompiled one: (object, stream, out address): the output buffer's read address (+68 + read offset × 256 + the block position) to r5; returns the samples readable before the ring's end. From the voice's update. |
| `sub_8245CE10` | | Dropped | `src/hooks/xma.cpp`: logs an error and returns 1 (never called). The recompiled one: (object, stream, r5): moves the output write offset back by r5's blocks (or to the read offset) and sets the output valid bit; probably drops decoded samples not yet read. Only from `sub_8246BB98`'s second loop, with r5 128. Never called (the user's session, 2026-10-21, a counter). Not read in detail. |
| `sub_8245CE98` | `XmaObject::ResetStream` | Overridden | `src/hooks/xma.cpp`: stops the sound and frees its decoded buffer. The recompiled one: (object, stream): zeroes the output buffer, clears the copy's input buffers and valid bits, read offsets, loop data and +76, +82, +84, +88. From the hardware wait's reset (`sub_8245D288`, no longer reached) and the voice's stop (`sub_8246B0C8`); once a sound. |
| `sub_8245CF60` | | Dropped | `src/hooks/xma.cpp`: 0 (no errors). The recompiled one: (object, stream): the context's error status (word 2's top bits), from the copy or the hardware context. From `sub_8246B480`. |
| `sub_8245CF98` | `XmaObject::SetLoopData` | Overridden | `src/hooks/xma.cpp`: the loop's bit offsets turned into sample positions by the frame walk (`audio::xma::FrameAtBitOffset`; start + skip × 128, end frame + (end subframe + 1) × 128, less the decoder's 64): equal to the banks' loop regions (`bgm_tmp_04`, `bgm_tmp_06`). The recompiled one: (object, stream, r5 the loop data: start and end as bit offsets into the input buffer, count, the subframes to skip and play): into the copy's words 0, 1, 3 and 4 (`XMASetLoopData`). Once a music voice (count 255, forever). From `sub_8246B480` and `sub_8246BB98`. |
| `sub_8245D010` | | Dropped | `src/hooks/xma.cpp`: logs an error and returns 0 (never called). The recompiled one: (object, stream): the loop count left (word 0), from the copy or the hardware context. From `sub_8246BB98`. |
| `sub_8245D050` | | Overridden | `src/hooks/xma.cpp`: the samples taken since the sound started. The recompiled one: (object, stream): the samples consumed so far (+76). From the voice's update. |
| `sub_8245D070` | | Dropped | `src/hooks/xma.cpp`: logs an error and writes zeros (never called). The recompiled one: (object, stream, r5 out): with an input buffer valid, the current one's address (+84 or +88), the input read offset and word 1's low 3 bits to r5, else zeros. From `sub_8246B1C8`. Never called (the user's session, 2026-10-21, a counter). |
| `sub_8245D0E0` | | Overridden | `src/hooks/xma.cpp`: where playing starts: the frame at bit r5, less r6 subframes (32 and 0 in play: the start). The recompiled one: (object, stream, r5, r6): sets the copy's word 2 top bits from r5 and word 1 bits 24-26 from r6 (the output padding, probably). From `sub_8246B480`, once a sound. Not read in detail. |
| `sub_8245D118` | | | r3 = an XMA packet's first word: its first frame's bit offset + 32 (the packet header), or 0 for 32767 (no frame starts here). Only from `sub_8245C958`, which is ours: never called now. |
| `sub_8245D138` | `XmaObject::MakeContexts` | Overridden | `src/hooks/xma.cpp`: sets the object's "contexts made" state only. The recompiled one: `XMACreateContext` for each entry (into +64), its id ((physical address − `82954F0C`) / 64) at +80, and that context's Clear register written (`7FEA1A80` + id / 32 × 4, bit id % 32). Sets flag 0x40000. From `sub_8245C650` and `sub_8246BE20`. |
| `sub_8245D208` | `XmaObject::Lock` | Dropped | `src/hooks/xma.cpp`: returns 0 (nothing to lock). The recompiled one: unless flag 0x10000 or 0x20000 is set, writes each context's Lock register (`7FEA1A40` + id / 32 × 4); sets 0x10000. Once a voice update (~118 a second). |
| `sub_8245D288` | | Overridden | The audio library's wait for its hardware (r3 = the object): polls `sub_8245D350` until it answers (returns 0); after an eighth of a second (the performance counter's rate >> 3 timebase ticks) resets it: `sub_82106540`, Sleep(1000) (`sub_82295500`), 0 then `0x03000000` written to the XMA registers at `0x7FEA1804`, `sub_8245CE98` for each of the object's [+0] contexts; returns 1. ~200 calls a second in a race, from `0x8246C51C`. Ours (`src/hooks/clock.cpp`) keeps the steps as guest calls, the rate from `src/helpers/clock.h`. Audio library (B2). Its poll `sub_8245D350` is now ours (`src/hooks/xma.cpp`) and answers at once, so the reset path is never reached. |
| `sub_8245D350` | `XmaObject::IsLocked` | Dropped | `src/hooks/xma.cpp`: returns 1 (nothing to wait for). The recompiled one, polled by the hardware wait (`sub_8245D288`): 0 while `CurrentContextIndex` (`7FEA1818`) is one of its contexts; then copies each hardware context's first 48 bytes into its entry, clears the addresses (+84, +88) of input buffers no longer valid, sets flag 0x20000 and returns 1. |
| `sub_8245D4D0` | `XmaObject::Unlock` | Overridden | `src/hooks/xma.cpp`: decodes each stream a second ahead of its position and frees a finished sound's buffer (moving to a waiting one). The recompiled one flushes each output buffer from the cache; with flag 0x20000 writes each copy back to its hardware context; writes each context's Kick register (`7FEA1940` + id / 32 × 4) so the hardware decodes on; clears 0x30000. Once a voice update. |
| `sub_824657E0` | `AudioRenderDriver::Register` | Overridden | `src/audio/native_audio.cpp` (`--native_audio`; `docs/native-audio.md`). r3 = the driver object (callback argument +12, handle +16), r4 = the mixer callback (0 only unregisters): registered with our audio output instead of the kernel's. |
| `sub_82465870` | `AudioRenderDriver::Submit` | Overridden | Same file. r4 = the mixed buffer (`sub_8245F718` gives its descriptor; samples at +8), handed to our audio output. |
| `sub_824658C8` | `AudioRenderDriver::Destroy` | Wrapped | Same file. Unregisters our handle first, so the original doesn't pass it to `XAudioUnregisterRenderDriverClient`. |
| `sub_8246A5F8` | | Overridden | `src/hooks/xma.cpp` over `audio::xma::BitOffsetOfSample` (`src/audio/xma_packets.cpp`); harness `xma`: 512 cases on made-up packets (1-3 streams), 0 differ. The recompiled one: XMA frame walk, sample to bit offset (r3 the data, r4 its size, r5 the stream, r6 the sample): from the stream's first 2 KB packet, adds each packet's frame count (header's top 6 bits) until the packet holding frame r6 / 512, skipping other streams' packets (header's low 8 bits), then walks that packet's frames (15-bit lengths) from its first frame (header bits, + 32); returns the bit offset from the data's start, or 0 past the end. From `sub_8246B480` and `sub_8246BB98`. |
| `sub_8246A698` | | | XMA frame walk to the data's end (r3 the data, r4 its size, r5 the stream): the bit offset of the stream's last frame. From `sub_8246BB98`. |
| `sub_8246A758` | | | The XMA source voice's release of its XMA object (`sub_8245C880`). From `sub_8246AAD0`. Not read in detail. |
| `sub_8246AEE0` | | | The XMA source voice's set-up: makes its XMA object (`sub_8245C650`). From `8246CE24`. Not read in detail. |
| `sub_8246B0C8` | | | Probably the XMA source voice's stop: lock, wait (`sub_8245D288`), `sub_8245CE98` on its streams. From `sub_8246B408` and `sub_8246B450`. Not read in detail. |
| `sub_8246B1C8` | | | Probably the XMA source voice's flush: reads the input position (`sub_8245D070`) between lock and unlock. No direct callers (a vtable). Never called (the user's session, 2026-10-21, a counter). Not read in detail. |
| `sub_8246B480` | | | The XMA source voice's input side, once a voice update (voice in r3; its buffer queue at +20: address +8, size +12, loop count +16, state +116: 0 new, 3 handed over, 4 done). Hands each new buffer over (`sub_8245C958`) with the output settings (`sub_8245D0E0`) and, with a loop count, the loop data (`sub_8245CF98`): bit offsets from the buffer (+40, +44), or with voice +266 = 2 from sample positions through `sub_8246A5F8`. A handed-over buffer is done once its address is no longer a valid input buffer (`sub_8245CA30`) or the error status (`sub_8245CF60`) has bit 8; then the buffer-end callback (voice +16's vtable +92). Returns 1 while a stream still has input. |
| `sub_8246BB98` | | | The XMA source voice's second loop, once a voice update: for the first handed-over buffer with +32 set, once its loop count has run out (`sub_8245D010`), sets a loop from sample +32 − 128 (`sub_8246A5F8`) to the data's end (`sub_8246A698`), count 1 (`sub_8245CF98`); once the input is gone, moves each stream's output by 128 samples (`sub_8245CE10`). Its loop and output moves never happened in the user's session (2026-10-21, a counter). |
| `sub_8246BE20` | | | The XMA source voice's start: makes its contexts (`sub_8245D138`). From `8245EC1C`, once a sound. Not read in detail. |
| `sub_8246BFD8` | | | The XMA source voice's end: releases its contexts (`sub_8245C8D0`). From `8245EA38` and `8246CADC`, twice a sound. Not read in detail. |
| `sub_8246C320` | `XmaVoice::Update` | | The XMA source voice's update, from the mixer callback, ~118 a second a voice (voice in r3, its XMA object at +184): lock (`sub_8245D208`), wait (`sub_8245D288`), output side (`sub_8246BB98`), reads the decoded samples (`sub_8245CB38`, `sub_8245CD68`, `sub_8245CBB0`, `sub_8245D050`) into the mix (`sub_8245F718`, `sub_8245FA10`), input side (`sub_8246B480`), unlock (`sub_8245D4D0`). Reaches the XMA object only through these helpers. |
