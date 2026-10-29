# Known functions: XAudio's engine and mixer

**XAudio's engine** is the part of XAudio (the Xbox 360's low-level
audio library, linked into the game like XACT above it) under the API
entry points and voices of `xaudio.md`: the engine object made once by
XAudioInitialize (`sub_8245A410`, `sound-cues.md`), its object factory,
its allocators, the mastering voice that hands each mixed frame to the
render driver (`audio.md`, our audio output since `--native_audio`,
`../native-audio.md`), and the mix itself: the mixer's callback, the
engine's processor threads and their passes over the voices' stages.
The voices call into it only through the commit and the waits
(`xaudio.md`); the codecs, matrices and effects its factory makes
(`8246xxxx`) are a lead (`../loops/leads.md`). Translated as the game's
code (the user's call for XACT, 2026-10-23; followed for XAudio,
`xaudio.md`). Read by the B7 loop's mapping agent, 2026-10-29, from
the engine's lead; the engine's teardown (never run: the game never
shuts XAudio down) is read in outline only.

**The engine** (372 bytes, vtables `0x8204A6CC` then `0x8204A6E0`;
`XAudioEngine::XAudioEngine` `sub_8245C098`, layout in
`../known-data.md`) is made by `XAudio::CreateEngine` (`sub_82459D60`,
the function XAudioInitialize calls through `[0x826002AC]`) and kept at
`[0x82954F08]`. XAudioInitialize's parameters (12 bytes): +0 a byte,
the mix buffers' channels (at least 6), +1 the submix stage count, +2
the processor mask (the hardware threads XAudio mixes on; 16, thread 4,
when 0), +4 the factory's class table (`0x8204A7B0` when 0), +8 the
mastering voice's description (3 words, or none). The game's
(`AudioEngine::StartXAudio`, `sound-cues.md`) are probably 6, 1, 48
(threads 4 and 5).

**The mix.** Our output thread calls the mixer's callback
(`sub_8245FBE0` -> `XAudioEngine::Mix` `sub_8245BF80`) once a frame
(256 samples, 187.5 a second), which holds the mixer's lock (the
critical section `0x8264D4F8`) for the whole mix. With processor
threads (+304 their count; in play, two), it doesn't mix itself: it
wakes the master thread and waits for it, and the mix runs on XAudio's
own guest threads (`XAudio::MasterThread` `sub_8245BE08` on the mask's
main processor, `XAudio::WorkerThread` `sub_8245BED0` on each other;
made by `XAudioEngine::Init` with `ExCreateThread`, suspended, on that
hardware thread, priority 15). The master swaps each stage's lists
(`XAudioEngine::SwapStageLists`) and runs the category volume and the
passes (`sub_8245B808`, `audio.md`), which wake the workers; every
processor takes voices off a stage one at a time
(`XAudioEngine::RunStage`, under the spin lock) and meets the others
at a barrier after each stage (`XAudioEngine::Barrier`, a spin over an
8-byte word, one byte a processor). Then the callback runs the
mastering voice's pass (its slot +68), which submits the frame to the
render driver.

**The kernel objects the engine uses** (none has a precedent of its
own; for the user's call when a migration reaches them):

- `0x82954EF8`, a synchronization event (its header built inline:
  type 1, not signaled), "mix now": set by the mixer's callback
  (`KeSetEvent`) and by the engine's teardown; the master thread waits
  on it (`KeWaitForSingleObject`, no timeout).
- `0x82954EE8`, a synchronization event, "mix done": set by the master
  thread after each mix (and as it ends); the mixer's callback waits on
  it.
- `0x82954EC4`, a notification event (type 0), "title terminating": set
  by the title terminate routine (`sub_8245B0D0`, not seen in play); the
  callback's wait (`KeWaitForMultipleObjects`, wait any, with "mix
  done") returns 1 on it and the callback skips the mastering voice's
  pass.
- `0x82954ED4`, a kernel semaphore (`KeInitializeSemaphore`, count 0,
  limit 6): the workers' wake-up, released by `sub_8245B808` (`audio.md`)
  with the thread count less one, and by the master thread as it ends.
- `0x82954EB4`, a title terminate notification (`ExRegisterTitleTerminateNotification`,
  the routine `sub_8245B0D0` and priority `0x7D800000`), unregistered
  under the critical section `0x8264D4D8` (`sub_8245AA48`, not seen).
- The processor threads (`ExCreateThread`, `ObReferenceObjectByHandle`
  with the thread type (`[0x820007FC]`, probably `PsThreadObjectType`), `KeSetBasePriorityThread`,
  `KeResumeThread`, `ObDereferenceObject`; handles at the engine's
  +308 + 4 × the processor), each storing its thread (`[r13 + 256]`) at
  +332 + 4 × its processor (`[r13 + 268]`).
- The mixer's lock `0x8264D4F8` (a kernel critical section), only by
  the callback (and the teardown); the voice lists' spin lock
  `0x82954F10`, inline in most functions here, taken natively as the
  original does until every user is native (the user's call,
  2026-10-29, `xaudio.md`); the API lock `0x8264D518` only in
  XAudioInitialize (`sound-cues.md`).

`KeGetCurrentProcessType` (the processor set-up) and the debug monitor
data (`0x820007F0`, 0 without a debugger: the statistics' registration)
are read once at boot. The barrier's spin (`db16cyc`) and the threads'
processor numbers are part of the original's behavior that a migration
keeps (B7 changes no behavior).

Shared rows: XAudioInitialize (`sub_8245A760`, `sub_8245A410`) in
`sound-cues.md`; the category volume and passes (`sub_8245B808`) and
the XMA set-up (`sub_8245C638`), the render driver's register, submit
and destroy (`sub_824657E0`, `sub_82465870`, `sub_824658C8`) in
`audio.md`; `XAudio::Alloc` (`sub_8245EF80`), `XAudio::AddStat`, the
waits, the buffers, the voices' base, `XAudio::GetAllocator` and
`XAudio::DefaultAllocator` in `xaudio.md`; `memset` (`sub_825A71D0`) in
`c-runtime.md`; `sub_825C8388` in `vehicle-accessors.md`; `XMemAlloc`
and `XMemFree` in `heap.md`; `CloseHandle` (`sub_82294E20`) in
`functions.md` and `WaitForSingleObject` (`sub_822970A0`) in
`video-output.md` (the teardown's).

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
| `sub_82459D60` | `XAudio::CreateEngine` | | The function XAudioInitialize (`sub_8245A410`, `sound-cues.md`) calls through `[0x826002AC]` when there's no engine: r5 = the parameters, r6 = an allocator (XAudioInitialize's r4, 0 from `sub_8245A760`). The XMA set-up (`sub_8245C638`, Dropped in `audio.md`), then `XAudioEngine::Create` (parameters, allocator, out `0x82954F08`). Once at boot. Reached only through the pointer. |
| `sub_8245C538` | `XAudioEngine::Create` | | r3 = the parameters, r4 = an allocator (0: the default), r5 = the engine out. `XAudioEngine::Size`; `XAudio::GetAllocator` (`xaudio.md`; attributes `0x61820005`, the size + 372 when that succeeded); 372 bytes from the allocator's slot +20; `XAudioEngine::XAudioEngine`; `XAudioEngine::Init`; on success the engine to [r5], else its slot +12 (probably `sub_8245AAB8`, the delete); the allocator released (slot +4) either way. E_OUTOFMEMORY when the allocation fails. From `XAudio::CreateEngine`, once at boot. |
| `sub_8245B2C0` | `XAudioEngine::Size` | Overridden | r3 = the parameters, r4 = the size out. The factory's size (`XAudioFactory::SizeOf` with the class table, +4 or `0x8204A7B0`); an output buffer's size (`XAudioBuffer::Size`, `xaudio.md`) for a 48,000 Hz format of max(+0, 6) channels; the processors (`XAudio::SetUpProcessors` with +2); the size = the buffer's × the processor count (byte +1 of the processors) × 2 + the factory's, + 44 × the stage count (+1) + 4 when there are stages. Returns the buffer size's result (or the factory's failure). From `XAudioEngine::Create`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::EngineSize`, with `XAudioFactory::SizeOf`'s, `XAudioBuffer::Size`'s (`xaudio.md`; its original where it refuses, which a 48,000 Hz format never makes it) and `SetUpProcessors`' plain C++ (in a frame below), in a frame of the original's size (176 bytes); `--native_xaudio_engine`. Harness `xaudio_engine` (48 cases: channels 0-11, stage counts, masks, the default or a given class table). |
| `sub_8245DBD0` | `XAudioFactory::SizeOf` | Native | r3 = a pointer to the class table, r4 = the size out: `XAudioFactory::TableSize` + 24 (the 24-byte factory and its 8-byte entries); 0. From `XAudioEngine::Size`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::FactorySizeOf`, `--native_xaudio_engine`. Harness `xaudio_engine` (48 cases, as `TableSize`'s). Adapter removed (2026-10-29, migration 1; a temporary counter in an attract race saw no other caller): only `XAudioEngine::Size`'s plain C++ calls it in play, so `--native_xaudio_engine` now switches only that caller; the harness runs its register step. |
| `sub_8245D9E8` | `XAudioFactory::TableSize` | Native | r3 = a pointer to the class table (+0 the count byte, +4 the 12-byte entries, +0 each the class id): 8 × (the largest id + 1), 0 for no table. Leaf. From `XAudioFactory::SizeOf`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::FactoryTableSize` (`src/xaudio/engine.cpp`), `--native_xaudio_engine`. Harness `xaudio_engine` (48 cases: no pointer, no table, no classes, up to 39 random ids). Adapter removed (2026-10-29, migration 1; a temporary counter in an attract race saw no other caller): only `XAudioFactory::SizeOf`'s plain C++ calls it in play, so `--native_xaudio_engine` now switches only that caller; the harness runs its register step. |
| `sub_8245A828` | `XAudio::SetUpProcessors` | Overridden | r3 = the processor mask (low 6 bits; 0 means 16, thread 4), r4 = 8 bytes out, cleared. When `KeGetCurrentProcessType()` is 1 (the title's): for each of the 6 hardware threads in the mask, its byte (+2 + i) = 2 and +0, +1 counted up; then the main one = 1: thread 4 when the mask has it (+6), else the highest set. Otherwise +1 = 1, +6 = 4 (no thread). Value 1 gets the master thread, 2 a worker (`XAudioEngine::Init`). From `XAudioEngine::Size` and `XAudioEngine::Init`. Probably mask 48 in play (threads 4 and 5). Ours (2026-10-29, the B7 loop, migration 1): `xaudio::SetUpProcessors`, the kernel's `KeGetCurrentProcessType` called through the translated code at its import's thunk (`0x825DB934`) as the original calls it, in a frame of the original's size (112 bytes); r3 left the kernel's answer; `--native_xaudio_engine`. Harness `xaudio_engine` (80 cases: every 6-bit mask and random words; the kernel answers the title's process, so the other branch is checked by reading only). |
| `sub_8245C098` | `XAudioEngine::XAudioEngine` | Overridden | r3 = the engine (372 bytes), r4 = the allocator: vtable `0x8204A6CC`, +4 = 1 (its count), +8 the allocator (AddRef'd, slot +0); vtable `0x8204A6E0`; +68 the start-pending list (offset 24); +80 the source voices' stage (its list offset 16, two lists of offset 24 at +92 and +104, +116 and +120 pointing at them: the active and next lists); +132, +136 = 1.0 (the categories' volumes); then `[0x82954F08]` = the engine (before its `Init`). Leaf apart from the AddRef. From `XAudioEngine::Create`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::EngineConstruct` (`src/xaudio/engine.cpp`), the allocator's AddRef through the translated code, in a frame of the original's size (96 bytes); `--native_xaudio_engine`. Harness `xaudio_engine` (16 cases, with and without an allocator, run while the mixer's lock is held with our output thread waiting on it, so the [`0x82954F08`] it writes can be put back before the mixer reads it again). |
| `sub_8245C198` | `XAudioEngine::Init` | | r3 = the engine, r4 = the parameters. `XAudio::RegisterStats`; +128 = the stage count (+1) and +124 its stages (a count word, then 44 bytes a stage set up as +80's), from the allocator's slot +20; the factory (`XAudioFactory::Create`, class table +4 or `0x8204A7B0`) into +60; the kernel objects (the intro: the events `0x82954EF8` and `0x82954EE8` and the notification event `0x82954EC4` built inline, `KeInitializeSemaphore(0x82954ED4, 0, 6)`, the title terminate notification `0x82954EB4` registered); +304 = 0; `XAudio::SetUpProcessors` (+2); for each processor with a nonzero byte, a thread (`ExCreateThread`, suspended, on that hardware thread; `XAudio::MasterThread` for 1, `XAudio::WorkerThread` else), referenced, priority 15, resumed, dereferenced, +304 counted, its handle at +308 + 4i (0 for the others); two output buffers (`XAudioBuffer::Create`, `xaudio.md`, max(+0, 6) channels at 48,000 Hz) for each such processor at +12 + 8i and +16 + 8i; the mastering voice (`XAudioMasteringVoice::Create`, description +8) into +64; then its render driver's (+68) slot +28 (`AudioRenderDriver::Register`, `audio.md`) with the mixer's callback `0x8245FBE0`. E_OUTOFMEMORY when an allocation or a thread fails. From `XAudioEngine::Create`, once at boot. |
| `sub_8245F588` | `XAudio::RegisterStats` | Overridden | Hands the debug monitor (`[[0x820007F0]]` slot +24, request 47) the statistics' callback (`sub_8245F4E0`); nothing without a debugger (the data points to 0, as `sub_822B1E70`'s in `functions.md`). Returns 0. From `XAudioEngine::Init`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::RegisterStats`, the request written to the frame (96 bytes) and the monitor's slot through the translated code when there is one; `--native_xaudio_engine`. Harness `xaudio_engine` (4 cases without a debugger, as in play: the frame below r1 compared, but the prologue's saved lr). |
| `sub_8245F4E0` | | | The statistics' debugger callback: request 255 hands each counter (`0x82954F40`, the copies `XAudioEngine::TakeStats` makes) whose byte at `0x82954F5C` + i is set to `sub_825B9BA8` as a float; requests 16 and 17 clear and set that byte. Reached only through the debug monitor. Not seen in play. |
| `sub_8245F5E8` | `XAudio::UnregisterStats` | | As `XAudio::RegisterStats` with no callback. From the engine's teardown (`sub_8245B940`). Not seen in play. |
| `sub_8245DC10` | `XAudioFactory::Create` | Overridden | r3 = a pointer to the class table, r4 = the allocator, r5 = out: 24 bytes from the allocator's slot +20, `XAudioFactory::XAudioFactory`, the object + 8 (its interface, which `XAudioFactory::Create` and `Size` in `xaudio.md` take back by 8) to [r5]. E_OUTOFMEMORY. From `XAudioEngine::Init`. (Named as `xaudio.md`'s `sub_8245D630` and `sub_8245D610`, which are the interface's thunks; this is the factory's making.) Ours (2026-10-29, the B7 loop, migration 1): `xaudio::FactoryCreate`, the factory's constructor's plain C++ in a frame below; E_OUTOFMEMORY sign-extended in r3 as the original's `lis`/`ori` leave it; `--native_xaudio_engine`. Harness `xaudio_engine` (32 cases, with room for the factory and its entries or not for the factory). |
| `sub_8245DA48` | `XAudioFactory::XAudioFactory` | Native | r3 = the factory (24 bytes), r4 = a pointer to the class table, r5 = the allocator: vtable `0x8204A6F4`, +4 = 1, +8 the entry count (the largest id + 1), +12 its entries (8 bytes each, from the allocator's slot +20), each id's entry the table entry's two functions (+4, +8). Returns the factory. From `XAudioFactory::Create`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::FactoryConstruct`, the entries from the allocator's slot +20 through the translated code, the class table read again for each class as the original does; `--native_xaudio_engine`. Harness `xaudio_engine` (32 cases: up to 24 classes with ids below 40, none, from a bump allocator using the real `XAudioAllocator::Alloc`). Adapter removed (2026-10-29, migration 1; a temporary counter in an attract race saw no other caller): only `XAudioFactory::Create`'s plain C++ calls it in play, so `--native_xaudio_engine` now switches only that caller; the harness runs its register step. |
| `sub_8245D650` |  | Overridden | The factory's create slot (probably +24, reached from `xaudio.md`'s `sub_8245D630`): r3 = the factory, r4 = a descriptor (byte +0 the class id; below 128 in +8/+12, from 128 in +16/+20), r5: tail call to the class entry's first function with (descriptor, r5); 0x80004001 (E_NOTIMPL) when the id has no entry. Seen in play (the codecs and matrices made for each voice). Named `XAudioFactory::CreateObject` in its print. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::FactoryEntry` finds the function, the adapter moves the registers as the original (r3 the descriptor; r4 = r5 only on the call's path) and tail-calls it; E_NOTIMPL sign-extended; `--native_xaudio_engine`. Harness `xaudio_engine` (48 cases: ids below and from 128, in and past range, entries with a function missing; the functions a recompiled leaf storing r4 and r5). |
| `sub_8245D6D8` |  | Overridden | The factory's size slot (reached from `xaudio.md`'s `sub_8245D610`): as `sub_8245D650` with the entry's second function and (descriptor, r5, r6). Seen in play. Named `XAudioFactory::SizeObject` in its print. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::FactoryEntry` (the second function), the adapter moving r4 = r5 first and r5 = r6 for the call, as the original; `--native_xaudio_engine`. Harness `xaudio_engine` (48 cases, as the create slot's). |
| `sub_8245D760` | | | The factory: adds classes with ids from 128 (r4 = a table as the class table's: each entry given the next free id, 128 + its slot, written back into the table's id byte), growing +16/+20 by `XAudio::Alloc` and copying (`memcpy`); E_OUTOFMEMORY past 128. Probably a factory slot. Not seen in play. Not read in detail. |
| `sub_8245D970` | | | The factory: clears the +20 entries of a table's ids from 128 (the remove matching `sub_8245D760`). Leaf. Not seen in play. |
| `sub_8245DB60` | `XAudioFactory::~XAudioFactory` | | Vtable `0x8204A6F4`; +20 freed (`XAudio::Free`); vtable `0x8204A6B8`. Not seen in play. |
| `sub_824600F0` | `XAudioMasteringVoice::Create` | Overridden | r3 = its description (3 words, or 0), r4 = out (the engine's +64). `XAudioMasteringVoice::Size`; `XAudio::GetAllocator` (`0x61820006`, size + 72); 72 bytes from its slot +20; `XAudioVoice::Construct` (`xaudio.md`, r5 2: the type), vtable `0x8204A8E8`; `XAudioMasteringVoice::Init`; on success the voice to [r4], else its slot +12. The allocator released. E_OUTOFMEMORY. From `XAudioEngine::Init`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::MasteringCreate`, with `MasteringSize`, `XAudio::GetAllocator` (its original where the plain C++ can't, as `CreateSource`'s), `XAudioVoice::Construct` and `MasteringInit`'s plain C++ in frames below, `--native_xaudio_engine`. Seen in play once at boot. Harness `xaudio_engine` (8 cases: the whole voice made with its own default allocator, real objects throughout, compared with pointers into the allocator as offsets, then freed; type 2 joins no live list). |
| `sub_8245FF68` | `XAudioMasteringVoice::Size` | Native | r3 = the description, r4 = the size out: a 32-byte voice description (+1 6 channels, +4 48,000, +8, +16, +20 the description's three words), `XAudioVoice::BaseSize` (`xaudio.md`), + 20 (the render driver object). From `XAudioMasteringVoice::Create`. The description's first 24 bytes are cleared; +24 on (the output count and channels, the outputs) are left as the stack had them (`BaseSize` doesn't read them). Ours (2026-10-29, the B7 loop, migration 1): `xaudio::MasteringSize` (`src/xaudio/engine.cpp`), `XAudioVoice::BaseSize`'s plain C++ (`xaudio.md`) in a frame below, `--native_xaudio_engine`. Harness `xaudio_engine` (24 cases: no description, and descriptions without effects). Adapter removed (2026-10-29, migration 1; a temporary counter in an attract race saw no other caller): only `XAudioMasteringVoice::Create`'s plain C++ calls it in play, so `--native_xaudio_engine` now switches only that caller; the harness runs its register step. |
| `sub_8245FCA0` | `XAudioMasteringVoice::Init` | Native | r3 = the voice, r4 = its description: the same 6-channel 48,000 Hz description, `XAudioVoice::InitBase` (`xaudio.md`); 20 bytes from the allocator (+8) slot +20, `AudioRenderDriver::AudioRenderDriver` (type 2, the voice) into +68 (E_OUTOFMEMORY); its slot +32 (r4 the description, r5 the allocator); then the voice's slot +56 (`XAudioMasteringVoice::Start`). From `XAudioMasteringVoice::Create`. The driver's slot +32 is `sub_824657C8` (the vtable read in a harness run, 2026-10-29: `AudioRenderDriver::Register` through slot +28 with the description's +8, 0, so it only unregisters a handle, none at first); slot +56 of the mastering vtable is `XAudioMasteringVoice::Start`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::MasteringInit`, `XAudioVoice::InitBase`'s plain C++ in a frame below, the allocator, the driver's and the voice's slots through the translated code, `--native_xaudio_engine`. Harness `xaudio_engine` (16 cases over a real default allocator, its data zeroed as the game's, and too small for the buffer now and then; the voice's slot +56 a recorded leaf). Adapter removed (2026-10-29, migration 1; a temporary counter in an attract race saw no other caller): only `XAudioMasteringVoice::Create`'s plain C++ calls it in play, so `--native_xaudio_engine` now switches only that caller; the harness runs its register step. |
| `sub_82465938` | `AudioRenderDriver::AudioRenderDriver` | Native | r3 = the 20-byte driver object, r4 = a description (+4 the voice): vtables `0x8204AAEC` (+0) and `0x8204AAD0` (+4), +8 = 1, +12 = the voice (the callback's argument `audio.md` names). Leaf. From `XAudioMasteringVoice::Init`. Its methods (register, submit, destroy) are in `audio.md`. +4 is first written `0x8204AA58`, then `0x8204AAD0`; r3 left as it was. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::RenderDriverConstruct`, `--native_xaudio_engine`. Harness `xaudio_engine` (16 cases). Adapter removed (2026-10-29, migration 1; a temporary counter in an attract race saw no other caller): only `XAudioMasteringVoice::Init`'s plain C++ calls it in play, so `--native_xaudio_engine` now switches only that caller; the harness runs its register step. |
| `sub_8245FE20` | `XAudioMasteringVoice::Start` | Overridden | When not playing (+61 bit 0): its buffer (+32) cleared (`XAudioBuffer::ClearThunk`, flags 3), then `XAudioVoice::Play` (`xaudio.md`); 0. Slot +56 of `0x8204A8E8` (probably). From `XAudioMasteringVoice::Init` (once), and as an output's start (`XAudioVoice::Activate`). Ours (2026-10-29, the B7 loop, migration 1): `xaudio::MasteringStart`, `XAudioBuffer::ClearThunk`'s and `XAudioVoice::Play`'s plain C++ (`xaudio.md`; the voice lists' spin lock taken as the original does), `--native_xaudio_engine`. Harness `xaudio_engine` (24 cases, playing or not, over a buffer of 1 or 2 channels; the live spin lock). |
| `sub_8245FDC8` | `XAudioMasteringVoice::Pass` | Overridden | `XAudioVoice::Pass` (`xaudio.md`), then on success its buffer cleared (`XAudioBuffer::ClearThunk`, flags 3) for the next mix. Slot +68 of `0x8204A8E8` (probably). From `XAudioEngine::Mix` every frame, on our output thread. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::MasteringPass`, `XAudioVoice::Pass`'s plain C++ in a frame below and the buffer's clear, `--native_xaudio_engine`. Seen in play every mix (from `XAudioEngine::Mix`, recompiled, on our output thread). Harness `xaudio_engine` (32 cases: a callback or none, stopping with 0 to 2 passes left). |
| `sub_8245FB80` | `XAudioMasteringVoice::Process` | Overridden | r3 = the voice, r4 = the buffer: `XAudioVoice::ProcessEffects` (`xaudio.md`), then its render driver's (+68) slot +24 with the buffer (probably `AudioRenderDriver::Submit`, `audio.md`: the frame to our audio output). Slot +84 (the type's pass, probably), from `XAudioVoice::Pass`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::MasteringProcess`, `XAudioVoice::ProcessEffects`' plain C++ in a frame below, the driver's slot +24 (`AudioRenderDriver::Submit`, `audio.md`) through the translated code, `--native_xaudio_engine`. Seen in play every mix. Harness `xaudio_engine` (32 cases: up to two effects with any flags, the live engine's buffers where both bits 1 and 2 ask; the driver a recorded leaf). |
| `sub_8245FB10` | `XAudioMasteringVoice::~XAudioMasteringVoice` | | Vtable `0x8204A8E8`; `XAudioVoice::StopOrTail` (r4 1); the driver (+68) deleted through its +4 interface (slot +0, r4 1), 0; `XAudioVoice::Destroy` (`xaudio.md`). From `sub_8245FC70`. Not seen in play. |
| `sub_8245FC70` | | | `XAudioMasteringVoice::~XAudioMasteringVoice`; returns the voice. Probably slot +0 of `0x8204A8E8`. Not seen in play. |
| `sub_8245FE88` | | | Under the spin lock, the voice's node at +24 out of its list (as `XAudioVoice::Deactivate` without the outputs); 0. Probably the mastering voice's slot +80. Not seen in play. |
| `sub_8245FBE0` | `XAudio::MixerCallback` | | The render driver's callback (registered by `XAudioEngine::Init`; argument `4006DF6C` in `../native-audio.md`): `XAudioEngine::Mix([0x82954F08])`, tail call. Called by our output thread (`src/audio/`) once a frame, about 187.5 times a second. |
| `sub_8245BF80` | `XAudioEngine::Mix` | | r3 = the engine. `sub_825C8388`; the mixer's lock (`RtlEnterCriticalSection(0x8264D4F8)`); +300 = the thread (`[r13 + 256]`). With processor threads (+304): `KeSetEvent(0x82954EF8)` (the master's "mix now"), then `KeWaitForMultipleObjects` on "mix done" `0x82954EE8` and "title terminating" `0x82954EC4` (wait any, user mode, no timeout, a 48-byte wait block on the stack cleared with `memset`); index 1 (terminating) skips the next step. Without threads: `XAudioEngine::SwapStageLists` and `sub_8245B808` (r4 1, `audio.md`) on this thread. Then the mastering voice's (+64) slot +68 (`XAudioMasteringVoice::Pass`), `XAudio::AddStat(0, 1)` on success; +300 = 0; `XAudioEngine::TakeStats`; the lock left. Returns 0. From `XAudio::MixerCallback`. |
| `sub_8245F678` | `XAudioEngine::TakeStats` | Overridden | The 7 statistics counters (`0x82954F24`, `XAudio::AddStat`) copied to `0x82954F40` and cleared. Leaf. From `XAudioEngine::Mix`, each frame. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::TakeStats` (each counter read, cleared and copied, one at a time, not atomically, as the original), `--native_xaudio_engine`. Harness `xaudio_engine` (16 cases on the live counters, the mixer's lock `0x8264D4F8` held around them so the mix can't add to them, and put back after). |
| `sub_8245BE08` | `XAudio::MasterThread` | | The master processor thread's entry (made by `XAudioEngine::Init`): stores its thread at the engine's +332 + 4 × its processor; loops: waits on "mix now" (`KeWaitForSingleObject(0x82954EF8)`, user mode, no timeout); when +300 is set (a mix), `XAudioEngine::SwapStageLists` and `sub_8245B808` (r4 1: wakes the workers through the semaphore `0x82954ED4` and runs the passes); else (the teardown) releases the semaphore +304 − 1 times (the workers end) and ends after; then `KeSetEvent(0x82954EE8)` ("mix done"). Seen in play, on hardware thread 4 (probably). |
| `sub_8245BED0` | `XAudio::WorkerThread` | | A worker processor thread's entry: stores its thread as the master does; waits on the semaphore `0x82954ED4` (`KeWaitForSingleObject`); while +300 is set, `sub_8245B808` (r4 0: the passes, without the volumes) and waits again; ends when woken with +300 clear. Seen in play (probably hardware thread 5). |
| `sub_8245B720` | `XAudioEngine::SwapStageLists` | | Under the spin lock: `XAudioStage::Swap` on the source voices' stage (+80) and each submix stage (+124, +128 of them, 44 bytes each). From `XAudio::MasterThread` and `XAudioEngine::Mix` (no threads), each frame. |
| `sub_8245B208` | `XAudioStage::Swap` | | r3 = a stage: under the spin lock, its +36 (the list to mix) and +40 (the list mixed) swapped. From `XAudioEngine::SwapStageLists`. |
| `sub_8245B480` | `XAudioEngine::RunStage` | | r3 = the engine, r4 = a stage. Loops: under the spin lock, takes the first voice of the stage's +36 list (by the list's node offset +8), moves it to the end of +40, and records it at the engine's +144 + 4 × the processor (`[r13 + 268]`); out of the lock, the voice's slot +68 (its pass), `XAudio::AddStat(0, 1)` on success; until +36 is empty, then the processor's entry cleared. Every processor runs it on the same stage at once, so each voice is mixed by one of them. From `sub_8245B808` (`audio.md`). |
| `sub_8245A940` | `XAudioEngine::Barrier` | Overridden | r3 = the engine, r4 = an 8-byte barrier (+356 or +364). Nothing without threads (+304 = 0); else this processor's byte (`[r13 + 268]`) set to 1, then spins (`db16cyc`) until the word holds a 1 for each processor that has a thread (+308 + 4i nonzero) and clears it, or until another processor has cleared it. Leaf. From `sub_8245B808` (`audio.md`), after each stage. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::Barrier` (`src/xaudio/engine.cpp`), the word polled with the translation's volatile 64-bit loads, as the recompiled processors read it (the original's `db16cyc` is nothing in the translation), `--native_xaudio_engine`. Seen in play on XAudio's processor threads. Harness `xaudio_engine` (48 cases the one harness thread ends: its byte completing the word, its byte past the word with the word clear, no threads). |
| `sub_8245ACE0` | `XAudioEngine::RunCallbacks` | | r3 = the engine, r4 = the stage (0 or 1, low byte). Under the spin lock, for each of that stage's 8 callback slots (+168 + 64 × r4 + 8i: the function, its context): +296 = the slot; when set, the lock left, the function called with its context, the lock taken again; +296 = 0 at the end. From `sub_8245B808` (`audio.md`: r4 0 before the passes, 1 after). The game's callback is XACT's (`XAudioRegisterProcessingCallback`, `xaudio.md`). |
| `sub_8245AB60` | `XAudioEngine::RegisterCallback` | | r3 = the engine, r4 = the stage (low byte), r5 = the function, r6 = its context, r7 = the slot out (or 0). Under the spin lock, the stage's first free slot of 8 (+168 + 64 × r4 + 8i) set; E_OUTOFMEMORY when full. From `XAudioRegisterProcessingCallback` (`sub_8245A5E8`, `xaudio.md`), XACT's, at boot. |
| `sub_8245B3A0` | `XAudioEngine::UnregisterCallback` | | r3 = the engine, r4 = a slot (0 unless within +168 to +296). Under the spin lock, `sub_8245AFA0` (`xaudio.md`: waits until the mixer isn't running it) and the slot's function cleared (stored at [r4] even when r4 was out of range, so at address 0). From `sub_8245A668` (`xaudio.md`). Not seen in play. |
| `sub_8245B638` | `XAudioEngine::Commit` | | r3 = the engine. Under the spin lock, while the start-pending list (+68, node offset at +76) isn't empty, `XAudioSourceVoice::Start` (`xaudio.md`, r4 0) on its first voice (which takes it off the list), stopping at the first failure; returns the last result (0 with none). From `XAudio::Commit` (`sub_8245A6E8`, `xaudio.md`). |
| `sub_8245FBF0` | `XAudioEngine::StageOf` | | r3 = the engine, r4 = a voice: by its type (slot +52): 0 the source voices' stage (+80), 1 its submix stage (+124 + 44 × the voice's +76), else 0. From `sub_82460000`. |
| `sub_82460000` | | | r3 = a voice: its stage (`XAudioEngine::StageOf`); when it has one, under the spin lock, the voice appended to the stage's +40 list (by the list's node offset); 0. Probably a vtable slot (+76, joining the mix, as `XAudioVoice::Activate` does), of a voice class not read. Seen in play; reached only indirectly. |
| `sub_8245B0D0` | | | The title terminate routine (`0x82954EB4`'s): when r3 is that registration, `sub_8245AA48` and `KeSetEvent(0x82954EC4)` (the mixer's callback stops waiting). Not seen in play (closing the window ends the title without it). |
| `sub_8245AA48` | | | Under the critical section `0x8264D4D8`: when the title terminate registration (`0x82954EB4`, its routine word) is set, unregistered (`ExRegisterTitleTerminateNotification`, r4 0) and its word cleared. From `sub_8245B0D0` and the teardown. Not seen in play. |
| `sub_8245B940` | `XAudioEngine::~XAudioEngine` | | The teardown (691 lines, in outline): `XAudio::UnregisterStats`; the render driver's callback unregistered; +300 cleared and "mix now" set so the threads end; each thread waited for (`WaitForSingleObject`, `video-output.md`) and closed (`CloseHandle`, `functions.md`); the terminate registration removed (as `sub_8245AA48`); under the spin lock the voice lists emptied (`sub_82466EA0`, codecs lead); the mastering voice, the factory and the buffers (`XAudioBuffer::ReleaseThunk`) released; `sub_8246E060` four times; the allocator released. From `sub_8245C168`. Not seen in play. Not read in detail. |
| `sub_8245C168` | | | `XAudioEngine::~XAudioEngine`; returns the engine. Probably slot +0 of `0x8204A6E0`. Not seen in play. |
| `sub_8245B170` | | | The engine's base destructor: vtable `0x8204A6CC`; the allocator (+8) released (slot +4), 0; vtable `0x8204A6B8`; with r4 bit 0, freed (`XAudio::Free`). Not seen in play. |
| `sub_8245B118` | | | The root destructor: vtable `0x8204A6B8`; with r4 bit 0, freed (`XAudio::Free`, attributes `0x61820000`). Not seen in play. |
| `sub_8245EF28` | | | As `sub_8245B118` for vtable `0x8204A868` (the API lock object's, `sub_825DB1E0` in `xaudio.md`). Not seen in play. |
| `sub_8245AAB8` |  | Overridden | r3 = an object: when not 0, its slot +0 with r4 1 (the deleting destructor), tail call. Probably slot +12 of the engine's or another XAudio class's vtable (as `sub_8245AAD8`, `xaudio.md`). Seen in play; reached only indirectly. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::DeleteObject` picks the method, the adapter tail-calls it with r4 1 (at the caller's frame and lr, as the original); `--native_xaudio_engine`. Harness `xaudio_engine` (24 cases: no object, and a recompiled leaf that stores r4 and r5). |
| `sub_8245EF90` | `XAudio::Free` | Overridden | r3 unused (`0x82954F20` at every call), r4 = the block, r5 = the attributes: `XMemFree` (`heap.md`), tail call. Leaf. The partner of `XAudio::Alloc` (`sub_8245EF80`, `xaudio.md`). From the destructors here and in `xaudio.md` (`sub_82460E90`, `sub_824615A0`), `XmaObject::Destroy` (`audio.md`) and the codecs (`sub_82465398`, `sub_82465590`, `sub_82467450`, `sub_82469A88`). Seen in play. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::Free`, XMemFree's plain C++ (`memory::crt::XMemFree`, `heap.md`) while `--native_heap` routes the heap (as `XAudio::Alloc`), else the original; `--native_xaudio_engine`. Harness `xaudio_engine` (8 cases: a block of each version's own freed, r3 compared as whether it's the block). |
| `sub_8245F6F0` | `XAudioBuffer::GetFormat` | Overridden | r3 = a buffer, r4 = 12 bytes out: +20 (the 2 bytes there and 2 more), +24 (the rate) and +28 (the samples); 0. Leaf. From the codecs `sub_82466440` and `sub_8246D860`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::BufferGetFormat`, `--native_xaudio_engine`. Harness `xaudio_engine` (32 cases). |
| `sub_8245F718` | `XAudioBuffer::GetFormatThunk` | Overridden | `XAudioBuffer::GetFormat` through its interface (8 bytes in; 0 stays 0), inline. Leaf. From `AudioRenderDriver::Submit` and `XmaVoice::Update` (`audio.md`), `sub_82465428`, `sub_825A6210` (seen), and the effects `sub_82579E18`, `sub_825A5610`, `sub_825A5838`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::BufferGetFormatThunk`, `--native_xaudio_engine`. Harness `xaudio_engine` (32 cases). |
| `sub_8245F8D0` |  | Overridden | r3 = a buffer: its interface (r3 + 8, or 0) to [r4]. Leaf. Probably the buffer's slot +20, which `XAudioBuffer::Create` (`xaudio.md`) calls. Seen in play. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::BufferInterface` (named `XAudioBuffer::GetInterface` in its print; r3 left as it was, as the original), `--native_xaudio_engine`. Harness `xaudio_engine` (32 cases, one with no buffer). |
| `sub_82460288` | `XAudioAllocator::Alloc` | Overridden | r3 = the allocator, r4 = a size: rounded up to 4; 0 when more than what's left (its slot +16, `sub_82460388`), else the block (+24) + used (+20), used raised. Probably slot +20 of `0x8204A954` (the default allocator's, `XAudio::DefaultAllocator`, `xaudio.md`), which the engine, voices and factory allocate with. Seen in play. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::AllocatorAlloc`, its slot +16 through the translated code (r4 the caller's size, as the original's call leaves it), in a frame of the original's size (112 bytes); r3 the original's 64-bit sum; `--native_xaudio_engine`. Harness `xaudio_engine` (48 cases: slot +16 the real `Remaining` or a recompiled AddRef, sizes that fit and don't). |
| `sub_82460388` | `XAudioAllocator::Remaining` | Overridden | +16 − +20 (the size less the used). Leaf. Slot +16, from `XAudioAllocator::Alloc`. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::AllocatorRemaining` (r3 the original's 64-bit difference), `--native_xaudio_engine`. Harness `xaudio_engine` (32 cases). |
| `sub_824601F8` | `XAudioAllocator::Reserve` | | +16 += r4 rounded up to 4. Leaf. Probably slot +8 (the sizing allocator `sub_82460538` counts sizes before `XAudioAllocator::Commit`). Not seen in play. |
| `sub_82460210` | `XAudioAllocator::Commit` | | r4 = the attributes: when +16 isn't 0, +12 = the attributes and +24 = one block of +16 bytes (`XAudio::Alloc`); E_OUTOFMEMORY when that fails. Probably slot +12. Not seen in play. |
| `sub_82460538` | | | r3 = out: a sizing allocator (28 bytes, `XAudio::Alloc`, attributes 1): vtables `0x8204A954` and `0x8204A96C`, +8 = 1, +12 to +24 cleared. E_OUTOFMEMORY. From `XAudio::GetAllocator` when the size is 0 (`xaudio.md`). Not seen in play. |
| `sub_824602F8` | `XAudioAllocator::~XAudioAllocator` | | Vtables `0x8204A954` and `0x8204A940`; the block (+24) freed (`XAudio::Free`, attributes +12) and cleared; +4 = `0x8204A6B8`. From `sub_824604E8`, `sub_82460410`, `sub_82460470`. Seen in play (the voices' allocators freed). |
| `sub_824604E8` | | | The default allocator's destructor (vtables `0x8204A954`, `0x8204A980`; +24 = 0, since its block is itself), then `XAudioAllocator::~XAudioAllocator`. From `sub_824603A0`. Seen in play. |
| `sub_824603A0` | | | `this − 4`, then `sub_824604E8`. A thunk through the +4 interface (`0x8204A980`'s slot +0). Seen in play. |
| `sub_824603A8` | | | r3 = the +4 interface: its slot +0 (the destructor, r4 1), then the object freed (`XAudio::Free`, its attributes +12). Probably the free slot (+12) a release calls at 0. Seen in play. |
| `sub_82460370` |  | Overridden | The interface's AddRef: the count (+8 of the object, +4 from r3) raised and returned. Leaf. Called by `sub_8246D330` (codecs lead, not seen) and through a vtable. Seen in play. The word it raises is the one at r3 + 8. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::InterfaceAddRef` (r3 the original's 64-bit sum), `--native_xaudio_engine`. Harness `xaudio_engine` (32 cases, one at the wrap). |
| `sub_82460410` | | | `XAudioAllocator::~XAudioAllocator`; with r4 bit 0 freed (`XAudio::Free`, `0x61820000`). From `sub_82460368`. Not seen in play. |
| `sub_82460368` | | | `this − 4`, then `sub_82460410` (`0x8204A940`'s thunk). Not seen in play. |
| `sub_82460470` | | | The sizing allocator's destructor (vtables `0x8204A954`, `0x8204A96C`), `XAudioAllocator::~XAudioAllocator`, with r4 bit 0 freed (attributes 1). From `sub_82460398`. Not seen in play. |
| `sub_82460398` | | | `this − 4`, then `sub_82460470` (`0x8204A96C`'s thunk). Not seen in play. |
| `sub_82461EF8` |  | Overridden | r4 = 4 bytes out: byte +0 = 6, half-word +2 = 3750; 0. Leaf. Probably a factory object's info slot (+8: the byte and the latency `XAudioVoice::CreateObject` reads, `xaudio.md`), of a class not read. Seen in play; reached only indirectly. Ours (2026-10-29, the B7 loop, migration 1): `xaudio::ObjectInfo`, `--native_xaudio_engine`. Harness `xaudio_engine` (32 cases). |
