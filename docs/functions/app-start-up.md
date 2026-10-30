# Known functions: the app's start-up

The app's constructor (`App::App`, `sub_82094590`, made once at boot by
`App::Create`, `framework.md`) and the start-up it ends with
(`App::StartUp`, `sub_82106548`): the app's six vtables, its four
handler maps, its tick handler and run-loop listener, the objects it
owns (the state stack, `app-states.md`; a second camera class, a lead in
`../loops/leads.md`; the sound manager, `sound-cues.md`; the movie,
`movies.md`), then the start-up's sequence of set-ups, each given the
app's interface at app + 16 (vtable `0x82000E38`): the globals that
keep that interface (`0x82600344`, `0x8266D084`, `0x8266D088`,
`0x826BABA0`), the scene's 1024 × 576 target and its tile surfaces
(`[0x8266D08C]`), the archive (`Archive::Init`, `files-and-saves.md`;
`Archive::WaitIdle`, `archive.md`), the action mappers
(`action-mappers.md`), the camera passes (`cameras.md`), the screen
fades (`mode-tick.md`), the movie class (`movies.md`), the light
billboards' shaders and vertex declaration, the sound globals
(`car-sounds.md`), the save interface (`save-interface.md`), the network
manager (`system-services.md`), the text (`strings-and-font.md`) and the
players' controllers (`save-data.md`). Those callees keep their rows in
their files; this file has the app's own functions, the billboards' and
the scene target's set-up, and the parts of the voice engine's set-up
(XHV, under the network manager's `sub_8214F6E8`) that had no rows,
beside the voice rows in `system-services.md` (`Voice::Init`
`sub_8257B7A8`, `sub_82578BF8`, `sub_8257B688`, the local talkers'
`sub_8257E568`, the thread `sub_8257B5D8`).

The voice engine isn't made (the user's call, 2026-10-30: Xbox features
a subsystem sets up for its own use are not made, their game-level users
skipped with what they'd read with the feature absent): the network
manager's voice set-up (`sub_8214F6E8`, `system-services.md`) is
Dropped in `src/hooks/voice.cpp`, keeping only its two words (+456 the
machine id, +444 the tick count), so the engine pointer (the manager's
+0, `0x826C74A0`) stays 0 as the manager's constructor (`sub_8214F660`)
leaves it, and the set-up's parts below (`XHVCreateEngine`,
`Voice::Init`, the talkers, `Voice::CreateVoices`, the thread and its
event, listener and critical section) no longer run: Dropped, not
ported. The engine's readers (read 2026-10-30, greps over `generated/`
for the manager's address and its callers): Network::Update's per-frame
chat data read (`sub_8214FA78`) and the Live users' registration
(`sub_8214F810`), left out by its override since B5; the UI's headset
checks (+52 in `sub_8226BCB8` and `sub_8226EEC8`, reached through app
message handlers bound in `sub_82279118`), each behind
`sub_82260908`, which answers 0 unless the session state at
`0x826C6AA8` is in 0x1000..0x100C (only system link and Live code
sets it; 0 at boot from `sub_8214E190` and `sub_822915C0`); system link
code (`sub_8214B330`, `sub_8214D7A0` and what they call: no mode for
it); and the release (`sub_8214F690`, from the exit's `sub_825DB040`,
never in play), which checks for null. An attract race (`itc_2576.log`)
and the pad replay through the menus and a driven race (`itc_2577.log`,
127 s at 30 ticks/s, no error lines) ran with no engine.

What the native port meets (read 2026-10-30, the B7 loop's mapping
agent): the start-up's own callees and the scene target are plain
memory and the graphics resources already ours (`Device::CreateSurface`,
`CreateTexture`, the device lock); the billboards' compiles are the
stored shaders (`--use_hardcoded_shaders`); `CreateVertexDeclaration`
is a leaf over `XMemAlloc` and `memset`/`memcpy` (ours); by the
graphics rule it's native if the renderer needs the object (the draws
set it on the device; probably needed, not checked). Everything that blocks the start-up
and the constructor is the voice engine's set-up under the network
manager (`sub_8214E190` → `sub_8214F6E8` → `sub_82578C78` →
`sub_82578BF8` → `Voice::Init`): its kernel objects and imports are an
event (`CreateEvent` `sub_82295E78`, `NtCreateEvent`, the core's), a
notification listener (`XamNotifyCreateListener`, inline; B5's step 6
answered the game's other listeners with a fixed nonzero handle,
`sub_822952B8`), a critical section (`RtlInitializeCriticalSection`,
the engine's +1272, taken by its +8 slot), the voice thread
(`ExCreateThread` through `sub_825BA380`; its entry `sub_8257B5D8` is
Dropped: it sets the started event and returns), the wait for the
thread to start (`WaitForSingleObject` `sub_822970A0`, kept recompiled
by the user's call, 2026-10-24), the four local talkers' profile reads
(`XamUserReadProfileSettings` inside `sub_8257E568`; B5's step 7 has
the profile as hardcoded data, `src/user/profile.cpp`) and XAudio's
voices (already native, `xaudio.md`). B5 left voice to B7 with the
network manager (the user agreed, 2026-10-21) and ruled out an engine
built without its parts (a stand-in); the user's call (2026-10-30, above)
drops it instead.

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

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82094590` | `App::App` | | The app's constructor: r3 = the app (132 bytes from `App::Create`, `framework.md`), r4 = the block (+0 the run loop, +4 the framework's +36 interface). Sets the base classes' vtables (+4 `0x82000ABC`, +8 `0x82000A74`, +12 `0x82000D48`, +16 `0x82000DD0`, +20 `0x82000DA0`), then its own (+0 `0x82000EA0`, +4 `0x82000E94`, +8 `0x82000E80`, +12 `0x82000E70`, +16 `0x82000E38`, +20 `0x82000E08`); the four handler maps (+28, +40, +52, +64, `HandlerMap::HandlerMap`); +80, +84, +88 (the message queue) and +100, +104, +108 (the requests) = 0, bytes +112, +113, +114 = 0; +24 = the block's +4. Registers on the run loop (its vtable): slot 2 `AddTickHandler` with key 8 and a delegate (vtable `0x8200833C`, the app, member pointer {`App::Update` `0x82093DC0`, 0}), slot 1 `AddListener` with key 9 and the delegate for `sub_82092DE0`. Then +116 = `AppStates::Create({the app})` (`app-states.md`); +120 = a 368-byte object (`operator new`, `sub_820A8B88` with {app + 20, app + 4}: the second camera class, a lead in `../loops/leads.md`), or 0; +128 = a 312-byte `Movie::Movie` with {app + 20, app + 8} (`movies.md`), or 0; +124 = `AudioManager::Create(app + 12, "txr_xact" (`0x82000EAC`), &3)` (`sound-cues.md`); then `App::StartUp(app + 16)`. Returns the app. Once at boot. Ready only after the start-up (`b7_groups.py`: 43 functions in its group with the camera class and the sound manager's). |
| `sub_82092DE0` | `App::OnRunLoopEvent` (probably) | | The app's run-loop listener (key 9, registered by `App::App`): r3 = the app, r4 = the run loop, r5 = the event (+0 its code). Code 0: `App::DeliverMessage(app, 5)` (`app-states.md`); code 1: `App::DeliverMessage(app, 6)`, then the run loop's slot 4 (remove tick handler, key 8) and slot 3 (remove listener, key 9). Not seen in play (probably the shut-down's). |
| `sub_82106548` | `App::StartUp` | | r3 = app + 16 (the app's interface, vtable `0x82000E38`), from `App::App` once at boot. In order: `GlobalMemoryStatus` into a frame buffer it never reads (`sub_82294DA0`, Dropped, `functions.md`); `ReadError::SetApp`, `sub_8209AC30`, `sub_8209F580` and `SceneTarget::Create` with r3 (the globals `0x82600344`, `0x8266D084`, `0x826BABA0`, `0x8266D088`); `Archive::Init(its slot +16's answer, 0xC80000)` (the slot is `sub_820944A8`: the framework's first worker pool; `files-and-saves.md`); `ActionMappers::Init(r3, 1)` (`action-mappers.md`); `CameraPasses::Init` (`cameras.md`); `ScreenFades::Init` (`mode-tick.md`); `Movies::Set(r3)` (`movies.md`); `Billboards::Init`; `SoundGlobals::Init(its slot +48's answer)` (`sub_82379338`: app + 124, the sound manager; `car-sounds.md`); `SaveInterface::Init(r3)` (`save-interface.md`); the network manager's set-up `sub_8214E190(r3)` (`system-services.md`: the sign-in helper, the network start-up, the voice engine); `Text::Init` (`strings-and-font.md`); `sub_8225FB70`; `Players::ResetControllers` (`save-data.md`); `Archive::WaitIdle` (`archive.md`); `GlobalMemoryStatus` again. 78 lines; its group (29 functions, 2,213 lines with the voice set-up) waits on the voice engine's set-up (the intro). |
| `sub_820944A8` | | | Slot +16 of the app's interface at app + 16 (vtable `0x82000E38`): this − 8 (app + 8), then `sub_82094368`. From `App::StartUp` (its answer is `Archive::Init`'s r3). Leaf through a vtable call. |
| `sub_82094368` | | | Slot +4 of the app's +8 interface (vtable `0x82000E80`): the word at this + 16 (app + 24, the framework's +36 interface, vtable `0x82000AFC`), then its slot 4 (`sub_820908E8`, `framework.md`: the first worker pool, framework + 68), a tail call. From `sub_820944A8`. |
| `sub_82379350` | | | Returns the word at r3 + 112. Leaf; slot +44 of the app's interface at app + 16 (so app + 128, the movie), probably folded with other classes' getters like `sub_82379338`. From `Movies::Set` (`movies.md`), whose plain C++ reads +112 when the slot is this one. Read 2026-10-30. |
| `sub_82379338` | | | Returns the word at r3 + 108. Leaf; slot +48 of the app's interface at app + 16 (so app + 124, the sound manager `AudioManager::Create` made; `SoundGlobals::Init` calls it the music player), probably folded with other classes' getters (its address is far from the app's). From `App::StartUp`. |
| `sub_8209AC50` | `ReadError::SetApp` (probably) | Overridden | r3 = app + 16: stored at `0x82600344`, the +4 of the object at `0x82600340` (vtable `0x82001028`) that `ReportReadError` (`sub_8209AC78`, `files-and-saves.md`) uses. Leaf. Once at boot, from `App::StartUp`. Ours (`src/app_start_up/start_up.cpp`, `--native_app_start_up`): the same store. Harness `app_start_up`: 8 cases, 0 differ. |
| `sub_8209AC30` | | Overridden | r3 = app + 16: stored at `0x8266D084`. Leaf. Once at boot, from `App::StartUp`. No reader found by its offset. Ours (`src/app_start_up/start_up.cpp`, `--native_app_start_up`): the same store. Harness `app_start_up`: 8 cases, 0 differ. |
| `sub_8209F580` | | Overridden | r3 = app + 16: stored at `0x826BABA0` (the object the billboards, camera passes, screen fades and UI movies reach the command list and render queue through: its slot 8, `sub_820944D8`). Leaf. Once at boot, from `App::StartUp`. Ours (`src/app_start_up/start_up.cpp`, `--native_app_start_up`): the same store. Harness `app_start_up`: 8 cases, 0 differ. |
| `sub_8209B378` | `SceneTarget::Create` (probably) | | r3 = app + 16: stored at `0x8266D088` (the video output's way in: slot 7 then slot 3, `video-output.md`); a 32-byte object (`operator new` `sub_825A6880`) built by `SceneTarget::SceneTarget` into `0x8266D08C`, or 0 there when the allocation fails. Once at boot, from `App::StartUp`. Its group (3 functions, 238 lines) is ready. |
| `sub_8209B070` | `SceneTarget::SceneTarget` (probably) | | r3 = the 32-byte object: +16 = 1024, +20 = 576, +28 = 0 (the mode `Modes::SetSceneTargetMode` sets, `mode-tick.md`). With the device locked (`[0x8266D088]`'s slot 7 then slot 3, the video output; `VideoOutput::LockDevice`/`UnlockDevice`, `video-output.md`): +4 = `Device::CreateSurface(1024, 576, 0x1A2201BF, 1, {0, 0, 0})`; its EDRAM tiles (`Surface::EdramTiles(1024, 576, 0x182801B6, 1)`) as the base of +0 = `CreateSurface(1024, 576, 0x1A220197, 1, {tiles, 0, 0})`; +24 = `Device::CreateTexture(1280, 720, 1, 1, 0, 0x182801B6, 0, 3)`; then the largest tile of the display's tiling block (`Tiling::LargestTile([0x8260034C])`: 1280 × 256 in play) aligned by its multisample type (`[0x8260034C]` + 8: 0 width to 80 and height to 16, 1 width to 80 and height to 8, 2 width to 40 and height to 8; then both to 32) for +12 = `CreateSurface(w, h, 0x1A2201BF, type, {0, 0, 0})` and, over its tiles, +8 = `CreateSurface(w, h, 0x1A220197, type, {tiles, 0, 0})`. Returns the object. From `SceneTarget::Create` (once at boot). Graphics resources (`graphics-resources.md`'s lead): the calls are ours. |
| `sub_8209ADD8` | `Tiling::LargestTile` (probably) | | r3 = a tiling block (`[0x8260034C]`, `0x82600350`: +0 1280, +4 720, +8 the multisample type (2), +12 the tile count (3), +16 the tiles, 16 bytes each: left, top, right, bottom), r4 = two words out: the largest width (right − left) and height (bottom − top) of the tiles (0, 0 with none). Leaf. From `SceneTarget::SceneTarget`. |
| `sub_8209BE88` | `Billboards::Init` (probably) | | No arguments; once at boot from `App::StartUp`. The billboards' ring flipped inline (`[0x826A5098]` = 1 when it was 0, else 0; `[0x826A509C]` = 0, as `Billboards::FlipRing`); the vertex shader's source (`[0x82600A54]`: `matWVP` × position, color and UV passed on) compiled (`D3DXCompileShader`, entry `main`, `vs_3_0`, the stored shaders) and made under the device lock (`VertexShader::Create`, `shader-objects.md`) into `0x826A50A0`, its buffers released (slot 2, the error buffer only on a failed compile); the pixel shader's (`[0x82600A60]`: the texture × the diffuse color, `ps_3_0`) into `0x826A50A4` (`PixelShader::Create`); the vertex declaration (`CreateVertexDeclaration(0x82600A64)`: a float4 position, a D3DCOLOR at +16 and a float2 texture coordinate at +20, 28 bytes a vertex) into `0x826A50A8` under the lock; `[0x8266D090]` = 0.0 (`0x82000A34`). The ring and the shaders are what `Billboards::RecordState` and `BuildVertices` use (`sprites.md`). Ready once `CreateVertexDeclaration` is ours. |
| `sub_822A8980` | `CreateVertexDeclaration` (probably) | Overridden | Probably CreateVertexDeclaration: r3 the elements (`8264D550` from `sub_82482B18`); allocates (`sub_822976F8`), clears and copies (`memset`, `sub_825A7410`). Not read in detail. Read 2026-10-30 (B7 loop, the app's start-up): the 12-byte elements are counted up to the one whose stream (halfword +0) is 255, the highest stream used kept and a 16-byte map set to 255 for each stream used; `XMemAlloc(56 + 12 × count, 0x24800000)`, cleared for 56 bytes: +0 = `0x00100005`, +4 = 1, +20 = `0xFFFF0000`, +24 the count, +28 the highest stream, +32 to +47 the stream map, +48 = 0, the elements (without the end) copied to +52; returns it, or 0 when the allocation fails. Leaf over ours (`XMemAlloc`, `memset`, `memcpy`). Callers seen in play: `Billboards::Init`, `Font::CreateShaders` (`sub_821088B8`, `strings-and-font.md`), `ScreenObject::Init` (`sub_82132CE0`, `screen-objects.md`), `PostEffects::InitBoot` (`sub_82138C38`, `post-effects.md`), `PrimitiveDrawer::Init` (`sub_825A1438`, `model-animation.md`), `sub_82589E78`, `sub_82591858`; also `sub_82482B18` (`functions.md`, not seen in play). Moved from `functions.md`. Ours (`src/app_start_up/start_up.cpp`, `--native_app_start_up`, with `--native_heap`): the same block through `XMemAlloc`'s plain C++, the elements copied with `c_runtime::Memcpy` (memcpy through the guest where it refuses); a stream above 15 (the original's map store lands on its stack past the map, never in play) takes the original. Harness `app_start_up`: 24 cases (0 to 12 elements, one with a stream above 15), 0 differ. |
| `sub_8225FB70` | | Overridden | No arguments: `0x82816110` and `0x82816114` = 0. Leaf. Once at boot, from `App::StartUp`. Not read further (their readers not looked for). Ours (`src/app_start_up/start_up.cpp`, `--native_app_start_up`): the same stores. Harness `app_start_up`: 8 cases, 0 differ. |
| `sub_82578C78` | `XHVCreateEngine` (probably, the wrapper) | Dropped | r3 = the parameters, r4, r5 = the engine out: `sub_82578BF8(r3, r4, 0, r5)` (`system-services.md`), a tail call. From the network manager's `sub_8214F6E8` once at boot, with the 48-byte parameters on its stack: +0 8 (probably the remote talkers), +4 4 (the local talkers), +8 a list of two words (`[0x8206752C]`, `[0x82067528]`, probably the local processing modes: loopback and voice chat), +12 2, +16 a list of one (`[0x82067528]`, the remote mode), +20 1, the rest 0; r4 0; the engine to the network manager's +0. Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_82579F88` | `Voice::CreateVoices` (probably) | Dropped | r3 = the voice engine, r4 = the parameters: the engine's XAudio voices. A submix voice (`XAudioCreateSubmixVoice`, `xaudio.md`) into engine + 56 (its description at 48,000 Hz with a 4-entry map at the level `[0x82000A34]`, 0.0, and its outputs the parameters' +44 list when there is one) and, when that worked, a source voice (`XAudioCreateSourceVoice`) into engine + 64 (48,000 Hz, a 4-entry map at `[0x82000A30]`, 1.0, the callbacks at `0x82579F78`, the context the engine, output to the submix). The descriptions are built on its stack with `XMemSet`; their fields not read in detail. From `Voice::Init` (once at boot). The voices' functions are native already; nothing plays through them in play (no talker sends data). Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_8257C858` | `PacketPool::Create` (probably) | Dropped | r3 = a packet's size, r4 = an entry's size, r5 = the count, r6 = a header offset, r7 = out: a 12-byte pool (`XMemAlloc(12, 0x618A8006)`): +8 the entry size, +0 a buffer of size × count, +4 the entries (entry size × count), each entry's +8 its packet (the buffer + i × size + r6) and +12 size − r6; `E_OUTOFMEMORY` (`0x8007000E`) and `sub_8257C710` on a failed allocation, the pool (or 0) to r7. From the local talker set-up (`sub_8257E568`, five a talker) and `sub_825791B0`, `sub_82579A88`, `sub_8257D518` (not seen in play). Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_8257C710` | | | `PacketPool::Create`'s clean-up: frees its three blocks (`XMemFree`). Not read in detail. Not seen in play. |
| `sub_8257C488` | | Dropped | Probably makes one of a talker's processing objects: r3 = the engine, r4 = a description (+16 the object's size, +0 a set-up function), r5 = the node count, r6 = two more counts, r7 = out. The object (`XMemAlloc(size, 0x648A0008)`): +0 r3, +4, +8, +12 node lists (`sub_8257C798`: r5 nodes, then r6[0] and r6[1] when nonzero), +24 the description; then the description's +0 with the object (1 without one); on failure `sub_8257C3E0` and 0 out. From the local talker set-up (`sub_8257E568`) and `RemoteTalker::Create` (10 nodes). Not read in detail. Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_8257C798` | `NodeList::Create` (probably) | Dropped | r3 = the count, r4 = out: a 20-byte list (`XMemAlloc(20, 0x618A8005)`) with an array of r3 8-byte nodes at +16 linked into it (+0 the head, +4 the tail, a node's +4 the next); `E_OUTOFMEMORY` and `sub_8257C6B0` on a failed allocation. From `sub_8257C488`. Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_8257C6B0` | | | `NodeList::Create`'s clean-up: frees the nodes and the list (`XMemFree`). Not read in detail. Not seen in play. |
| `sub_8257C3E0` | | | `sub_8257C488`'s clean-up: `sub_8257C2B0`, the description's release through a register, the node lists (`sub_8257C6B0`, twice) and the object freed. Not read in detail. Not seen in play. |
| `sub_82581FC0` | | Dropped | Probably a talker's processing chain: r3, r4 kept at +0 and +4, r5 = the packet count (+12), r6 = the stages (r7 of them, copied to +80, the count at +136), r8 = out. A 160-byte object (`XMemAlloc(160, 0x618A800D)`); per stage a 24-byte-entry array (24 × the count) and an 8-byte list pointing into it; then `sub_82581350` for the shared buffers; on failure `sub_82581E28`, `E_OUTOFMEMORY`, 0 out. From the local talker set-up (`sub_8257E568`). Not read in detail. Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_82581350` | | Dropped | The chain's buffers: r3 = the chain. The frame size: the largest of the stages' +24 → +24 words, made a multiple of each stage's +24 → +20 word; +8 a buffer of that × the packet count (`XMemAlloc`, `0x608A2002`), +132 a 28-byte entry a packet, +96 an 8-byte list a packet, all cleared (`XMemSet`) and each entry given its slice (+8) and the frame size (+12); `E_OUTOFMEMORY` with the three freed on a failed allocation. Skips the allocation when +8 is already set. From `sub_82581FC0`. Not read in detail. Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_82581E28` | | | `sub_82581FC0`'s clean-up: `sub_8257C2B0` and six `XMemFree`s. Not read in detail. Not seen in play. |
| `sub_8257F388` | `RemoteTalker::Create` (probably) | Dropped | r3 = the voice engine, r4 = the remote processing modes (parameters +16), r5 = their count (+20, 1), r6 and r7 the parameters' +36 and +40 (0), r8 = out (engine + 1128 + 4n). An 80-byte talker (`XMemAlloc`, `0x618A8010`; +8 the engine): each mode's set-up through its +0 (engine, talker, 1, the talker's +12 slot), its format through the made object's slot +20, a processing object per mode (`sub_8257C488`, 10 nodes; +24 on, count at +32), the voices (`RemoteTalker::SetOutputs`); on failure `sub_8257EDE8` and 0 out. From `Voice::Init`, once per remote talker: it ran in play (the coverage list), so `Voice::Init`'s loop over the parameters' +0 (8) runs; also `sub_8257F070` (the engine's +40, register a remote talker; not seen in play). Not read in detail. Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_8257EB68` | `RemoteTalker::SetOutputs` (probably) | Dropped | r3 = a remote talker, r4 and r5 = two voice lists (0 at boot), r6 = 0: releases its old XAudio voices (`XAudioVoice::Release`, three), makes a submix voice (`XAudioCreateSubmixVoice`, at 48,000 Hz, to +72; its description not read in detail) and another for the outputs, `XAudioVoice::AddRef`, and sets them through a voice's vtable. From `RemoteTalker::Create` and `sub_8257F070` (not seen in play). Not read in detail. Dropped (the user's call, 2026-10-30: the voice engine, XHV, isn't made): no longer runs, as the network manager's voice set-up (`sub_8214F6E8`, `src/hooks/voice.cpp`) leaves out XHVCreateEngine; nothing reads the engine with no headset (attract race `itc_2576.log`, the replay `itc_2577.log`). |
| `sub_8257EDE8` | | | `RemoteTalker::Create`'s clean-up: the processing objects (`sub_8257C3E0`), a call through a register, three `XAudioVoice::Release`s, `XMemFree`. Not read in detail. Not seen in play. |
| `sub_825BA380` | `CreateThread` (probably, with a processor) | Dropped | r3 not read (the security attributes), r4 = the stack size, r5 = the entry, r6 = its parameter, r7 = the flags (bit 2, suspended), r8 = a processor (-1 for none; else `1 << r8` in the flags' top byte), r9 = the thread id out: `ExCreateThread(&handle, r4, r9, the start routine at 0x825BA558, r5, r6, flags)`; on failure `SetLastErrorFromStatus` and 0, else the handle. From `Voice::Init` (the voice thread, entry `sub_8257B5D8`, Dropped) and `sub_825BA158` (the same without a processor, from the movie library's `sub_82491468`). The core's threads (`ExCreateThread`). Dropped (the user's call, 2026-10-30): its only caller in play was `Voice::Init`, which no longer runs (the voice engine isn't made, `src/hooks/voice.cpp`); the movie library's caller no longer runs either (`--play_movies_with_ffmpeg`). |
