# Known functions: XACT's waves and voices

**XACT's waves and voices**: where XACT, the sound library linked into
the game (`xact.md`, translated as the game's code: the user's call,
2026-10-23), meets XAudio below it (`xaudio.md`). How the game's audio
works is in `../native-audio.md`. This file is three small classes:

- **The voice** (`XACTVoice`, 24 bytes, vtable `0x82075B98`,
  `825C9B60`-`825CA448`): XACT's wrapper over one XAudio source voice,
  made by the renderer's slot +4 (`XACTRenderer::CreateVoice`
  `sub_825C31D8`, `xact.md`) for each wave that plays. Each method is
  one or two XAudio API calls (`xaudio.md`): create, state, volume,
  pitch, submit a packet, start, stop, flush, release; its output and
  matrix methods (an optional reverb send) never ran in play. Beside
  it, the **reverb submix** (8 bytes, vtable `0x82075BD4`,
  `825CA4B8`-`825CA680`, made by the renderer's slot +8 `sub_825C3398`),
  never made in play.
- **The wave** (`XACTWave`, vtable `0x82076A38` for the base, made at
  `825D23F0`-`825D3B10`): one play of one wave bank entry. A wave holds
  its wave bank and entry index, its owner (the play-wave event of a
  sound's track, probably: `825D0000`-`825D1C10` and the events at
  `825D53D8`-`825D9000`, in `../loops/leads.md`), a state (1 preparing,
  2 prepared, 3 playing, 4 stopping, 5 stopped) and a voice. The game's
  banks are in memory, so every wave in play is the **in-memory wave**
  (96 bytes, vtable `0x82076A74`, made by `XACTWaveBank::CreateWave`
  `sub_825CECE8`, `xact.md`): its prepare (`sub_825D3B10`) makes the
  voice and submits the entry's data (PCM split into the intro and the
  loop region, or XMA with its loop count), the game thread's wave
  bank update (`sub_825D3890`) prepares the next wave of a loop or
  variation and releases finished ones, and the audio callback's
  (`sub_825D2560`, XAudio's thread, from `XACTWaveBank::AudioUpdate`)
  follows the voice's state and tells the owner when it's done. The
  **streaming wave** (vtable `0x82076AA4`, `825D3F28`-`825D5350`, with
  its stream entries, vtable `0x82076AD4`) never ran in play.
- **The variation tables** (vtables `0x82076A68` and `0x82076A6C`,
  `825D2AD8`-`825D3778`): helpers over a sound bank's wave variation
  and transition records, called from XACT's sound bank code and the
  events. None ran in play.

The wave's vtables, read from the loaded image
(`itc/xex_image/default.bin`, 2026-10-29): the base `0x82076A38` +0
`825D2A48`, +4 to +20 `825A7098` (probably `_purecall`), +24
`825D2778`, +28 `825D2300`, +32 `825D2380`, +36 `825D1ED8`, +40
`825D1C30`, +44 `825D1C50`; the in-memory wave `0x82076A74` +0
`825D3820` (the deleting destructor), +4 `825D3B10` (Prepare), +8
`825D3938` (Play), +12 `825D3A28` (Stop), +16 `825D43F0`, +20
`825C8388` (returns 0, `vehicle-accessors.md`), +24 `825D3890`
(Update), +28 to +44 as the base's; the streaming wave `0x82076AA4` +0
`825D4690`, +4 `825D4B78`, +8 `825D5058`, +12 `825D4300`, +16
`825D43F0`, +20 `825D4530`, +24 `825D46E8`, +28 `825D2300`, +32
`825D2380`, +36 `825D1ED8`, +40 `825D41F0`, +44 `825D3FD0`. The wave's
second vtable at +12, `0x82076A70` (one slot, `825D24B0`), is its
listener interface on the wave bank (`AddListener` at construction,
`RemoveListener` at release). The voice's vtable `0x82075B98`: +0
`825C9DB8` (GetState), +4 `825C9BE8` (SetVolume), +8 `825C9BF0`
(SetPitch), +12 `825C9E08` (SubmitPacket), +16 `825C9BF8` (Start), +20
`825C9C10` (Stop), +24 `825C9C28` (FlushPackets), +28 `825C9ED8`, +32
`825C9D18`, +36 `825C9EE0`, +40 `825C9E90`, +44 `825CA3B8`, +48
`825C9EE8`, +52 `825C9C30`, +56 `825CA448` (the deleting destructor);
the reverb submix's `0x82075BD4`: +0 `825CA560`, +4 `820911F8` (a
shared getter of +4), +8 `825CA680`.

Shared with other files, which keep their rows: `XACTRenderer::CreateVoice`
(`sub_825C31D8`), `XACTWaveBank::CreateWave` (`sub_825CECE8`),
`XACTWaveBank::AudioUpdate` (`sub_825CC5C0`), `XACTWaveBank::GetWaveFormat`
(`sub_825CC320`), `XACTSoundBank::GetWaveBank` (`sub_825C7218`),
`XACTEngine::SendNotification` (`sub_825C1388`), `XACTGetTimeMs`
(`sub_825BFE28`), `sub_825C0708` (the category flag), the wave entry's
accessors (`XACTWaveEntry::`, `825CED88`-`825CF288`), `XACTSound::GetVolume`
and `GetPitch` (`sub_825CB5E8`, `sub_825CAC48`) and the folded getters
`sub_82099BA0`, `sub_820A8958`, `sub_82379D50` in `xact.md`;
`sub_825C8388` (returns 0) in `vehicle-accessors.md`; XAudio's API
entry points the voice calls in `xaudio.md`; `XMemAlloc`, `XMemFree`,
`operator new`, `operator delete` in `heap.md`; `memset`, `memcpy` in
`c-runtime.md`. The rows were drafted by the B7 loop's mapping agent
(2026-10-29) from the generated code and the loaded image; nothing here
ran differently.

**The locks.** The voice takes none itself: each method calls an XAudio
API entry point (`xaudio.md`), which takes XAudio's API lock (and,
below it, the voice lists' spin lock) as the original does; those stay
the game's, taken as the original does (the user's calls, 2026-10-29).
XACT's own lock is the engine's critical section (`XACTEngine` +144,
`xact.md`'s intro), entered here by one function, `sub_825D1C78` (the
engine's +184 set under it, from the wave's Play and resume). Its
callers already hold it (`XACTCue::Play` takes it, then plays the
sound, its tracks and their waves: the wave's Play enters it again, on
purpose), and it's entered inline (`RtlEnterCriticalSection` called
directly by each user: 62 functions in XACT's range call it, 23 of
them seen in play, all recompiled; the engine's lock in those read).
So the precedent for a lock taken inline whose users aren't all ours
holds (XAudio's voice-list spin lock and its API lock, the user's
calls, 2026-10-29, "Precedent first" in `../loops/b7-loop.md`): native
code takes the game's own critical section exactly as the original
does (the kernel imports called at their thunks, as `xaudio::ApiEnter`
does), and it becomes a host recursive lock whose depth can be read
once every user is native. The audio callback's wave update
(`sub_825D2560`) runs with that lock held by the callback
(`XACTEngine::AudioCallback`) and takes none itself; the wave's two
request words (+28 and +32) are set with atomic exchanges (`lwarx` and
`stwcx.`) from the game's thread and read the same way on XAudio's.

Making it native (B7 loop, 2026-10-29, `--native_xact_waves`; plain C++
in `src/xact/waves.h`, adapters in `src/hooks/xact_waves.cpp`, harness
`xact_waves`): the voice's ten methods seen in play are ours, calling
XAudio's entry points' plain C++ directly in frames of the originals'
sizes below the caller's (`xaudio::FrameBelow`), with the registers set
as the original sets them, as the car sounds do. Then the waves'
constructors, the track's volume and pitch getters, `GetStartPosition`,
`RequestNext` and `XACTEngine::RequestCommit` (XACT's lock entered as
the original does, through its imports at their thunks); then the
wave's `SetVoice`, `Play`, `Pause`, `PrepareNext` and the in-memory
wave's `Update` (the voice's methods
through their plain C++ while its slots hold them, the owner's, the
renderer's and the banks' slots through the guest call). Before, the
voice's methods were ready (XAudio's entry points are ours, `--native_xaudio`);
the waves call through their owner's vtable (the tracks' and events'
code, still recompiled), the renderer's slots +4, +12 and +24
(`xact.md`, recompiled) and the voice's (this file), so a native wave
calls those through its adapter's guest call until they're ours, or
runs natively only while each slot holds the function already made
native (reading the slot's word first, as `xact.md`'s native functions
do).

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
| `sub_825C9C40` | `XACTVoice::Create` | Overridden | r3 = XAudio's source voice description (built by `XACTRenderer::CreateVoice` `sub_825C31D8`, `xact.md`), r4 = where the reverb submix's holder is, r5 = where the reverb send level is (a float), r6 = a flag (send to the reverb), r7 = where the voice goes. XMemAlloc(24, 0x2084802A); E_OUTOFMEMORY (0x8007000E) and 0 to [r7] when it fails. Sets the vtable `0x82075B98`, +4 and +8 = 0, byte +12 = 0, +16 = 1.0 (the float at `0x82000A30`), +20 = 1, then `XACTVoice::Init` with r3 to r6; on success the voice to [r7], else slot +56 (the deleting destructor) with 1 and 0 to [r7]. Returns Init's result. Only caller `sub_825C31D8`. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceCreate` (`src/xact/voices.cpp`; XMemAlloc's plain C++, Init's in a frame below its 144-byte one, the deleting destructor's while slot +56 holds it), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`, the original without the native heap); harness `xact_waves` (real voices over the live engine, without a reverb as in play). Seen in play. |
| `sub_825C9B60` | `XACTVoice::Init` | Native | r3 = the voice, r4 = the description, r5, r6, r7 as `Create`'s r4 to r6. `XAudioCreateSourceVoice` (`sub_8245A768`, `xaudio.md`) into +4; on failure returns its error. Byte +12 = the description's byte +4 (the channel count for PCM, the stream count for XMA, probably). When r7 is set: +8 = the holder's slot +4 (the reverb submix wrapper's `sub_820911F8`: its XAudio submix voice), +16 = [r5], then slot +32 (`XACTVoice::SetOutputs`) with 0, and returns its result; else returns the create's. Only caller `XACTVoice::Create`. In play r7 was always 0 (`SetOutputs` never ran). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceInit` (`src/xact/voices.cpp`; XAudioCreateSourceVoice's plain C++ in a frame below its 128-byte one; the reverb's two calls, never made in play, through the guest call), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves` (through Create). Seen in play. Adapter removed (2026-10-29, a temporary counter in an attract race saw no other caller): only `XACTVoice::Create`'s plain C++ calls it in play, so `--native_xact_waves` now switches only that caller; the harness checks it through Create. |
| `sub_825C9DB8` | `XACTVoice::GetState` | Overridden | Slot +0 of `0x82075B98` (no direct callers): r4 = a word out. `XAudioVoice::GetState` (`sub_82459FA0`) on +4 into a stack byte; on success [r4] = 1 when its bit 0 (playing) is clear, else 0 (so 1 means stopped). Returns XAudio's result. From the wave's audio update (`sub_825D2560`). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceGetState` (`src/xact/voices.cpp`; XAudio's GetState plain C++ in a frame below its 112-byte one), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825C9BE8` | `XACTVoice::SetVolume` | Overridden | Slot +4: a tail call of `XAudioSourceVoice::SetVolume` (`sub_8245A2C8`) on +4 with f1. From the waves' Prepare (`sub_825D3B10`) and probably the tracks' volume updates (`825D0000`-`825D1C10`, through the owner). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceSetVolume` (`src/xact/voices.cpp`; XAudio's SetVolume plain C++, its frame below the caller's), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825C9BF0` | `XACTVoice::SetPitch` | Overridden | Slot +8: a tail call of `XAudioSourceVoice::SetFrequencyRatio` (`sub_8245A340`) on +4 with f1. From the waves' Prepare and probably the tracks' pitch updates. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceSetPitch` (`src/xact/voices.cpp`; XAudio's SetFrequencyRatio plain C++), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825C9E08` | `XACTVoice::SubmitPacket` | Overridden | Slot +12: r4 = XACT's 40-byte packet, r5 a flag. Copies the packet's words +0, +4, +8 (255 becomes -1: loop forever), +20, +24, +28, +32, +36 into XAudio's 32-byte packet on the stack (+12 and +16 left out), then `sub_8245A108` (the codec's slot +36, `xaudio.md`) on +4 with it and (r5 != 0). Returns its result. From the waves' Prepare (one or two packets a wave). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceSubmitPacket` (`src/xact/voices.cpp`; the codec's +36 entry point's plain C++ in a frame below its 128-byte one), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825C9BF8` | `XACTVoice::Start` | Overridden | Slot +16: a tail call of `XAudioVoice::Start` (`sub_8245A168`) on +4 with (r4 != 0) (bit 0: at the next commit). From the wave's Play and resume (`sub_825D3938`, `sub_825D1ED8`, deferred: r4 = 1, with the engine's +184 set for `XACTRenderer::CommitVoices`). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceStart` (`src/xact/voices.cpp`; XAudio's Start plain C++), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves` (deferred starts on playing voices only, so the engine's pending list isn't touched). |
| `sub_825C9C10` | `XACTVoice::Stop` | Overridden | Slot +20: a tail call of `XAudioVoice::Stop` (`sub_8245A1B0`) on +4 with (r4 != 0) (bit 0: at once). From the wave's Stop and Pause (`sub_825D3A28`, `sub_825D1ED8`, r4 = 1). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceStop` (`src/xact/voices.cpp`; XAudio's Stop plain C++), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825C9C28` | `XACTVoice::FlushPackets` (probably) | Overridden | Slot +24: a tail call of `sub_8245A208` (`XAudioSourceVoice::FlushPackets`, probably: no more packets to come) on +4. From the wave's Play and resume, after the start. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceFlushPackets` (`src/xact/voices.cpp`; XAudio's FlushPackets plain C++), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825C9ED8` | `XACTVoice::SetOutputVoices` | | Slot +28: a tail call of `XAudioVoice::SetOutputVoices` (`sub_82459EF0`) on +4 with r4. From the wave's voice set-up (`sub_825D1DF8`, when the cue's +348 is set). Not seen in play. |
| `sub_825C9D18` | `XACTVoice::SetOutputs` | | Slot +32: r4 = a channel matrix (or 0). Builds the outputs on the stack: entry 0 {voice 0 (the default output, probably the mastering voice), r4}, and when +8 (the reverb submix) is set entry 1 {+8, 0} (count 2); `XAudioVoice::SetOutputVoices` (`sub_82459EF0`) on +4; on success with +8 set, slot +44 (`SetReverbSend`) with (this + 16, 1). Returns its result. From `XACTVoice::Init` (r4 = 0), `SetChannelMatrix` and the wave's voice set-up (when the cue's +316 is set). Not seen in play. |
| `sub_825C9EE0` | `XACTVoice::SetOutputMatrices` | | Slot +36: a tail call of `XAudioVoice::SetOutputMatrix` (`sub_82459F48`) on +4 with r4. No callers read. Not seen in play. |
| `sub_825C9E90` | `XACTVoice::SetMainMatrix` (probably) | | Slot +40: r4 = a channel matrix. `XAudioVoice::SetOutputMatrix` (`sub_82459F48`) on +4 with one entry {output 0, r4}. From `SetChannelMatrix` (after the first time). Not seen in play. |
| `sub_825CA3B8` | `XACTVoice::SetReverbSend` (probably) | | Slot +44: r4 = a float (the send level), r5 unused. +16 = [r4]; a matrix on the stack with one 8-byte entry per channel (byte +12 of them: {byte i, the level}), set for output 1 (the reverb submix) through `XAudioVoice::SetOutputMatrix` (`sub_82459F48`). Returns its result. From `XACTVoice::SetOutputs`. Not seen in play. |
| `sub_825C9EE8` | `XACTVoice::SetChannelMatrix` (probably) | | Slot +48: r4 = the source channels (1 to 8, else E_FAIL), r5 = the output channels (must match the mastering voice's: `XAudioGetMasteringVoice` `sub_8245A568`, `XAudioVoice::GetOutputFormat` `sub_8245A3B8`; else E_INVALIDARG), r6 = a matrix of gains (non-null, else E_INVALIDARG). Maps each output channel to its sources by the two channel counts' speaker masks (`sub_825C9AF8`), folding missing speakers (sums of two at 0.5, the float at `0x82001BE4`), into 8-byte entries {byte source, byte output, float gain} (XMemAlloc of 8 × +12 × r5 bytes, 0x20840001, freed after); then slot +32 (`SetOutputs`) the first time (+20 set, then cleared) or slot +40 after. From the wave's voice set-up (`sub_825D1DF8`, when the cue's +72 is set: its +68 channels and +64 matrix). Not read in detail. Not seen in play. |
| `sub_825C9AF8` | `XACTSpeakerMask` (probably) | | r3 = a channel count: the speaker mask (1 → 0x4, 2 → 0x3, 3 → 0xB, 4 → 0x33, 6 → 0x3F, 8 → 0x63F, else 0). Only caller `XACTVoice::SetChannelMatrix`. Not seen in play. |
| `sub_825C9C30` | `XACTVoice::ExitLoop` (probably) | | Slot +52: a tail call of `sub_8245A270` (the codec's slot +72, `xaudio.md`) on +4 with 0. From the wave's Stop (`sub_825D3A28`, a stop that plays out with r5 set: the loop count zeroed first). Not seen in play. |
| `sub_825CA448` | `XACTVoice::~XACTVoice` (deleting) | Overridden | Slot +56: r4 = flags. The vtable put back, `XAudioVoice::Release` (`sub_82459E18`) on +4 when set, and XMemFree(this, 0x2084802A) when r4 bit 0. Returns this. From `XACTVoice::Create`'s failure and the renderer's voice release (`XACTRenderer::ReleaseVoice` `sub_825C2F00`, `xact.md`, through the slot). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::VoiceDeletingDestruct` (`src/xact/voices.cpp`; XAudio's Release plain C++ in a frame below its 112-byte one, XMemFree's), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`, the original without the native heap); harness `xact_waves`. |
| `sub_825C3398` | `XACTRenderer::CreateReverb` (probably) | | Slot +8 of the renderer's vtable `0x820753F0` (no direct callers): r4 = where the reverb submix goes. The first time (bit 0 of `[0x82971494]`) stores the address of a stack descriptor {byte 3, word 0} in `[0x82971490]` (the effect the chain at `0x82075410` names: {count 1, `0x82971490`}); builds a submix description on the stack (+1 = 2, +4 = 48000, +8 = 1, +12 = `0x82075410`), makes the wrapper (`XACTReverb::Create`), adds it to the renderer's second voice pool (+228, `sub_825C60C0`) and stores it in [r4]. Returns the create's result. Not read in detail. Not seen in play. |
| `sub_825CA4B8` | `XACTReverb::Create` (probably) | | r3 = XAudio's submix voice description, r4 = where the wrapper goes. XMemAlloc(8, 0x2084802B) (E_OUTOFMEMORY and 0 to [r4] when it fails); vtable `0x82075BD4`, +4 = 0; `XAudioCreateSubmixVoice` (`sub_8245A7C8`) into +4; on success the wrapper to [r4], else slot +8 with 1 and 0 to [r4]. Only caller `sub_825C3398`. Not seen in play. |
| `sub_825CA560` | `XACTReverb::SetParameters` (probably) | | Slot +0 of `0x82075BD4`: r4 = a reverb parameter block. Two calls to `XAudioVoice::SetEffectParameters` (`sub_82459E80`) on +4: (0, 2, 1, the float r4 + 84 × 0.01, the float at `0x8200222C`: probably a wet-dry percentage), then (0, 0, 3, {a 48-byte block on the stack, 48}) with the block built from r4's words +0, bytes +4 to +48 and floats +52 to +80. Not read in detail. Not seen in play. |
| `sub_825CA680` | `XACTReverb::~XACTReverb` (deleting) | | Slot +8 of `0x82075BD4`: the vtable put back, `XAudioVoice::Release` (`sub_82459E18`) on +4 when set, XMemFree(this, 0x2084802B) when r4 bit 0. From `XACTReverb::Create`'s failure (through the slot). Not seen in play. |
| `sub_825D23F0` | `XACTWave::XACTWave` | Native | Base constructor: r3 = the wave, r4 = the engine (+20), r5 = the wave bank (+36), r6 = the wave index (+52, 16-bit), r7 = the owner (+48). +4 and +8 linked to itself (the node in the bank's wave list), vtables +0 `0x82076A38` and +12 `0x82076A70` (after `0x8203DF00` at +12), +16, +28, +32, +40, +44, +56, +60 and byte +24 zeroed, byte +25 kept to its bit 0x20; then the bank's +4 interface slot +0 (`AddListener`, `xact.md`) with this + 12. Returns this. From the in-memory and streaming waves' constructors. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::WaveConstruct` (`src/xact/waves.cpp`; the wave bank's AddListener `sub_825C8690` through its plain C++ while its +4 interface's slot +0 holds it and the native heap is on, else through the guest call), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. Seen in play. Adapter removed (2026-10-29, a temporary counter in an attract race saw no other caller): only `XACTInMemoryWave::XACTInMemoryWave`'s plain C++ calls it in play, so `--native_xact_waves` now switches only that caller; the harness runs its register step. |
| `sub_825D37A8` | `XACTInMemoryWave::XACTInMemoryWave` | Overridden | r3 = 96 bytes, r4 to r7 as the base's. The base constructor, then vtables +0 `0x82076A74` and +12 `0x82076A70`, +64 to +92 zeroed, +64 and +80 = this. From `XACTWaveBank::CreateWave` (`sub_825CECE8`, `xact.md`). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::InMemoryWaveConstruct` (`src/xact/waves.cpp`; the base constructor's plain C++ in a frame below its 96-byte one), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. Seen in play. |
| `sub_825D3820` | `XACTInMemoryWave::~XACTInMemoryWave` (deleting) | | Slot +0 of `0x82076A74`: r4 = flags. Its vtables put back, the base destructor (`sub_825D29E8`), XMemFree(this, 0x2084801A) when r4 bit 0. Returns this. No direct callers: from the wave's Update (`sub_825D3890`, a finished wave) and the next wave's failed Prepare (`sub_825D2778`). Runs in attract races. |
| `sub_825D29E8` | `XACTWave::~XACTWave` | | Base destructor: the base vtables put back (`0x82076A38`, `0x82076A70`), `XACTWave::Release` unless already released (byte +25 bit 0x4), then the vtable `0x82075344` (the list node's base, `xact.md`). From the in-memory and streaming waves' destructors. Runs in attract races. |
| `sub_825D2A48` | `XACTWave::~XACTWave` (deleting) | | Slot +0 of the base `0x82076A38`: as `sub_825D29E8` inline, then XMemFree(this, 0x20848066) when r4 bit 0. Not seen in play. |
| `sub_825D28D0` | `XACTWave::Release` | | Byte +25 \|= 0x4; unlinks the node +4/+8 from the bank's wave list (and links it to itself); deletes the next wave (+56: slot +0 with 1); the owner's (+48) slot +28; the voice (+44) given back through the renderer's slot +12 (`XACTRenderer::ReleaseVoice`, the engine +20, its +12); a state of 3 or 4 becomes 5 with the notification 13 (`sub_825D2030`); +40 = 5; then the bank's +4 interface slot +4 (`RemoveListener`) with this + 12. From the base destructor. Runs in attract races. |
| `sub_825D24D8` | `XACTWave::SetState` | | r4 = the new state (1 preparing, 2 prepared, 3 playing, 4 stopping, 5 stopped). Going to 3 from below 3: the notification 12 (probably XACT2's wave play); going to 5 from 3 or 4: the notification 13 (probably wave stop), each sent with +40 already at the new state; +40 = r4. From the waves' Prepare and Stop and the streaming wave's slots. Runs in attract races. |
| `sub_825D2030` | `XACTWave::Notify` | | r4 = the notification type. Only when the wave has a bank (+36): a 32-byte notification on the stack (zeroed: memset `sub_825A71D0`), the type, then with an owner the cue's (`[[[[owner + 44] + 16] + 20] + 48]`) sound bank (+296; none: nothing sent), cue index (+304) and the cue, else index -1; the wave bank (+36) and the wave index (+52); sent with `XACTEngine::SendNotification` (`sub_825C1388`, the engine +20) in r4 to r7. From `SetState`, the audio update and `Release`. Runs in attract races (the engine sends only registered types). |
| `sub_825D2170` | `XACTWave::GetStartPosition` | Overridden | r3 = the wave, r4 = the entry's accessor (`XACTWaveEntry`), r5 = a time offset (ms), r6 = the start (samples) out, r7 = the loop count byte (in and out). The offset in samples (`XACTWaveEntry::MsToSamples`) and the duration (`GetDuration`); with a count (the count byte only while byte +25's bit 0x20 is clear, else 0: corrected 2026-10-29 by the harness, the bit set skips the load), the loop region (`GetLoopStart`, and `GetLoopLength` with a loop region, else the whole duration): a finite count folds an offset inside the loops into one pass and lowers the count by the passes skipped, an offset past the loops lands after them with the count 0; 255 (forever) folds it into the loop. An offset past the duration: 0x8AC70019 (an XACT error). Returns 0 or that. From the waves' Prepare (`sub_825D3B10`; `sub_825D4B78` not seen). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::WaveGetStartPosition` (`src/xact/waves.cpp`; the entry accessor's leaves' plain C++; the original where it traps, the count 255 with no loop length), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825D3B10` | `XACTInMemoryWave::Prepare` | | Slot +4 of `0x82076A74` (no direct callers): r4 = the loop count byte (255 forever), r5 = loops (bool), r6 = the time offset (ms). +60 = 0, state 1, byte +24 = r4, byte +25 bit 0x20 = r5; with an owner, the sound's category flag (`sub_825C0708`, the sound's record +1). The entry's accessor (`XACTWaveEntry::XACTWaveEntry`, `Set` with the bank and index), the start (`GetStartPosition`) in bytes (`SamplesToBytes`), the entry's 40-byte region (the bank's slot +60: data, length, loop start, loop length), the format (`XACTWaveBank::GetWaveFormat`); a sound whose record has flag 0x10 sends to the reverb (the global settings' +8 and the sound's +116, else 1.0). The voice from the renderer's slot +4 (`XACTRenderer::CreateVoice`: the format, 2 packets when the start lies inside the loop region else 1, the category flag, the reverb's holder, level and flag), set up (`sub_825D1DF8`); then the packets through the voice's slot +12: XMA (format tag 1, `GetFormatTag`) one packet with the loop count and the start sample (rounded down to 128 above 128); PCM one packet from the start with the loop region and count, or, from inside the loop region, the rest of that pass then the loop region with the count less one (its region copied to +80 to +92, +88 = 1); +72 = 1, +64 the packets' context. On success +60 = 1, state 2, and with an owner the voice's volume (`sub_825D0268`) and pitch (`sub_825D02B0`). Returns 0 or the first error. Not read in detail. Runs in attract races. |
| `sub_825D1DF8` | `XACTWave::SetVoice` | Overridden | r4 = the voice (or 0): +44 = r4. With a voice and an owner, from the cue (`[[[[owner + 44] + 16] + 20] + 48]`): +348 set, the voice's slot +28 (`SetOutputVoices`) with the cue + 340; else +316 set, slot +32 (`SetOutputs`) with the cue + 308; then +72 set, the renderer's slot +24 (the output's channel count, probably) and the voice's slot +48 (`SetChannelMatrix`) with the cue's +68 (16-bit), that count and the cue's +64. In play none of the three was set. From the waves' Prepare and Update. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::WaveSetVoice` (`src/xact/waves.cpp`; the voice's slots +28, +32 and +48 and the renderer's +24 through the guest call), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves` (fake owner, renderer, banks and voices over AddRef recorders). Seen in play. |
| `sub_825D3938` | `XACTInMemoryWave::Play` | Overridden | Slot +8: r4 = deferred (bool). With an owner: its slot +100 when looping with a count, then its slot +96. Unless paused (byte +25 bit 0x80): the voice's Start (slot +16) with r4 (a failure returned at once); when deferred, `sub_825D1C78` (the engine's +184); byte +25 \|= 0x2 (started). Then byte +25 \|= 0x40 and the voice's slot +24 (`FlushPackets`). No direct callers (from the owner, probably the play-wave event). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::InMemoryWavePlay` (`src/xact/waves.cpp`; the voice's Start and FlushPackets through their plain C++ while its slots hold them, `RequestCommit`'s in a frame below, the owner's slots +100 and +96 through the guest call), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves` (fake owner, renderer, banks and voices over AddRef recorders); Play reads its voice unchecked (always one in play). Seen in play. |
| `sub_825D3A28` | `XACTInMemoryWave::Stop` | | Slot +12: r4 = the mode (0 at once, 1 as authored), r5 = exit the loop (bool). Nothing when stopped (5), or stopping (4) and already stopped at once (+16) or r4 set. From playing (3), or stopping with r4 = 0: r4 = 0 sets +16 = 1, state 4 and the voice's Stop (slot +20) at once; r4 = 1 sets state 4 and, with r5, byte +24 = 0 and the voice's slot +52 (`ExitLoop`, probably); other r4 nothing. From an earlier state: state 5 (`SetState`). Returns 0 or the voice call's result. No direct callers. Runs in attract races. |
| `sub_825D1ED8` | `XACTWave::Pause` | Overridden | Slot +36 (both waves): r4 = pause (bool). Nothing when the paused bit (byte +25 bit 0x80) already matches; else sets or clears it, and with a voice: pausing stops it at once (slot +20 with 1); resuming a started wave (0x2) starts it deferred (slot +16 with 1), `sub_825D1C78` (the engine's +184), and flushes again (slot +24) when it had been (0x40). Returns 0 or the voice call's result. No direct callers. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::WavePause` (`src/xact/waves.cpp`; the voice's Stop, Start and FlushPackets through their plain C++ while its slots hold them, `RequestCommit`'s in a frame below), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves` (fake owner, renderer, banks and voices over AddRef recorders). Not seen in the migration's attract race. |
| `sub_825D1C78` | `XACTEngine::RequestCommit` (probably) | Native | r3 = the engine: under its lock (the critical section at +144: `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`) sets +184 = 1 (deferred voices to start: `XACTEngine::DoWork` commits them through the renderer's slot +20). Called with the lock already held (from `XACTCue::Play`'s path): entered again on purpose. From the wave's Play and Pause (`sub_825D3938`, `sub_825D1ED8`; `sub_825D45D0` not seen). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::EngineRequestCommit` (`src/xact/waves.cpp`; the engine's critical section is the game's, entered and left through `RtlEnterCriticalSection` and `RtlLeaveCriticalSection` at their thunks as the original does, the user's call of 2026-10-29), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves` (a critical section of its own, held one time in two). Adapter removed (2026-10-29, a temporary counter in an attract race saw no other caller): only `XACTInMemoryWave::Play`'s and `XACTWave::Pause`'s plain C++ calls it in play, so `--native_xact_waves` now switches only those callers; the harness runs its register step. |
| `sub_825D1C30` | `XACTWave::RequestNext` (probably) | Overridden | Slot +40 of the base and the in-memory wave: an atomic exchange (`lwarx`, `stwcx.`) of +32 with r5 (r4 unused). The audio update reads +32 = 1 as: stop now and hand the next wave (+56) to the owner. No direct callers. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::WaveRequestNext` (`src/xact/waves.cpp`; a compare-and-swap on the big-endian word, as XAudio's AddStat), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825D1C50` | `XACTWave::RequestNotify` (probably) | | Slot +44 of the base and the in-memory wave: an atomic exchange of +28 with 1. The audio update reads +28 = 1 as: call the owner's slot +32. No direct callers. Not seen in play. |
| `sub_825D2560` | `XACTWave::AudioUpdate` | | The audio callback's per-wave step (XAudio's thread, the engine's lock held by `XACTEngine::AudioCallback`; from `XACTWaveBank::AudioUpdate` `sub_825CC5C0` for each wave in the bank's list +240). Nothing when released (0x4) or finished (0x10) or stopped (5). +32 taken atomically (1 → 0): state 5 (the notification 13 from 3 or 4), and with a next wave (+56): +56 = 0, the owner's slot +128 with it, the owner (+48) = 0, the next wave's Play (slot +8 with 1, 0, 0) and `XACTWave::Finish`; without a next wave, the owner's done or `Finish` as below. Else +28 taken atomically (1 → 0): the owner's slot +32. Then the voice's state (slot +0): playing while below 3, state 3 (the notification 12); stopped while 3 or more and not paused: state 5 (the notification 13 from below 5), then the owner's done (`sub_825D13F0`, the tracks lead) or, without an owner, `Finish`. Runs in attract races. |
| `sub_825D1FD8` | `XACTWave::Finish` | | Byte +25 \|= 0x10 (finished); with an owner, its slot +28, then +48 = 0. From the audio update (the next-wave hand-off, or no owner). Not seen in play. |
| `sub_825D3890` | `XACTInMemoryWave::Update` | Overridden | Slot +24 (no direct callers; probably from the wave bank's game-thread update, `0x82075AE8` slot +52, `xact.md`): `XACTWave::PrepareNext`, then a finished wave (0x10) deletes itself (slot +0 with 1); else a stopped wave (5) with a voice gives the voice back (the renderer's slot +12) and clears it (`SetVoice` with 0). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::InMemoryWaveUpdate` (`src/xact/waves.cpp`; `PrepareNext`'s and `SetVoice`'s plain C++ in frames below its 96-byte one, the wave's slot +0 and the renderer's +12 through the guest call), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. Seen in play. |
| `sub_825D2778` | `XACTWave::PrepareNext` | Native | Slot +24 of the base: when byte +25 has 0x8 (cleared) and the loop count byte +24 is nonzero: the owner's slot +108 (a bank byte and a 16-bit wave index), `XACTSoundBank::GetWaveBank` (`sub_825C7218`) on the cue's sound bank (+296), the bank's slot +40 (`XACTWaveBank::CreateWave`) with the index and the owner, then the new wave's Prepare (slot +4) with the count less one (255 kept), whether any is left, and 0; on success +56 = the new wave, linked at the tail of the bank's wave list (+240); else it's deleted (slot +0 with 1). From `XACTInMemoryWave::Update` and the streaming wave's (`sub_825D46E8`). In play 0x8 was never set: the check only. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::WavePrepareNext` (`src/xact/waves.cpp`; `XACTSoundBank::GetWaveBank`'s plain C++ where it answers with the native heap on, else through the guest call; the owner's slot +108, the wave bank's +40 and the new wave's +4 and +0 through the guest call), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves` (fake owner, renderer, banks and voices over AddRef recorders). Seen in play. Adapter removed (2026-10-29, a temporary counter in an attract race saw no other caller): only `XACTInMemoryWave::Update`'s plain C++ calls it in play (the streaming wave's Update `sub_825D46E8` never ran), so `--native_xact_waves` now switches only that caller; the harness runs its register step. |
| `sub_825D2300` | `XACTWave::GetLengthMs` (probably) | | Slot +28 (both waves): r4 = a loop count byte. The entry's accessor (as Prepare's), then ((r4 + 1) × the loop length + the loop start) / (the rate / 1000) (the rate from `sub_825CEE60`, probably): the length in ms through r4 + 1 passes of the loop. No direct callers. Not seen in play. |
| `sub_825D2380` | `XACTWave::GetDurationMs` (probably) | | Slot +32 (both waves): the entry's duration (`XACTWaveEntry::GetDuration`) / (the rate / 1000), in ms. No direct callers. Not seen in play. |
| `sub_825D24B0` | `XACTWave::OnBankEvent` (probably) | | Slot +0 of the listener interface `0x82076A70` (the wave's +12): r3 = this + 12, r4 = the caller: the caller's slot +4 with the wave (this − 12, or 0). Probably the wave bank's listener callback (the bank going away). Not read in detail. Not seen in play. |
| `sub_825D43F0` | `XACTWave::StopCue` (probably) | | Slot +16 (both waves): the cue's (`[[[[owner + 44] + 16] + 20] + 48]`) slot +4 (`XACTCue::Stop`) with (r4 != 0). No direct callers. Not seen in play. |
| `sub_825D0268` | `XACTTrack::GetVolume` (probably) | Overridden | r3 = `[[owner + 44] + 16]` (a track's object, probably: +20 the sound, +40 and +56 volume factors): returns in f1 its +56 × its +40 × `XACTSound::GetVolume` (`sub_825CB5E8`) of the sound. In the tracks' range (`825D0000`-`825D1C10`, `../loops/leads.md`) but called only by the waves' Prepare (`sub_825D3B10`; `sub_825D4B78` not seen). Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::TrackGetVolume` (`src/xact/waves.cpp`; `XACTSound::GetVolume`'s plain C++; the original where that declines), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825D02B0` | `XACTTrack::GetPitch` (probably) | Overridden | As `sub_825D0268` for the pitch: its +60 + its +44 + `XACTSound::GetPitch` (`sub_825CAC48`) of the sound (octaves). Called only by the waves' Prepare. Runs in attract races. Ours (B7 loop, 2026-10-29): `xact::TrackGetPitch` (`src/xact/waves.cpp`; `XACTSound::GetPitch`'s plain C++), its adapter in `src/hooks/xact_waves.cpp` (`--native_xact_waves`); harness `xact_waves`. |
| `sub_825D3F28` | `XACTStreamingWave::XACTStreamingWave` | | The base constructor, then vtables +0 `0x82076AA4` and +12 `0x82076A70`, byte +176 kept to its low four bits, +64 to +96, +116 to +152 and +160 to +172 zeroed. From `sub_825CF348` (the streaming wave bank, probably; `xact.md`'s range, not mapped). Not seen in play. |
| `sub_825D4020` | `XACTStreamingWave::~XACTStreamingWave` | | Base vtables put back, `XACTWave::Release`, the stream entries (+64, +68, +72) checked (`sub_825D5088`) and given back (`sub_825D50E8`, `sub_825CFAE0`), then the base destructor (`sub_825D29E8`). Not read in detail. From its deleting destructor. Not seen in play. |
| `sub_825D4690` | `XACTStreamingWave::~XACTStreamingWave` (deleting) | | Slot +0 of `0x82076AA4`: `sub_825D4020`, XMemFree(this, 0x2084801B) when r4 bit 0. Not seen in play. |
| `sub_825D4B78` | `XACTStreamingWave::Prepare` | | Slot +4 (685 lines): as the in-memory wave's Prepare over the stream (the start, the region, the format, the voice from the renderer, `SetVoice`, the volume and pitch), with the stream entries (`sub_825CF528`, `sub_825CEEE8`, `sub_825CFAE0`). Not read in detail. Not seen in play. |
| `sub_825D5058` | `XACTStreamingWave::Play` | | Slot +8: when prepared (2), `sub_825D45D0`; else byte +176 \|= 0x10 (play when prepared, probably). Returns 0 or its result. Not seen in play. |
| `sub_825D45D0` | `XACTStreamingWave::Start` (probably) | | r4 = deferred: as the in-memory wave's Play (the owner's slots +100 and +96, the voice's Start, `sub_825D1C78`, the flush). From `sub_825D5058` and `sub_825D46E8`. Not read in detail. Not seen in play. |
| `sub_825D4300` | `XACTStreamingWave::Stop` | | Slot +12 (140 lines): as the in-memory wave's Stop, with `SetState`. Not read in detail. Not seen in play. |
| `sub_825D4530` | `XACTStreamingWave::IsBusy` (probably) | | Slot +20: whether any of the stream entries +64, +68, +72 (`sub_825D5088`) is still reading (+176 bit 0x40 consulted). Not read in detail. Not seen in play. |
| `sub_825D46E8` | `XACTStreamingWave::Update` | | Slot +24 (647 lines): `PrepareNext`, the start once prepared (`sub_825D45D0`), the voice (`SetVoice`), the next read ranges (`sub_825D5258`), the loop's bytes (`sub_825D4420`), `SetState`, `sub_825D4128` and the stream entries (`sub_825CF528`, `sub_825CEEE8`). Not read in detail. Not seen in play. |
| `sub_825D4128` | | | From the streaming wave's Update (111 lines, one call through a vtable). Not read. Not seen in play. |
| `sub_825D4420` | | | From the streaming wave's Update (150 lines): the entry's accessor and its loop region in bytes (`HasLoopRegion`, `GetLoopLengthBytes`, `GetLoopLength`, `GetFormatTag`). Probably the loop's byte range for the reads. Not read in detail. Not seen in play. |
| `sub_825D41F0` | | | Slot +40 of the streaming wave (206 lines; calls `sub_825D1C10`). Not read. Not seen in play. |
| `sub_825D3FD0` | | | Slot +44 of the streaming wave: `sub_825D1C10`, then +28 = 1 atomically (as the base's slot +44). Not seen in play. |
| `sub_825D1C10` | | | Byte +24 (the loop count) less one, unless 0 or 255 (forever). From the streaming wave's slots +40 and +44. Not seen in play. |
| `sub_825D5258` | | | r4 = a position: when past +88, the next read range at +12 and +16 (+84 + +88, and the rest up to +80), returned; else 0. From the streaming wave's Update. Not read in detail. Not seen in play. |
| `sub_825D52A8` | `XACTStreamEntry::XACTStreamEntry` (probably) | | r4 = the wave bank (+108), r5 = a wave index (+112, 16-bit). Vtable `0x82076AD4`, +4 and +8 linked to itself, +52 (20 bytes, memset `sub_825A71D0`), +12 to +48 and +72 to +88 zeroed. From `sub_825CF598` and `sub_825CFB28` (the streaming wave bank, probably; not mapped). Not seen in play. |
| `sub_825D5350` | `XACTStreamEntry::~XACTStreamEntry` (deleting) | | Slot +0 of `0x82076AD4`: frees its buffer (+84, XMemFree 0xAB840003), the vtable `0x82075344`, XMemFree(this, 0x20848024) when r4 bit 0. Not seen in play. |
| `sub_825D50F8` | `XACTStreamEntry::SetUp` (probably) | | r4 = the buffer's size: XMemAlloc(r4, 0xAB840003) into +84, +76 = r4, +12 to +48 zeroed; the entry's region and loop in bytes (`GetLoopLength`, `GetLoopStartBytes`, `GetLoopStart`, `GetLoopLengthBytes`, `GetPlayLength`, `GetDuration`, `GetFormatTag`). From `sub_825CF598` and `sub_825CFB28`. Not read in detail. Not seen in play. |
| `sub_825D5088` | `XACTStreamEntry::IsReading` (probably) | | When +72 is 1: the bank's (+108) slot +64 with the index (+112), +52 and a word out; 1 when it returns 0. Else 0. From the streaming wave and `sub_825CF8A8`, `sub_825CFF90`. Not seen in play. |
| `sub_825D50E8` | `XACTStreamEntry::Release` (probably) | | `sub_825CFF70` with the bank (+108) and the entry. From the streaming wave's destructor and `sub_825CF8A8`. Not seen in play. |
| `sub_825D3360` | `XACTVariationTable::XACTVariationTable` (probably) | | r4 (+4), r5 = a variation record (+8: its word +0 holds the entry count in bits 0-15, the selection type in bits 16-18, the entry format in bits 19-21). Vtable `0x82076A68`, +12 = 0; for selection types 2 and 3 (random, probably), unless the format is 3, +12 = the sum of the entries' weight bytes (+4 of 5-byte entries for format 0, +5 of 6-byte ones otherwise, from +8). From `sub_825CD020`, `sub_825CE2F8` (`xact.md`'s range, not mapped) and `sub_825D7AA0` (an event). Not seen in play. |
| `sub_825D2AD8` | `XACTVariationTable::~XACTVariationTable` | | Puts back the vtable `0x82076A68`. From `sub_825D7BA0` (an event). Not seen in play. |
| `sub_825D2D18` | `XACTVariationTable::~XACTVariationTable` (deleting) | | Slot +0 of `0x82076A68`: the vtable, XMemFree(this, 0x20848017) when r4 bit 0. Not seen in play. |
| `sub_825D3428` | `XACTVariationTable::Next` (probably) | | By the selection type (bits 16-18 of the record, a five-way switch): the next entry index (in order, from a random start, random by weight `sub_825D30B0`, `sub_825D3178`, with `sub_825BF650`), stored at the record's +4 (16-bit) and returned. Not read in detail. From `sub_825D35A8` and `sub_825D3618`. Not seen in play. |
| `sub_825D35A8` | | | `sub_825D3428` and more (65 lines). From `sub_825D7698` (an event). Not read. Not seen in play. |
| `sub_825D3618` | | | `sub_825D3428` and more (65 lines). From `sub_825CD020`. Not read. Not seen in play. |
| `sub_825D30B0` | | | From `XACTVariationTable::Next` (111 lines, `sub_825BF650`): probably a weighted random pick. Not read. Not seen in play. |
| `sub_825D3178` | | | From `XACTVariationTable::Next` (254 lines, `sub_825BF650`, memcpy): probably a random pick without repeats or a shuffle. Not read. Not seen in play. |
| `sub_825D31D8` | | | No direct callers (185 lines, `sub_825BF650`, memcpy); like `sub_825D3178`. Not read. Not seen in play. |
| `sub_825D3470` | | | A fragment the recompiler split out of `sub_825D3428` (one of its switch cases: the next index in order, wrapping). No callers. Not seen in play. |
| `sub_825D3338` | `XACTVariationTable::UsesWaveBank` (probably) | | By the entry format (bits 19-21 of the record's word: 0, 1 or other): `sub_825D2EF0`, `sub_825D2F90` or `sub_825D3030`. From `sub_825CDCC8` (`xact.md`'s range, not mapped). Not seen in play. |
| `sub_825D2EF0` | | | r4, r5 = a wave bank: for each 5-byte entry (from the record's +8), the entry's bank byte (+2) through `XACTSoundBank::GetWaveBank`; 1 when one is r5. Probably whether a variation plays from that bank. Not read in detail. From `sub_825D3338`. Not seen in play. |
| `sub_825D2F90` | | | As `sub_825D2EF0` over 6-byte entries, through `sub_825D2B80`. From `sub_825D3338`. Not read in detail. Not seen in play. |
| `sub_825D3030` | | | As `sub_825D2EF0` over another entry format (memcpy, `sub_825D2B80`). From `sub_825D3338`. Not read in detail. Not seen in play. |
| `sub_825D2B80` | | | 228 lines (`sub_825D2AE8`, `XACTSoundBank::GetWaveBank`, four calls through vtables). From `sub_825D2F90` and `sub_825D3030`. Not read. Not seen in play. |
| `sub_825D2AE8` | | | r4 = a track's wave list (16-bit count at +2, 5-byte entries from +8), r5 = a sound bank, r6 = a wave bank: 1 when an entry's bank byte (+2) is r6 (`XACTSoundBank::GetWaveBank`). From `sub_825D2B80`. Not seen in play. |
| `sub_825D2D68` | | | f1 = a value, r5 and r6 = a 16-bit and a word out (both -1 first): a 2 × count-byte array (`operator new`), the entries walked (`sub_825BF6B8`), memcpy, `operator delete`. Probably a variation picked by a variable's value (interactive variations). Not read in detail. From `sub_825CE7C8` (not mapped). Not seen in play. |
| `sub_825D2EA0` | | | r4 = an entry index: in the 16-byte entries at [r3 + 8], 1 when the entry's +20 is set and f1 lies in [+12, +16] (floats). From `sub_825CE7C8`. Not seen in play. |
| `sub_825D3688` | `XACTTransitionTable::XACTTransitionTable` (probably) | | Vtable `0x82076A6C`, +4 = r4 (a table of 26-byte records), +8 = r5 (16-bit, a count). From `sub_825CE2F8`. Not seen in play. |
| `sub_825D36A0` | `XACTTransitionTable::~XACTTransitionTable` (deleting) | | Slot +0 of `0x82076A6C`: the vtable, XMemFree(this, 0x2084802D) when r4 bit 0. Not seen in play. |
| `sub_825D36F0` | `XACTTransitionTable::Entry` (probably) | | r4 = a source index (0xFFFF: the count), r5 = a destination index (0xFFFF: the count less one): the 26-byte record at +4 + 4 + 26 × (source × count + destination, the destination less one when above the source). Not read in detail. From `sub_825D3778`. Not seen in play. |
| `sub_825D3778` | | | `sub_825D36F0`, the record to [r6], returns 0. From `sub_825CE7C8`. Not seen in play. |
