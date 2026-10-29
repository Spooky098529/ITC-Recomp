# Known functions: XACT's tracks and events

**XACT's tracks and events**: the layer of XACT, the sound library
linked into the game (`xact.md`, translated as the game's code: the
user's call, 2026-10-23), between a sound (`XACTSound`, `xact.md`) and
the waves that play its wave bank entries (`XACTWave`, `xact-waves.md`).
How the game's audio works is in `../native-audio.md`; the objects'
fields are in `../known-data.md`. Four kinds of object:

- **The track** (`XACTTrack`, 64 bytes, vtable `0x820765F8` over the
  base `0x82075344`, `825D0000`-`825D0E78`): one of a sound's tracks,
  made by the sound's prepare slot (`XACTComplexSound::Prepare`
  `sub_825CCA88`, `XACTSimpleSound::Prepare` `sub_825CCDB0`, `xact.md`)
  and linked into the sound's track list (+36). It holds the track's
  volume and pitch (the voice's volume is the track's × the sound's ×
  an RPC factor; the pitch the sum), the track's RPC entries (44 bytes
  each, `825C91E0`-`825C9788`, never made in play: the game's sounds
  have no RPCs) and its **instance**. The sound's methods call one
  track function per track: Play, Stop, Pause, the volume, the
  outputs, the game thread's update and the audio callback's.
- **The track instance** (`XACTTrackInstance`, probably; 64 bytes,
  vtable `0x82076AD8`, one slot, `825D53D8`-`825D5F00`): one play of a
  track's events. It holds the engine, its track, the events (an array
  of pointers and a count), a state (1 created, 2 preparing, 3
  prepared, 4 playing, 5 stopping, 6 done), its play-wave event (+36,
  the event every volume, pitch, output and pause call is forwarded to)
  and counts of events not yet fired (+44) and fired but not done
  (+45), with a list (+48) of uncounted helper events. Most of its
  methods are one-line forwarders to the play-wave event's vtable.
- **The events** (`XACTEvent`, the base 48 bytes, vtable
  `0x82076A10`, `825D0EF0`-`825D1B88`): a track's clip events, made by
  `XACTEvent::Create` (`sub_825D0F00`) from a sound bank record by its
  type (the record's word +0 low 5 bits, a 21-way jump table at
  `0x825D0F4C`). Each is scheduled on the sound's event queue
  (`XACTSound::ScheduleEvent` `sub_825CB0C8`, `xact.md`), fired by the
  sound's audio update (slot +4), and tells its instance when it's done.
  In play only type 1 ran (the play-wave event, from the simple sounds'
  one-track record and the complex sounds' track records: a counter in
  an attract race, 2026-10-29, saw `XACTTrack::Prepare` called from
  `XACTComplexSound::Prepare`, and the simple sounds' prepare not at all).
  The types, by what each makes: 0 stop (`sub_825D8950`); 1, 2, 4, 5
  play wave (`sub_825D82E8`, 72 bytes; 2 and 5 with the record + 12 at
  +56, 4 and 5 with a volume and pitch variation record); 3, 6 play
  wave with a wave variation (`sub_825D7AA0`, 188 bytes); 7 and 16
  pitch, 8 and 17 volume, 9 and 18 marker, 13 and 15 a variable set
  (`sub_825D67E8`, `sub_825D6308`, `sub_825D5FE8`, `sub_825D6CF0`;
  16, 17, 18 and 15 with a repeat record; never ran, in
  `../loops/leads.md`); 10 and 11 (`sub_825D1A80`, probably a
  conditional transition to a new instance of the track); 12, 14 and
  19 (`sub_825D70C8`, `sub_825D5F40`, `sub_825D71C0`, little or nothing
  done when fired); 20 none; above 20 the error 0x8AC70011.
- **The play-wave event** (`XACTPlayWaveEvent`, 72 bytes, vtable
  `0x82076CE8` over the wave event base `0x82076B58`, 32 slots): the
  wave's owner (`XACTWave` +48, `xact-waves.md`). Its Prepare
  (`sub_825D86F8`) makes the wave through the wave bank's `CreateWave`
  and prepares it; its Fire (`sub_825D8378`) plays it, deferred; its
  Stop stops it; its volume, pitch and output slots go to the wave's
  small forwarders (`825D1CC8`-`825D1DD0`, here: each calls one slot of
  the wave's voice, +44), and the wave calls back its slots +28, +32,
  +96, +100 and +108. The wave-variation event (`sub_825D7AA0`) has
  the same slots with a variation table (`XACTVariationTable`,
  `xact-waves.md`) and an inner marker event that moves it to the next
  wave; it never ran.

The vtables, read from the loaded image (`itc/xex_image/default.bin`,
2026-10-29): the track's `0x820765F8` +0 `825D0978` (the deleting
destructor; one slot); the base `0x82075344` +0 `825C0958`, +4
`825C0B48`, +8 `825C0B58`, +12 `825C0B50` (`xact.md`'s range, never
reached here); the track instance's `0x82076AD8` +0 `825D5E58`; the
event base's `0x82076A10` +0 `825D17E0`, +4 `825D1358` (Fire), +8
`825D17D0` (Prepare), +12 `825C8388` (the duration: returns 0,
`vehicle-accessors.md`), +16 `825D17B0` (SetQueued); type 10 and 11's
`0x82076A24` +0 `825D1AD8`, +4 `825D1B88`, then as the base; type
14's `0x82076ADC` +0 `825D5F80`, +4 `825D1358`; type 12's
`0x82076B44` +0 `825D7148`, +4 `825D7110`; type 19's `0x82076BD8` +0
`825D7328`, +4 `825D7210`, +8 `825D73B8`, +20 `825D7270`, the rest as
the wave event base's; the stop event's `0x82076D68` +0 `825D8990`, +4
`825D89F8`; the wave event base `0x82076B58` +0 `825D72C0`, +4
`825D1358`, +8 `825D17D0`, +12 `825C8388`, +16 `825D17B0`, +20 and +24
`825A7098` (probably `_purecall`), +28 and +32 `82106540` (empty,
`audio.md`), +36 `824892F0` (stores +48: a folded setter in the movie
library's range), +40 to +56 `825C8388`, +60 `825D71B0`, then empty
functions, `825C8388` and `825D71B0`. The play-wave event's
`0x82076CE8`: +0 `825D8660` (the deleting destructor), +4 `825D8378`
(Fire), +8 `825D86F8` (Prepare), +12 `825D8870` (GetDuration), +16
`825D8530` (SetQueued), +20 `825D76A0` (IsPrepared), +24 `825D7708`
(IsPlaying), +28 `825D8360` (ForgetWave), +32 `825D8570`, +36
`824892F0`, +40 `825D85C8`, +44 `825D81E8` (IsBusy), +48 `825D8358`
(the loop count), +52 `825D84C0` (the wave bank), +56 `82090770` (the
wave: a getter of +60 folded with the framework's, `framework.md`),
+60 `825D83F8` (Stop), +64 `825D7FE0` (SetVolume), +68 `825D8048`,
+72 `825D74F0` (SetPitch), +76 `825D8180`, +80 `825D7418`, +84
`825D7480`, +88 `825D8268` (Pause), +92 `825D8920` (the variation
record), +96 `825D7558` (ApplyVariation), +100 `82106540` (empty),
+104 `82483208` (SetWave), +108 `825D71B0` (no next wave), +112
`82090778` (+64), +116 `8248F030` (+68, `framework.md`), +120
`825D80B0`, +124 `825D8118`. The wave-variation event's `0x82076C60`:
+0 `825D7F88`, +4 `825D7618`, +8 `825D7C48`, +12 `825D7790`, +16
`825D17B0`, +20 `825D76A0`, +24 `825D7708`, +28 `825D8360`, +32
`82106540`, +36 `824892F0`, +40 `825D79E8`, +44 `825D81E8`, +48
`825D7B70`, +52 `825D84C0`, +56 `82483380` (+72, folded, the movie
library's range), +60 `825D7848`, +64 `825D7FE0`, +68 `825D8048`, +72
`825D74F0`, +76 `825D8180`, +80 `825D7418`, +84 `825D7480`, +88
`825D75A8`, +92 `825D7B80`, +96 `825D7558`, +100 `825D7908`, +104
`825D7B78`, +108 `825D7698`, +112 `82090788` (+88, `framework.md`),
+116 `82493700` (+92, `system-services.md`), +120 `825D80B0`, +124
`825D8118`, +128 `825D7A48`; its node's `0x82076C58` +0 `825D7F18`.
The parameter events' vtables (`0x82076AF4`, `0x82076B08`,
`0x82076B1C`, `0x82076B30`, and their repeat base `0x82076D7C`) are in
the lead for them.

Shared with other files, which keep their rows: the sound's methods that
call the tracks (`XACTSound::Play`, `Stop`, `Pause`, `UpdateVolume`,
`SetTrackParameter`, `AnyTrackBusy`, `Update`, `AudioUpdate`,
`ScheduleEvent`, `GetVolume`, `GetPitch`, `XACTComplexSound::Prepare`,
`XACTSimpleSound::Prepare`), `XACTSoundBank::GetWaveBank`
(`sub_825C7218`), `XACTWaveBank::GetWaveDurations` (`sub_825CC380`),
`XACTWaveBank::DoWork` (`sub_825CC560`), `XACTGetTimeMs`
(`sub_825BFE28`), `XACTEngine::SendNotification` (`sub_825C1388`),
`XACTCue::GetVariable` (`sub_825C5960`) and `sub_825C0748` in
`xact.md`; the waves' and the voice's slots, `XACTWave::Pause`,
`XACTVariationTable::XACTVariationTable` and its destructor, and the
tracks' volume and pitch getters only the waves call
(`sub_825D0268`, `sub_825D02B0`) in `xact-waves.md`; `sub_825C8388`
(returns 0) in `vehicle-accessors.md`; `sub_82106540` (empty) in
`audio.md`; the folded getters `sub_82090770`, `sub_82090778`,
`sub_82090788`, `sub_8248F030` in `framework.md` and `sub_82493700` in
`system-services.md`; `XMemAlloc`, `XMemFree`, `operator new`,
`operator delete` in `heap.md`; `log10`, `pow`, `memset` in
`c-runtime.md`. The folded one-instruction methods `sub_82483380` (+72)
and `sub_824892F0` (stores +48), in the movie library's range and never
run here, are named in the notes without rows (the movie library is
replaced and not mapped: "Rules that still hold" in
`../loops/b7-loop.md`); `sub_82483208` (stores +60), also in that range,
has a row here, as only the play-wave event reaches it in play. The
rows were drafted by the B7 loop's mapping agent (2026-10-29) from the
generated code and the loaded image; nothing here ran differently.

**What's left out.** The parameter events (pitch, volume, marker and
variable set, with their repeat base and ramp helpers, 28 functions at
`825D5FE8`-`825D6EB8` and `825D8AA8`-`825D8E98`, none seen in play)
and XACT's random generator `sub_825D8F18` are in a lead in
`../loops/leads.md` (this map's limit of about 150 functions); the
static set-up `sub_825D8F88` at the end of the range (a zero 4×4
matrix at `0x8266D000`, not XACT's) too.

**The lock.** One function here enters XACT's lock: the track
instance's `CancelEvents` (`sub_825D5520`), which takes the engine's
critical section (`XACTEngine` +144) inline (`RtlEnterCriticalSection`
and `RtlLeaveCriticalSection` called directly) around the sound's event
queue. The audio callback's functions here (`XACTTrack::AudioUpdate`
and the instance's, the events' Fire, Schedule and Done) run with that
lock held by `XACTEngine::AudioCallback` and take none themselves. By
the user's call for XACT's lock (2026-10-29, "Precedent first" in
`../loops/b7-loop.md`; `xact-waves.md`'s locks paragraph), native code
takes the game's own critical section exactly as the original does,
until every user is native.

**Native** (B7's loop, 2026-10-29, `--native_xact_tracks`): plain C++ in
`itc_recomp/src/xact/tracks.cpp` behind the adapters in
`src/hooks/xact_tracks.cpp`, checked by the harness `xact_tracks`
(`src/harnesses/xact_tracks.cpp`: each case's objects in a block of
random words, with fake vtables whose slots hold the real functions or
leaf recorders). A call through an object's vtable runs the plain C++
here while the slot holds the native function, and goes through the
guest call otherwise; the waves' and voices' slots always go through the
guest call (their adapters run their own plain C++). The play-wave
event's slots that ask for the wave call its slot +56 twice or three
times, as the original does (the getter of +60, read directly while the
slot holds it). The second migration (2026-10-29) made the complex
sounds' track prepare native (`XACTTrack::Prepare`, `CreateInstance`,
`XACTTrackInstance::Prepare`), which walks the track's event records
through the sound bank's FirstEvent and NextEvent (`xact.md`, their
plain C++ while the bank's slots hold them). What's left in play waits
on `XACTGetTimeMs` (`sub_825BFE28`) over `QueryPerformanceCounter`
(`sub_825BA0C8`), a kernel import, not taken: the instances' and
tracks' Play and audio updates, the events' Schedule and Done, and the
play-wave event's Fire and Stop.

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
| `sub_825D0CE0` | `XACTTrack::XACTTrack` | Overridden | r3 = the track (64 bytes, XMemAlloc 0x20848004 by the sound's prepare), r4 = the engine (+12), r5 = the sound (+20), r6 = the track's record in the sound bank (+16; 0 for a simple sound), r7 = the track index (byte +48). +4 and +8 linked to itself (the node in the sound's track list), vtable `0x820765F8`, +44 and +60 = 0.0 (`0x82000A34`), +56 = 1.0 (`0x82000A30`), +24, +28, +36, +52 and byte +32 = 0. Then `XACTTrack::SetVolume` with 1.0, or with a record the gain of its volume byte (+0) from the table at `0x820761F8` (256 floats, 0 to 0.0, 255 to 1.995). From `XACTComplexSound::Prepare` (`sub_825CCA88`) and `XACTSimpleSound::Prepare` (`sub_825CCDB0`, `xact.md`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); SetVolume through its plain C++ (the original through the guest call where it declines); harness `xact_tracks`. |
| `sub_825D0110` | `XACTTrack::~XACTTrack` | Native | Vtable `0x820765F8`; deletes the next instance (+28) and the instance (+24) through their slot +0 with 1; frees the RPC entries (+52: with a count at -4, the vector deleting destructor through the first entry's +4 vtable with 3; else `operator delete` on the block); then the base vtable `0x82075344`. Only caller `sub_825D0978`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by `XACTTrack`'s deleting destructor's plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the instances' and the RPC entries' deletes (never in play for the RPCs) through the guest call; harness `xact_tracks`. |
| `sub_825D0978` | `XACTTrack::~XACTTrack` (deleting) | Overridden | Slot +0 of `0x820765F8`: `~XACTTrack` (`sub_825D0110`), then XMemFree(0x20848004) when r4 bit 0. No direct callers (the sound's destructor and its prepare's failure path call slot +0). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); `~XACTTrack` through its plain C++, XMemFree through the allocation layer (the original unless `--native_heap`); harness `xact_tracks`. |
| `sub_825D09D0` | `XACTTrack::PrepareSimple` (probably) | Overridden | r4 = the one-track record (the simple sound's 12 bytes at sound + 128), r5 = the time offset. The RPC set-up (`sub_825D06B8`; its result kept), a 64-byte instance (XMemAlloc 0x20848008, `XACTTrackInstance::XACTTrackInstance` with the engine, the track and -1) into +24 (E_OUTOFMEMORY, 0x8007000E, when it fails); then, unless the RPC set-up failed, `XACTTrackInstance::PrepareSimple` (`sub_825D53D8`) with r4 and r5, and on success `sub_825D0800` (the RPCs' start). On failure the instance is deleted (slot +0 with 1) and +24 = 0. Returns the result. Only caller `XACTSimpleSound::Prepare` (`sub_825CCDB0`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); allocates and frees through the allocation layer (the original unless `--native_heap`); the event's constructor, Prepare and IsPrepared through their plain C++; harness `xact_tracks` (the failure paths only: a new event's Prepare succeeds only with a real wave, which the harness doesn't make). Not called in the attract races of 2026-10-29 (a counter saw `XACTTrack::Prepare` called twice in one, from `XACTComplexSound::Prepare`, and this path not at all). |
| `sub_825D0AA0` | `XACTTrack::Prepare` | Overridden | r4 = the track's events key (a word read by the sound's prepare from its record's +1), r5 = the time offset. The RPC set-up (`sub_825D06B8`); when it succeeds, `XACTTrack::CreateInstance` (`sub_825D02F8`) with r4, a stack word and r5; on success +24 = the instance and `sub_825D0800`. Returns the result. Only caller `XACTComplexSound::Prepare` (`sub_825CCA88`); called twice in an attract race (2026-10-29, a counter: from `XACTComplexSound::Prepare`, return address `825CCB98`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`, the original unless `--native_heap`; the second migration, 2026-10-29); CreateRpcs, CreateInstance and StartRpcs through their plain C++ (the originals through the guest call where they decline); harness `xact_tracks` (the RPC-less track; its instance's events fail or are kept as described under `XACTTrackInstance::Prepare`). A counter in an attract race (2026-10-29) saw it called twice from `825CCB98`, both returning 0 with a play-wave event kept. |
| `sub_825D02F8` | `XACTTrack::CreateInstance` | Native | r4 = the events key, r5 = where the instance goes, r6 = the time offset. A 64-byte instance (XMemAlloc 0x20848008, `XACTTrackInstance::XACTTrackInstance` `sub_825D57D0` with the engine +12, the track and -1), then `XACTTrackInstance::Prepare` (`sub_825D5A48`) with r4 and r6; on success the instance to [r5]; on failure (or E_OUTOFMEMORY) it's deleted (slot +0 with 1). Returns the result. From `XACTTrack::Prepare` (`sub_825D0AA0`) and `sub_825D0B08`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` (the second migration, 2026-10-29) called directly by `XACTTrack::Prepare`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play, `sub_825D0B08` never ran; `--native_xact_tracks` now switches only the callers); allocates through the allocation layer; the instance's constructor and Prepare through their plain C++, the delete on a failure through the guest call (its slot +0); harness `xact_tracks` (success and failure). `sub_825D0B08` never ran in play. |
| `sub_825D06B8` | `XACTTrack::CreateRpcs` (probably) | Overridden | Returns 0 unless the sound's record (sound +28) byte +0 has 0x4 (RPCs): then the track's RPC count (`sub_825CB348` with byte +48; 0 gives a 4-byte `operator new`), a counted array of 44-byte entries (XMemAlloc 0x20840053 with the count at +0, the entries from +4, each `sub_825C95F8`) into +52 (E_OUTOFMEMORY when it fails), each set up by `sub_825C9788`(entry, engine, track, the RPC `sub_825C07C8`(engine, `sub_825CB3D8`(sound, index, i))) until one fails. None of those ran: no sound in play has RPCs. From `sub_825D09D0` and `sub_825D0AA0`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); with no RPCs (as in play) nothing to do; with RPCs the original runs (the adapter declines before writing); harness `xact_tracks`. |
| `sub_825D0800` | `XACTTrack::StartRpcs` (probably) | Overridden | With RPCs (the sound record's 0x4 and a nonzero `sub_825CB348`): `sub_825C93A8` on each 44-byte entry (+52). Else nothing. From `sub_825D09D0` and `sub_825D0AA0`. Runs in attract races (never with RPCs). Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); with no RPCs (as in play) nothing to do; with RPCs the original runs (the adapter declines before writing); harness `xact_tracks`. |
| `sub_825D01D0` | `XACTTrack::Play` | | Tail call of `XACTTrackInstance::Play` (`sub_825D5980`) on +24 (no null check). From `XACTSound::Play` (`sub_825CBB28`, `xact.md`). Runs in attract races. |
| `sub_825D01D8` | `XACTTrack::Stop` | Overridden | r4 = immediate (bool). With an instance (+24): when not immediate and its play-wave event (+36) is missing or not playing (slot +24, IsPlaying), immediate = 1; `XACTTrackInstance::Stop` (`sub_825D5D18`) with it. Then a next instance (+28) is handed to `sub_825C1548`(engine, it) (probably a deferred delete; not read) and +28 = 0. Returns Stop's result (0 without an instance). From `XACTSound::Stop` (`sub_825CBC58`) and `sub_825C91E0` (an RPC's, never ran). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the event's IsPlaying and the instance's Stop through their plain C++, `sub_825C1548` (never seen in play: it links the next instance at the tail of the engine's +76 list) through the guest call; harness `xact_tracks`. |
| `sub_825D05A8` | `XACTTrack::Pause` | Overridden | r4 = pause (bool): `XACTTrackInstance::Pause` (`sub_825D57A8`) on +24, or 0 without one. From `XACTSound::Pause` (`sub_825CA6F8`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D0590` | `XACTTrack::IsBusy` (probably) | Overridden | `sub_825D5780` on +24 (the play-wave event's slot +44, down to the wave's slot +20: 0 for the in-memory waves of play), or 0 without one. From `XACTSound::AnyTrackBusy` (`sub_825CAA58`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D0BF8` | `XACTTrack::SetVolume` | Native | f1 = a gain: +40 = f1, or 0.0 when below it; with an instance, its play-wave event's volume (`sub_825D5678`) = `XACTSound::GetVolume` (`sub_825CB5E8`) × +40 × +56. From the constructor (`sub_825D0CE0`), `sub_825D1478` and the volume events (`sub_825D63D0`, `sub_825D6580`, in the parameter events' lead). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by `XACTTrack::XACTTrack`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the instance's SetVolume through its plain C++; the original where `XACTSound::GetVolume`'s plain C++ declines (+40 already written, as the original writes it); harness `xact_tracks`. |
| `sub_825D0428` | `XACTTrack::UpdateVolume` | Overridden | With an instance (+24): its volume (`sub_825D5678`) = +40 × `XACTSound::GetVolume` (`sub_825CB5E8`) × +56. From `XACTSound::UpdateVolume` (`sub_825CA860`), `sub_825CB818` and `sub_825D0670`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the instance's SetVolume through its plain C++; the original where `XACTSound::GetVolume`'s plain C++ declines (+40 already written, as the original writes it); harness `xact_tracks`. |
| `sub_825D0C78` | `XACTTrack::SetPitch` | | f1 = a pitch offset (octaves): +44 = f1; with an instance, its pitch (`sub_825D56B8`) = f1 + `XACTSound::GetPitch` (`sub_825CAC48`) + +60. From `sub_825D1478` and the pitch events (`sub_825D68B0`, `sub_825D6AD8`, in the parameter events' lead). Not seen in play. |
| `sub_825D04A8` | `XACTTrack::UpdatePitch` | | With an instance: its pitch (`sub_825D56B8`) = +44 + `XACTSound::GetPitch` + +60. From `sub_825CA8B0` (probably the sound's pitch update) and `sub_825D06A0`. Not seen in play. |
| `sub_825D0670` | `XACTTrack::SetRpcVolume` (probably) | | f1 = a factor: +56 = f1 (0.0 when below it); with r5 set, `XACTTrack::UpdateVolume`. From `sub_825C91E0` (an RPC entry's update, never ran). Not seen in play. |
| `sub_825D06A0` | `XACTTrack::SetRpcPitch` (probably) | | f1: +60 = f1; with r5 set, `XACTTrack::UpdatePitch`. From `sub_825C91E0`. Not seen in play. |
| `sub_825D0868` | `XACTTrack::ResetRpc` (probably) | | r4 = 0 (volume) or 1 (pitch): resets +56 to 1.0 or +60 to 0.0, applies again (`sub_825C91E0` with 0) every RPC entry whose record's byte +3 (from its +20) is r4, then refreshes the instance's volume (`sub_825D5678` with +40 × +56 × the sound's volume) or pitch (`sub_825D56B8`). From `sub_825C96F0`. Not seen in play. |
| `sub_825D05C0` | `XACTTrack::HasReleaseRpc` (probably) | | 1 when the sound's record has RPCs (byte +0 0x4) and one of the track's RPC entries' records (entry +20) has 16-bit +0 = 2 and 16-bit +3 = 0; else 0. From the play-wave events' Stop (`sub_825D83F8`, `sub_825D7848`) and the stop event (`sub_825D89F8`), when the stop isn't immediate and the record's release flag is clear. Not seen in play (it's called from seen Stops only on a path that didn't run). |
| `sub_825D0490` | `XACTTrack::SetReverbSend` (probably) | | f1: `sub_825D5698` on +24 (down to the voice's slot +44, `XACTVoice::SetReverbSend`), when there is one. From `sub_825CAB40` and `sub_825CB818`. Not seen in play. |
| `sub_825D0510` | `XACTTrack::SetOutputVoices` (probably) | | r4: `sub_825D56D8` on +24 (down to the voice's slot +28), when there is one. From `sub_825CA900`. Not seen in play. |
| `sub_825D0528` | `XACTTrack::SetOutputMatrices` (probably) | | r4: `sub_825D56F8` on +24 (the voice's slot +36). From `sub_825CA940`. Not seen in play. |
| `sub_825D0540` | `XACTTrack::SetOutputs` (probably) | | r4: `sub_825D5718` on +24 (the voice's slot +32, `XACTVoice::SetOutputs`). From `XACTSound::SetTrackParameter` (`sub_825CA980`, `xact.md`, with the cue + 308, which the waves' `SetVoice` reads as the cue's outputs: so that sound method is probably SetOutputs). Not seen in play (the sound has no tracks yet when it runs). |
| `sub_825D0558` | `XACTTrack::SetMainMatrix` (probably) | | r4: `sub_825D5738` on +24 (the voice's slot +40). From `sub_825CA9C0`. Not seen in play. |
| `sub_825D0570` | `XACTTrack::SetChannelMatrix` (probably) | | r4 to r6: `sub_825D5758` on +24 (the voice's slot +48); 0 without an instance. From `sub_825CAA00`. Not seen in play. |
| `sub_825D03B0` | `XACTTrack::UsesWaveBank` (probably) | | r4 = a wave bank: 1 when the instance (+24) or the next one (+28) answers `sub_825D5620` (its play-wave event's wave bank is r4), else 0. From `sub_825CA808` (probably the sound's, when a wave bank goes). Not seen in play. |
| `sub_825D0E30` | `XACTTrack::Update` | Overridden | The game thread's per-track step (from `XACTSound::Update` `sub_825CB040`, under DoWork): a pending transition event (+36) makes the next instance (`sub_825D0B08`) and +36 = 0; then the empty `sub_82106540` on the instance. Runs in attract races (never with a transition). Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the transition (`sub_825D0B08`, never seen in play) through the guest call, the empty `sub_82106540` left out; harness `xact_tracks`. |
| `sub_825D0B08` | `XACTTrack::CreateNextInstance` (probably) | | r4 = the transition event (types 10 and 11): `XACTTrack::CreateInstance` with the event's record's word +41 when the event's +52 is set, else -1, and time 0; on success +28 = the new instance. From `XACTTrack::Update`. Not seen in play. |
| `sub_825D0B70` | `XACTTrack::SwitchInstance` (probably) | | When the next instance (+28) is prepared (+40 = 3): `sub_825C1548`(engine, the instance +24), +24 = +28, +28 = 0, and `XACTTrackInstance::Play` on it. Otherwise byte +32 \|= 0x80 (switch when it's ready). From `XACTTrack::AudioUpdate` and the transition event's Fire (`sub_825D1B88`). Not seen in play. |
| `sub_825D0D88` | `XACTTrack::AudioUpdate` | | The audio callback's per-track step (XAudio's thread, the engine's lock held; from `XACTSound::AudioUpdate` `sub_825CBD10`): with RPCs (the sound record's 0x4), `sub_825C9558` on each entry; then `XACTTrackInstance::AudioUpdate` (`sub_825D5EB0`) on +24; then, when byte +32 has 0x80 and the next instance (+28) is prepared, the bit cleared and `XACTTrack::SwitchInstance`. Runs in attract races. |
| `sub_825D0698` | | | Empty (`blr`). No direct callers; probably an unused vtable slot. Not seen in play. |
| `sub_825D06B0` | | | Empty (`blr`). No direct callers; probably an unused vtable slot. Not seen in play. |
| `sub_825D57D0` | `XACTTrackInstance::XACTTrackInstance` (probably) | Overridden | r3 = 64 bytes, r4 = the engine (+12), r5 = the track (+16), r6 = -1 from both callers (unused). +4 and +8 linked to itself, vtable `0x82076AD8`, +20, +28, +36, +60 and bytes +24, +32, +44, +45 = 0, +40 = 1 (created); a list head at +48 (vtable `0x82075344`, +52 and +56 linked to +48). Leaf. From `XACTTrack::PrepareSimple` and `CreateInstance`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D5838` | `XACTTrackInstance::~XACTTrackInstance` (probably) | Native | Vtable `0x82076AD8`; unlinks its node (+4/+8) when linked; with events (+20): `CancelEvents` (`sub_825D5520`), deletes each (slot +0 with 1) and frees the array (XMemFree 0x20840001); then deletes the list +48's entries (their +52's slot +0 with 1) until it's empty; state 4 or 5 becomes 6; the base vtable at +48 and +0. Only caller `sub_825D5E58`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by its deleting destructor's plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); CancelEvents through its plain C++, the events' and helpers' deletes through the guest call, XMemFree through the allocation layer (the original unless `--native_heap`); harness `xact_tracks` (the helper list empty, as in play). |
| `sub_825D5E58` | `XACTTrackInstance::~XACTTrackInstance` (deleting) | Overridden | Slot +0 of `0x82076AD8`: `sub_825D5838`, then XMemFree(0x20848008) when r4 bit 0. No direct callers. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the destructor's plain C++, XMemFree through the allocation layer (the original unless `--native_heap`); harness `xact_tracks`. |
| `sub_825D53D8` | `XACTTrackInstance::PrepareSimple` (probably) | Native | r4 = the one-track record, r5 = the time offset. State 2, byte +24 (the event count) = 1, a 4-byte event array (XMemAlloc 0x20840001) into +20; one 72-byte play-wave event (XMemAlloc 0x2084800D, `XACTPlayWaveEvent::XACTPlayWaveEvent` `sub_825D82E8` with the engine, this, the record, no previous event and 0) stored in it and at +36; its slot +8 (Prepare) with r5; on success, when its slot +20 (IsPrepared) says so, state 3. On allocation failure: byte +24 = 0, the event deleted, the array freed, E_OUTOFMEMORY. Returns the result. Only caller `XACTTrack::PrepareSimple` (`sub_825D09D0`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by `XACTTrack::PrepareSimple`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); allocates and frees through the allocation layer (the original unless `--native_heap`); the event's constructor, Prepare and IsPrepared through their plain C++; harness `xact_tracks` (the failure paths only: a new event's Prepare succeeds only with a real wave, which the harness doesn't make). Not called in the attract races of 2026-10-29 (a counter saw `XACTTrack::Prepare` called twice in one, from `XACTComplexSound::Prepare`, and this path not at all). |
| `sub_825D5A48` | `XACTTrackInstance::Prepare` | Native | r4 = the events key, r5 = the time offset. State 2. Walks the track's event records through the cue's sound bank (`[[[track +16] +20] +48] +296`): its slot +24 (the first record, with a 12-byte cursor on the stack holding r4; the count comes back in the cursor's byte +8) and +28 (the next). An array of count words (XMemAlloc 0x20840001) into +20. For each record: the previous event (the array's entry at the record's byte +6, or none when the record's word has 0x20000000), `XACTEvent::Create` (`sub_825D0F00`); a marker (type 9) whose time is before the offset is dropped; else its slot +8 (Prepare) with the offset; a play-wave kind (types 1 to 6) becomes +36; kept in the array. XACTENGINE_E_SEEKTIMEBEYONDCUEEND (0x8AC70019) from an event is noted and the event deleted; with no event kept and one beyond, that's the result. Any other failure deletes every event, frees the array and clears +36. At the end the play-wave event's slot +20 (IsPrepared; its result unused) and state 3. Returns the result. Only caller `XACTTrack::CreateInstance`. Runs in attract races (the complex path: not with the game's simple sounds, probably). Plain C++ in `src/xact/tracks.cpp` (the second migration, 2026-10-29) called directly by `XACTTrack::CreateInstance`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the sound bank's FirstEvent and NextEvent and `XACTEvent::Create` through their plain C++ while they answer (else the guest call), the events' Prepare and IsPrepared through theirs while the slots hold them, the deletes through the guest call; allocates and frees through the allocation layer; harness `xact_tracks` (records walked by the real FirstEvent and NextEvent: the stop event's and types 12's and 14's kept, markers before the offset dropped, XACTENGINE_E_SEEKTIMEBEYONDCUEEND dropped and the result when nothing is kept, the other failures cleaned up; a play-wave event kept, its success path, needs a real wave and is left to the attract race, where the game's two calls keep one). |
| `sub_825D5980` | `XACTTrackInstance::Play` | | When prepared (state 3): +28 = the time (`XACTGetTimeMs` `sub_825BFE28`), state 4, byte +44 = the event count; each event whose record has 0x20000000 is scheduled now (`XACTEvent::Schedule` `sub_825D1918`), the others go to the sound's pending list (`sub_825CB188`: the list at sound + 68). Otherwise byte +32 \|= 0x40 (play when prepared). Returns 0. From `XACTTrack::Play`, `XACTTrack::SwitchInstance` and `XACTTrackInstance::AudioUpdate`. Runs in attract races. |
| `sub_825D5D18` | `XACTTrackInstance::Stop` | Native | r4 = immediate (bool). Nothing when done (6), or stopping (5) unless this is the first immediate stop (+60 clear and r4 set). State 5; an immediate stop sets +60 = 1 and cancels the events (`CancelEvents` `sub_825D5520`). Then, when the play-wave event (+36) is playing (slot +24), its slot +60 (Stop) with r4. An immediate stop that cancelled nothing ends in state 6. Returns 0. From `XACTTrack::Stop` and the stop event (`sub_825D89F8`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by `XACTTrack::Stop`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); CancelEvents and the event's IsPlaying through their plain C++, the event's Stop through the guest call; harness `xact_tracks`. |
| `sub_825D5520` | `XACTTrackInstance::CancelEvents` (probably) | Native | Under XACT's lock (the engine's +144 critical section, `RtlEnterCriticalSection` and `RtlLeaveCriticalSection` called directly): each event in the array whose byte +40 has 0x20 (queued) is taken off the sound's queue (`sub_825CA6F0` → `XACTEvent::Cancel` `sub_825D1830`) and counted; then each entry of the list +48 (its node's +12) the same, until the list is empty. Returns the count. From the destructor and `Stop`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by `~XACTTrackInstance`'s and `Stop`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); XACT's lock entered and left through its imports as the original does (the user's call for XACT's lock, 2026-10-29), `sub_825CA6F0` (never seen in play) through the guest call; harness `xact_tracks` (the helper list empty, as in play). |
| `sub_825D5EB0` | `XACTTrackInstance::AudioUpdate` | | Preparing (2) with a play-wave event (+36) that's prepared (slot +20): state 3. Then a pending play (byte +32 bit 0x40) on a prepared instance clears the bit and plays it (`sub_825D5980`). From `XACTTrack::AudioUpdate`. Runs in attract races. |
| `sub_825D55E0` | `XACTTrackInstance::CheckDone` (probably) | Overridden | State 6 when bytes +44 (events not fired) and +45 (fired, not done) are 0 and the list +48 is empty. Leaf. From `XACTEvent::Done` (`sub_825D13F0`) and `Cancel` (`sub_825D1830`). Runs in attract races. Named in `../loops/leads.md` among XACT's waves' blockers (under `XACTWave::AudioUpdate`, through `Done`). Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D5DE8` | `XACTTrackInstance::AddHelper` (probably) | | r4 = a node: linked at the tail of the list +48. Leaf. From the wave-variation event's `sub_825D7908`. Not seen in play. |
| `sub_825D5E08` | `XACTTrackInstance::RemoveHelper` (probably) | | r4 = an event: unlinks the node of the list +48 whose +12 is r4 (and links it to itself). Leaf. From `XACTEvent::Done` and `Cancel`, for uncounted events (byte +40 0x08). Not seen in play. |
| `sub_825D5678` | `XACTTrackInstance::SetVolume` | Native | f1: tail call of the play-wave event's (+36) slot +64 (`sub_825D7FE0`), when there is one. From `XACTTrack::SetVolume`, `UpdateVolume` and `ResetRpc`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by `XACTTrack::SetVolume`'s and `UpdateVolume`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D5698` | `XACTTrackInstance::SetReverbSend` (probably) | | f1: the play-wave event's slot +68 (`sub_825D8048`). From `XACTTrack::SetReverbSend`. Not seen in play. |
| `sub_825D56B8` | `XACTTrackInstance::SetPitch` | | f1: the play-wave event's slot +72 (`sub_825D74F0`). From `XACTTrack::SetPitch`, `UpdatePitch`, `ResetRpc`. Not seen in play. |
| `sub_825D56D8` | `XACTTrackInstance::SetOutputVoices` (probably) | | r4: the play-wave event's slot +120 (`sub_825D80B0`). From `sub_825D0510`. Not seen in play. |
| `sub_825D56F8` | `XACTTrackInstance::SetOutputMatrices` (probably) | | r4: the play-wave event's slot +124 (`sub_825D8118`). From `sub_825D0528`. Not seen in play. |
| `sub_825D5718` | `XACTTrackInstance::SetOutputs` (probably) | | r4: the play-wave event's slot +76 (`sub_825D8180`). From `sub_825D0540`. Not seen in play. |
| `sub_825D5738` | `XACTTrackInstance::SetMainMatrix` (probably) | | r4: the play-wave event's slot +80 (`sub_825D7418`). From `sub_825D0558`. Not seen in play. |
| `sub_825D5758` | `XACTTrackInstance::SetChannelMatrix` (probably) | | r4 to r6: the play-wave event's slot +84 (`sub_825D7480`); 0 without one. From `sub_825D0570`. Not seen in play. |
| `sub_825D5780` | `XACTTrackInstance::IsBusy` (probably) | Native | The play-wave event's slot +44 (`sub_825D81E8`), or 0 without one. From `XACTTrack::IsBusy`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp`, called directly by `XACTTrack::IsBusy`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D57A8` | `XACTTrackInstance::Pause` | Native | r4: the play-wave event's slot +88 (`sub_825D8268`: the wave's Pause), or 0 without one. From `XACTTrack::Pause`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp`, called directly by `XACTTrack::Pause`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D5620` | `XACTTrackInstance::UsesWaveBank` (probably) | | r4 = a wave bank: 1 when the play-wave event's slot +52 (its wave's bank) is r4, else 0. From `XACTTrack::UsesWaveBank`. Not seen in play. |
| `sub_825D0F00` | `XACTEvent::Create` | Native | r3 = the engine, r4 = the track instance, r5 = the event's record, r6 = the previous event (for timing), r7 = where the event goes. By the record's type (word +0, low 5 bits; above 20 returns 0x8AC70011) through the jump table at `0x825D0F4C`: XMemAlloc of the class's size (attributes 0x208480xx by class) and its constructor with r3 to r6 (and the class's extra: the record + 12, + 18, + 22 or + 11, or a flag): 0 stop (48 bytes, `sub_825D8950`); 1, 4 and 2, 5 play wave (72, `sub_825D82E8`, with 0 or the word at the record + 12: a load, corrected 2026-10-29 by the migration); 3, 6 wave variation (188, `sub_825D7AA0`); 7, 16 pitch (88, `sub_825D67E8`); 8, 17 volume (88, `sub_825D6308`); 9, 18 marker (72, `sub_825D5FE8`); 10, 11 (56, `sub_825D1A80` with 0 or 1); 12 (52, `sub_825D70C8`); 13, 15 variable (64, `sub_825D6CF0`); 14 (48, `sub_825D5F40`); 19 (56, `sub_825D71C0`); 20 makes nothing. The event to [r7]; E_OUTOFMEMORY when [r7] ends 0, else 0. Only caller `XACTTrackInstance::Prepare` (`sub_825D5A48`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by `XACTTrackInstance::Prepare`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers): the play-wave types (1, 2, 4, 5), 20 and above 20; the other types (never seen in play) decline before writing anything and the original runs; XMemAlloc through the allocation layer (the original unless `--native_heap`), the play-wave event's constructor through its plain C++; harness `xact_tracks`. |
| `sub_825D0F4C` | | | Not a function: `XACTEvent::Create`'s 21-entry jump table (case addresses `825D0FA0` to `825D12F8`), decoded as code by the recompiler. Never runs. Not seen in play. |
| `sub_825D1760` | `XACTEvent::XACTEvent` | Native | r3 = the event, r4 = the engine (+12), r5 = the track instance (+44), r6 = the record (+16), r7 = the previous event (+36), r8 = uncounted (byte +40 = 0x08 when set, else 0). +4 and +8 linked to itself (the node in the sound's queue), vtable `0x82076A10`, +20, +24, +28, +32 = 0. Leaf. From every event constructor (`sub_825D82E8` among those seen). Runs in attract races. Plain C++ in `src/xact/tracks.cpp`, called directly by `XACTPlayWaveEvent::XACTPlayWaveEvent`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D0EF0` | `XACTEvent::~XACTEvent` | Native | Puts back the base vtable `0x82075344`. Leaf. From the events' destructors (`sub_825D8660` seen; 10 callers). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by the play-wave event's deleting destructor's plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D17E0` | `XACTEvent::~XACTEvent` (deleting) | | Slot +0 of `0x82076A10`: the base vtable, XMemFree(0x2084805E) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D17D0` | `XACTEvent::Prepare` | | Slot +8 of the base: +32 = r4 (the time offset already elapsed), returns 0. Leaf. No direct callers. Not seen in play (the play-wave event has its own). |
| `sub_825D17B0` | `XACTEvent::SetQueued` (probably) | | Slot +16 of the base: byte +40 bit 0x20 = (r4 != 0). Leaf. Called by `XACTSound::ScheduleEvent` with 1 and by Fire and Cancel with 0. Not seen in play (the play-wave event has its own, `sub_825D8530`). |
| `sub_825D1918` | `XACTEvent::Schedule` | | Works out the fire time (+20) and end (+28), then `XACTSound::ScheduleEvent` (`sub_825CB0C8`) on the sound (`[[+44] +16] +20`). The base: with the record's 0x20000000 the time now (`XACTGetTimeMs`) for an uncounted event (0x08) or the instance's start (+28); else from the previous event (+36): its +20 with the record's 0x40000000, its +24 (done time) when it's done (0x40), else its +28. Plus the record's time (bits 5-28 of word +0: 24 bits, with a test for a sign bit above them that can never be set, so unsigned), less +32 (cleared); plus, when the record's 16-bit +4 is set, a random 0 to it (`sub_825BF650`). +28 = +20 + slot +12 (the duration), or 0x7FFFFFFF when that's 0x7FFFFFFF. From `XACTTrackInstance::Play`, `XACTSound::AudioUpdate` (for pending events) and `sub_825D7908`. Runs in attract races. |
| `sub_825D18F0` | `XACTEvent::GetStartTime` (probably) | | The record's time (bits 5-28 of word +0; negated when bit 24 of those 24 bits is set, which can't happen, so unsigned). Leaf. From `XACTSound::AudioUpdate`, to move pending events to the queue. Not seen in play. |
| `sub_825D1358` | `XACTEvent::Fire` | Overridden | Slot +4 of the base: the first time (byte +40 without 0x80), sets 0x80 and, for a counted event, adds 1 to the instance's byte +45; slot +16 with 0; for a counted event, the instance's byte +44 less 1. From the events' own Fire (`sub_825D8378` seen, 10 callers) and slot +4 of several vtables. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D13F0` | `XACTEvent::Done` | | An uncounted event (0x08) is taken off the instance's helper list (`sub_825D5E08`); otherwise byte +40 \|= 0x40, the instance's byte +45 less 1, +24 = the time (`XACTGetTimeMs`) and `XACTTrackInstance::CheckDone` (`sub_825D55E0`). From `XACTWave::AudioUpdate` (`sub_825D2560`, through the wave's owner: the wave done) and the events' Fire (9 callers). Runs in attract races. |
| `sub_825D1830` | `XACTEvent::Cancel` (probably) | | Unlinks its node (+4/+8) from the sound's queue, slot +16 with 0; an uncounted event leaves the helper list (`sub_825D5E08`); a counted one takes 1 from the instance's byte +45 when fired (0x80) and from +44, then `CheckDone`. From `sub_825CA6F0` (`XACTSound`'s, a tail call with r4 as r3; from `CancelEvents`) and `XACTSound::AudioUpdate`. Not seen in play. |
| `sub_825D0E78` | `XACTEvent::ResetTimes` (probably) | | +20, +24, +28, +32 = 0, byte +40 keeps only 0x08. Leaf. From `sub_825D7908` (the wave-variation event's inner marker). Not seen in play. |
| `sub_825D0EA0` | `XACTOctavesToCents` (probably) | | f1 = a pitch in octaves: 2400 at 2.0 or above, -2400 at -2.0 or below, else f1 × 1200 truncated (16-bit). Leaf. From `sub_825D1478` and the pitch events (`sub_825D6AD8`). Not seen in play. |
| `sub_825D1A80` | `XACTTransitionEvent::XACTTransitionEvent` (probably) | | Types 10 and 11 (56 bytes): the base constructor (counted), +52 = r8 (0 for type 10, 1 for 11), +48 = 0, vtable `0x82076A24`. From `XACTEvent::Create`. Not seen in play. |
| `sub_825D1AD8` | `XACTTransitionEvent::~XACTTransitionEvent` (deleting) (probably) | | Slot +0 of `0x82076A24`: the vtable, the base destructor, XMemFree(0x2084800A) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D1B40` | `XACTTransitionEvent::IsDue` (probably) | | 1 when +48 is set or `sub_825BF7A8`(engine, the record + 7) (probably a variable condition) is true, else 0. From `XACTSound::AudioUpdate` (types 10 and 11). Not seen in play. |
| `sub_825D1B88` | `XACTTransitionEvent::Fire` (probably) | | Slot +4: the base Fire; when +48 is set or the condition (`sub_825BF7A8`, the record + 7) holds: the track's (`[+44] +16`) +36 = this when it has no next instance or that's in state 0, `XACTEvent::Done`, and `XACTTrack::SwitchInstance`. No direct callers. Not seen in play. |
| `sub_825D5F40` | | | Type 14 (48 bytes): the base constructor (counted), vtable `0x82076ADC` (+0 `sub_825D5F80`, +4 the base Fire). From `XACTEvent::Create`. Not seen in play. |
| `sub_825D5F80` | | | Slot +0 of `0x82076ADC`: the vtable, the base destructor, XMemFree(0x2084800F) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D70C8` | | | Type 12 (52 bytes): the base constructor (counted), +48 = 0, vtable `0x82076B44`. From `XACTEvent::Create`. Not seen in play. |
| `sub_825D7110` | | | Slot +4 of `0x82076B44`: the base Fire, then `XACTEvent::Done`. No direct callers. Not seen in play. |
| `sub_825D7148` | | | Slot +0 of `0x82076B44`: the vtable, the base destructor, XMemFree(0x20848014) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D71C0` | | | Type 19 (56 bytes): the base constructor (counted), +48 and +52 = 0, vtable `0x82076BD8`. From `XACTEvent::Create`. Not seen in play. |
| `sub_825D73B8` | | | Slot +8 of `0x82076BD8` (Prepare): `sub_825C7358` on the cue's sound bank with the record's word +7 into a stack word; on success +52 = it (an object with a vtable: slots +12, +64, +68 used). No direct callers. Not read in detail. Not seen in play. |
| `sub_825D7210` | | | Slot +4 of `0x82076BD8` (Fire): the base Fire; the object at +52, when set, gets its slot +64 with 1 and +52 = 0; then `XACTEvent::Done`. No direct callers. Not seen in play. |
| `sub_825D7270` | | | Slot +20 of `0x82076BD8`: the object at +52's slot +68, bit 2 of its result (as 0 or 1); 1 without an object. No direct callers. Not seen in play. |
| `sub_825D7328` | | | Slot +0 of `0x82076BD8`: the object at +52's slot +12, the wave event base's vtable `0x82076B58`, the base destructor, XMemFree(0x2084800C) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D72C0` | `XACTWaveEventBase::~XACTWaveEventBase` (deleting) (probably) | | Slot +0 of `0x82076B58`: the vtable, the base destructor, XMemFree(0x20848063) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D71B0` | | | Returns 0x8AC70006. Leaf. Slots of the wave event base and +108 of the play-wave event (the wave's `PrepareNext` asks the owner for the next wave there: none for a plain play-wave event). No direct callers. Not seen in play. |
| `sub_825D82E8` | `XACTPlayWaveEvent::XACTPlayWaveEvent` | Native | Types 1, 2, 4, 5 (72 bytes, XMemAlloc 0x2084800D): the base constructor (counted), vtable `0x82076CE8`, +48 = 0, byte +52 = the record's byte +11 (the loop count, 255 forever), +56 = r8 (0, or the word at the record + 12 for types 2 and 5), +60 (the wave), +64 (the play time) and +68 (the stop time) = 0. From `XACTEvent::Create` and `XACTTrackInstance::PrepareSimple`. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` called directly by `XACTEvent::Create`'s and `XACTTrackInstance::PrepareSimple`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D8660` | `XACTPlayWaveEvent::~XACTPlayWaveEvent` (deleting) | Overridden | Slot +0: the vtable; the wave (+60), when set, deleted (slot +0 with 1); the wave event base's vtable `0x82076B58`, the base destructor, XMemFree(0x2084800D) when r4 bit 0. No direct callers. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the wave's delete through the guest call, `~XACTEvent` and XMemFree as plain C++ (the original unless `--native_heap`); harness `xact_tracks`. |
| `sub_825D86F8` | `XACTPlayWaveEvent::Prepare` | Overridden | Slot +8: r4 = the time offset. The record's time (bits 5-28 of word +0): when it's after the offset, +32 = the offset and no seek; else +32 = the time and the seek = offset - time. The wave bank: `XACTSoundBank::GetWaveBank` (`sub_825C7218`) on the cue's sound bank (+296) with the record's byte +10; none gives XACTENGINE_E_NOWAVEBANK (0x8AC70013), one not prepared (+252 = 0) 0x8AC70016. The bank's slot +40 (`XACTWaveBank::CreateWave`, `sub_825CECE8`) with the record's 16-bit +8 (the wave index) and this as the owner; the wave's slot +4 (Prepare) with the loop count (own slot +48), 0 and the seek; own slot +104 (SetWave) with it; the wave linked at the tail of the bank's wave list (+240). A failure deletes the wave. Returns the result. No direct callers (`XACTTrackInstance::PrepareSimple` and `Prepare` call slot +8). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the getters and slots +48 and +104 through their plain C++ while the slots hold them, the wave bank's CreateWave and the wave's Prepare and delete through the guest call; GetWaveBank as GetDuration; harness `xact_tracks` (CreateWave and the wave's Prepare as recorders). |
| `sub_825D8870` | `XACTPlayWaveEvent::GetDuration` | Overridden | Slot +12: the wave bank (`GetWaveBank`, record byte +10); 0 without one; 0x7FFFFFFF when the loop count (record byte +11) is 255; else `XACTWaveBank::GetWaveDurations` (`sub_825CC380`) for the record's wave index: the loop count × the loop length + the duration (ms); 0 when it fails. No direct callers (`XACTEvent::Schedule` calls slot +12). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); `XACTSoundBank::GetWaveBank` and `XACTWaveBank::GetWaveDurations` through their plain C++ where they answer (GetWaveBank only under `--native_heap`), else the guest call; harness `xact_tracks`. |
| `sub_825D8378` | `XACTPlayWaveEvent::Fire` | | Slot +4: the base Fire; the wave (slot +56), when there is one, gets its slot +8 (Play) with 1 (deferred), 0, 0; +64 = the time (`XACTGetTimeMs`). No direct callers (`XACTSound::ScheduleEvent` and `AudioUpdate` call slot +4). Runs in attract races. |
| `sub_825D83F8` | `XACTPlayWaveEvent::Stop` | | Slot +60: r4 = immediate. Not immediate: +68 = the time. With a wave: its slot +12 (Stop) with the mode (0 when immediate; else 1 when the record's byte +7 bit 0 (release) is set or the track has a release RPC, `sub_825D05C0`; else 0) and the record's byte +7 bit 0 (exit the loop); returns its result, else 0. No direct callers (`XACTTrackInstance::Stop`). Runs in attract races. |
| `sub_825D8530` | `XACTPlayWaveEvent::SetQueued` (probably) | Overridden | Slot +16: byte +40 bit 0x20 = (r4 != 0) and bit 0x40 = (the wave's +60 is 0) (the wave read without a null check). Leaf. No direct callers. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D8358` | `XACTPlayWaveEvent::GetLoopCount` | Overridden | Slot +48: byte +52. Leaf. No direct callers (its Prepare). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D8360` | `XACTPlayWaveEvent::ForgetWave` (probably) | Overridden | Slot +28 of both play-wave events: a tail call of own slot +104 with 0 (SetWave: the wave forgotten). No direct callers (the wave's `Release` and `Finish` call the owner's +28). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_82483208` | `XACTPlayWaveEvent::SetWave` | Overridden | Slot +104 of the play-wave event: +60 = r4. A one-instruction setter in the movie library's range (`82481A70`-`82575B07`, `movies.md`, not mapped: replaced), probably folded with its code; in play only the play-wave event reaches it (its Prepare and `ForgetWave`). Leaf. Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D84C0` | `XACTPlayWaveEvent::GetWaveBank` (probably) | Overridden | Slot +52 of both play-wave events: the wave's (slot +56) bank (+36), or 0. No direct callers (`XACTTrackInstance::UsesWaveBank`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D8570` | | | Slot +32: own slot +96, then, when +48 is set, `sub_825CADF0` on the sound (its cue's slot +92 with the sound). No direct callers (the wave's audio update calls the owner's slot +32 on its +28 request). Not seen in play. |
| `sub_825D85C8` | `XACTPlayWaveEvent::GetTimeLeft` (probably) | | Slot +40: from the time now, the fire time (+20) and the wave's slots +28 (the length with r4 = 0) and +32 (the duration): the ms left. Not read in detail. No direct callers. Not seen in play. |
| `sub_825D8920` | `XACTPlayWaveEvent::GetVariationRecord` (probably) | Overridden | Slot +92: the record + 12 for type 4, + 16 for type 5, else 0 (the volume and pitch variation). Leaf. No direct callers (`sub_825D7558`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D7558` | `XACTPlayWaveEvent::ApplyVariation` (probably) | Overridden | Slot +96 of both play-wave events: with a variation record (own slot +92), `sub_825D1478` with it. No direct callers (the wave's Play calls the owner's slot +96). Runs in attract races (never with a record). Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); own slot +92 through its plain C++ while it holds it, `sub_825D1478` (never seen in play) through the guest call; harness `xact_tracks`. |
| `sub_825D1478` | `XACTPlayWaveEvent::SetVariation` (probably) | | r4 = a 7-byte variation record: 16-bit +0 and +2 a pitch range (cents), bytes +4 and +5 a volume range, byte +6 flags (0x40 pitch, 0x80 volume, 0x10 and 0x20 vary once only, bits 2-3 and 0-1 how to apply: replace, add, subtract or multiply). Picks a random value in each range (`sub_825BF6B8`), applies it to the track's pitch (cents to octaves, clamped to ±2400, `XACTTrack::SetPitch`) and volume (the millibel table at `0x82076600`, -9600 to 600; log10 × 2000 of the track's volume; pow(10, mb × 0.0005), clamped to 6400 mb, `XACTTrack::SetVolume`); byte +40 \|= 0x10 (varied). Not read in detail. From `sub_825D7558`. Not seen in play. |
| `sub_825D76A0` | `XACTPlayWaveEvent::IsPrepared` | Overridden | Slot +20 of both play-wave events: 1 when the wave (slot +56) exists and its state (+40) is 2 or more, else 0. No direct callers (the instance's Prepare and AudioUpdate). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D7708` | `XACTPlayWaveEvent::IsPlaying` | Overridden | Slot +24 of both: 1 when the wave's state is 3 to 5, else 0. No direct callers (`XACTTrack::Stop`, `XACTTrackInstance::Stop`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D81E8` | `XACTPlayWaveEvent::IsBusy` (probably) | Overridden | Slot +44 of both: the wave's slot +20 (`sub_825C8388`, 0, for the in-memory wave; the streaming wave's `sub_825D4530`), or 0 without a wave. No direct callers (`XACTTrackInstance::IsBusy`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D8268` | `XACTPlayWaveEvent::Pause` | Overridden | Slot +88: the wave's slot +36 (`XACTWave::Pause`) with r4, or 0 without a wave. No direct callers (`XACTTrackInstance::Pause`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D7FE0` | `XACTPlayWaveEvent::SetVolume` | Overridden | Slot +64 of both: f1 to the wave's (slot +56) `XACTWave::SetVolume` (`sub_825D1CC8`), when there is one. No direct callers (`XACTTrackInstance::SetVolume`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D8048` | `XACTPlayWaveEvent::SetReverbSend` (probably) | | Slot +68 of both: f1 to `sub_825D1CE8` on the wave. No direct callers. Not seen in play. |
| `sub_825D74F0` | `XACTPlayWaveEvent::SetPitch` | | Slot +72 of both: f1 to `XACTWave::SetPitch` (`sub_825D1D30`). No direct callers. Not seen in play. |
| `sub_825D8180` | `XACTPlayWaveEvent::SetOutputs` (probably) | | Slot +76 of both: r4 to `sub_825D1D90` on the wave. No direct callers. Not seen in play. |
| `sub_825D7418` | `XACTPlayWaveEvent::SetMainMatrix` (probably) | | Slot +80 of both: r4 to `sub_825D1DB0` on the wave. No direct callers. Not seen in play. |
| `sub_825D7480` | `XACTPlayWaveEvent::SetChannelMatrix` (probably) | | Slot +84 of both: r4 to r6 to `sub_825D1DD0` on the wave; 0 without a wave. No direct callers. Not seen in play. |
| `sub_825D80B0` | `XACTPlayWaveEvent::SetOutputVoices` (probably) | | Slot +120 of both: r4 to `sub_825D1D50` on the wave. No direct callers. Not seen in play. |
| `sub_825D8118` | `XACTPlayWaveEvent::SetOutputMatrices` (probably) | | Slot +124 of both: r4 to `sub_825D1D70` on the wave. No direct callers. Not seen in play. |
| `sub_825D1CC8` | `XACTWave::SetVolume` | Native | r3 = a wave: a tail call of its voice's (+44) slot +4 (`XACTVoice::SetVolume`, `xact-waves.md`) with f1, when it has one. From `XACTPlayWaveEvent::SetVolume` (`sub_825D7FE0`). Runs in attract races. Plain C++ in `src/xact/tracks.cpp`, called directly by `XACTPlayWaveEvent::SetVolume`'s plain C++ (its adapter removed 2026-10-29: no recompiled caller in play; `--native_xact_tracks` now switches only the callers); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D1CE8` | `XACTWave::SetReverbSend` (probably) | | The voice's slot +44 (`XACTVoice::SetReverbSend`) with a pointer to f1 and 1. From `sub_825D8048`. Not seen in play. |
| `sub_825D1D30` | `XACTWave::SetPitch` | | The voice's slot +8 (`XACTVoice::SetPitch`) with f1. From `sub_825D74F0`. Not seen in play. |
| `sub_825D1D50` | `XACTWave::SetOutputVoices` (probably) | | The voice's slot +28 (`XACTVoice::SetOutputVoices`) with r4. From `sub_825D80B0`. Not seen in play. |
| `sub_825D1D70` | `XACTWave::SetOutputMatrices` (probably) | | The voice's slot +36 (`XACTVoice::SetOutputMatrices`). From `sub_825D8118`. Not seen in play. |
| `sub_825D1D90` | `XACTWave::SetOutputs` (probably) | | The voice's slot +32 (`XACTVoice::SetOutputs`). From `sub_825D8180`. Not seen in play. |
| `sub_825D1DB0` | `XACTWave::SetMainMatrix` (probably) | | The voice's slot +40. From `sub_825D7418`. Not seen in play. |
| `sub_825D1DD0` | `XACTWave::SetChannelMatrix` (probably) | | The voice's slot +48 with r4 to r6; 0 without a voice. From `sub_825D7480`. Not seen in play. |
| `sub_825D7AA0` | `XACTWaveVariationEvent::XACTWaveVariationEvent` (probably) | | Types 3 and 6 (188 bytes, XMemAlloc 0x2084800E): the base constructor (counted), vtable `0x82076C60`, +48 = 0; a variation table at +52 (`XACTVariationTable::XACTVariationTable` `sub_825D3360` with the engine and the record + 9, or + 16 for type 6); byte +68 = the record's byte +8 (the loop count); +72 (the wave), +88, +92, +96 = 0; an inner, uncounted marker event at +100 (`sub_825D5FE8` with the engine, the instance and the record at +76); a 16-byte node at +172 (vtable `0x82076C58`, linked to itself, +12 = the inner event). From `XACTEvent::Create`. Not seen in play. |
| `sub_825D7BA0` | `XACTWaveVariationEvent::~XACTWaveVariationEvent` (probably) | | The vtable; the wave (+72) deleted; the node +172 unlinked (the base vtable); the inner event's destructor (`sub_825D6050`); the variation table's (`sub_825D2AD8`); the wave event base's vtable, the base destructor. Only caller `sub_825D7F88`. Not seen in play. |
| `sub_825D7F88` | `XACTWaveVariationEvent::~XACTWaveVariationEvent` (deleting) (probably) | | Slot +0: `sub_825D7BA0`, XMemFree(0x2084800E) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D7F18` | | | Slot +0 of `0x82076C58` (the wave-variation event's node): unlinks it, the base vtable, XMemFree(0x2084802C) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D7C48` | `XACTWaveVariationEvent::Prepare` (probably) | | Slot +8: r4 = the time offset. As the play-wave event's, over the variations: until the seek falls inside a wave, the next variation (own slot +108: a bank byte and a 16-bit wave index), its wave bank and wave (`GetWaveBank`, the bank's slot +40); the wave's length (slot +32, or +28 with the loop count when the record's word has 0x00400000 clear) taken off the seek, the loop count byte +68 lowered (unless 255); then the wave's Prepare, SetWave (slot +104) and the bank's wave list. 0x8AC70013, 0x8AC70016 or 0x8AC70019 (the seek beyond the end). Not read in detail. No direct callers. Not seen in play. |
| `sub_825D7618` | `XACTWaveVariationEvent::Fire` (probably) | | Slot +4: the base Fire; the wave's Play (slot +8 with 1, 0, 0); on success +96 = the time; +88 = the time. No direct callers. Not seen in play. |
| `sub_825D7790` | `XACTWaveVariationEvent::GetDuration` (probably) | | Slot +12: `XACTWaveBank::GetWaveDurations` for the current wave's bank (+36) and index (+52): the record's byte +8 × the loop length + the duration; 0 without a wave or on failure. No direct callers. Not seen in play. |
| `sub_825D7848` | `XACTWaveVariationEvent::Stop` (probably) | | Slot +60: as the play-wave event's Stop, with the stop time at +92. No direct callers. Not seen in play. |
| `sub_825D75A8` | `XACTWaveVariationEvent::Pause` (probably) | | Slot +88: the wave's slot +36 with r4, or 0. No direct callers. Not seen in play. |
| `sub_825D7908` | `XACTWaveVariationEvent::ScheduleNext` (probably) | | Slot +100 (the wave's Play calls the owner's +100 when looping with a count): the current wave's durations (`GetWaveDurations`, less the engine's +40 lead time, or 0); the inner marker reset (`sub_825D0E78`), its record at +76 = 0x20000029 (type 9, time 1) with +80 = 0, +82 = 255, +83 = 0; +168 = the wave; the node +172 added to the instance's helper list (`sub_825D5DE8`); the inner marker scheduled (`XACTEvent::Schedule`). Not read in detail. No direct callers. Not seen in play. |
| `sub_825D79E8` | `XACTWaveVariationEvent::GetTimeLeft` (probably) | | Slot +40: the wave's slot +32 (the duration) less the time since +96. No direct callers. Not seen in play. |
| `sub_825D7A48` | | | Slot +128: own slot +104, then, when +48 is set, `sub_825CADF0` on the sound. No direct callers. Not seen in play. |
| `sub_825D7B70` | `XACTWaveVariationEvent::GetLoopCount` (probably) | | Slot +48: byte +68. Leaf. No direct callers. Not seen in play. |
| `sub_825D7B78` | `XACTWaveVariationEvent::SetWave` (probably) | Overridden | Slot +104: +72 = r4. Leaf. A folded one-instruction setter: also called directly by the movie library's `sub_82485BA0` (replaced, not mapped), which is the play coverage's caller. Seen in play (probably from the movie library before its replacement; XACT never made this event). Plain C++ in `src/xact/tracks.cpp` behind an adapter (`src/hooks/xact_tracks.cpp`, `--native_xact_tracks`); the objects' slots through their plain C++ while they hold it, the waves' and voices' through the guest call; harness `xact_tracks`. |
| `sub_825D7B80` | `XACTWaveVariationEvent::GetVariationRecord` (probably) | | Slot +92: the record + 9 for type 6, else 0. Leaf. No direct callers. Not seen in play. |
| `sub_825D7698` | `XACTWaveVariationEvent::NextVariation` (probably) | | Slot +108: a tail call of `sub_825D35A8` (`xact-waves.md`) on the variation table (+52). No direct callers. Not seen in play. |
| `sub_825D8950` | `XACTStopEvent::XACTStopEvent` (probably) | | Type 0 (48 bytes): the base constructor (counted), vtable `0x82076D68`. From `XACTEvent::Create` and the jump table's data (`sub_825D0F4C`). Not seen in play. |
| `sub_825D8990` | `XACTStopEvent::~XACTStopEvent` (deleting) (probably) | | Slot +0 of `0x82076D68`: the vtable, the base destructor, XMemFree(0x20848013) when r4 bit 0. No direct callers. Not seen in play. |
| `sub_825D89F8` | `XACTStopEvent::Fire` (probably) | | Slot +4: the base Fire; with the record's byte +7 bit 1 the cue's slot +4 (`XACTCue::Stop`) with bit 0 (immediate); else `XACTTrackInstance::Stop` (`sub_825D5D18`) with bit 0, or 1 when the track has no release RPC (`sub_825D05C0`); then `XACTEvent::Done`. No direct callers. Not seen in play. |
| `sub_825D0020` | | | `sub_825CF918` (a destructor, not read), then XMemFree(0x20848025) when r4 bit 0: a scalar deleting destructor. From `sub_825CF910`. Probably the streaming wave bank's (`xact.md`'s range). Not seen in play. |
| `sub_825D0078` | | | When +252 is 0 (not prepared): +20's slot +192 with +260, +276, a stack word and 0, and on success own slot +80; else `sub_825CFF90` and `XACTWaveBank::DoWork` (`sub_825CC560`). No direct callers. Probably the streaming wave bank's DoWork (`xact.md`'s range). Not read in detail. Not seen in play. |
