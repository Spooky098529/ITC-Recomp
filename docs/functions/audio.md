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
| `sub_8245C638` | | | Reads the XMA decoder's register `7FEA1800` (audio, not the GPU; the SDK's XMA decoder answers it). Audio library (B2). Not read. |
| `sub_8245CE98` | | | The audio library's per-context reset after a hardware timeout (r3 = the object, r4 = the context index). Not read. |
| `sub_8245D138` | | | Writes an XMA decoder register (`7FEA18xx`, the address from a pointer). Audio library (B2). Not read. |
| `sub_8245D208` | | | Writes an XMA decoder register (`7FEA18xx`, the address from a pointer). Audio library (B2). Not read. |
| `sub_8245D288` | | Overridden | The audio library's wait for its hardware (r3 = the object): polls `sub_8245D350` until it answers (returns 0); after an eighth of a second (the performance counter's rate >> 3 timebase ticks) resets it: `sub_82106540`, Sleep(1000) (`sub_82295500`), 0 then `0x03000000` written to the XMA registers at `0x7FEA1804`, `sub_8245CE98` for each of the object's [+0] contexts; returns 1. ~200 calls a second in a race, from `0x8246C51C`. Ours (`src/hooks/clock.cpp`) keeps the steps as guest calls, the rate from `src/helpers/clock.h`. Audio library (B2). |
| `sub_8245D350` | | | Polled by the audio library's hardware wait (`sub_8245D288`, r3 = the object): nonzero once the hardware answers; reads the XMA decoder's registers (`7FEA1818` and a 16-bit one; the SDK's XMA decoder answers them). Not read in detail. |
| `sub_8245D4D0` | | | Writes an XMA decoder register (`7FEA18xx`, the address from a pointer). Audio library (B2). Not read. |
| `sub_824657E0` | `AudioRenderDriver::Register` | Overridden | `src/audio/native_audio.cpp` (`--native_audio`; `docs/native-audio.md`). r3 = the driver object (callback argument +12, handle +16), r4 = the mixer callback (0 only unregisters): registered with our audio output instead of the kernel's. |
| `sub_82465870` | `AudioRenderDriver::Submit` | Overridden | Same file. r4 = the mixed buffer (`sub_8245F718` gives its descriptor; samples at +8), handed to our audio output. |
| `sub_824658C8` | `AudioRenderDriver::Destroy` | Wrapped | Same file. Unregisters our handle first, so the original doesn't pass it to `XAudioUnregisterRenderDriverClient`. |
