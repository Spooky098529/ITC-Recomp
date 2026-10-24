# Known functions: the video output and the device lock

The game's video output (`VideoOutput`, at renderer + 64, vtable
`0x8206CAF8`): the object between the game's renderer
(`Renderer::Init`, `Renderer::DrawFrame`, `Renderer::Present` in
`functions.md`) and the graphics library's device, which it makes and
holds (+140). Besides the display set-up, the front buffers' resolve and
flip and the vblank wait, it holds the **device lock** (+224, a
semaphore of count 1): every thread that uses the device outside the
render thread's frame takes it first (`VideoOutput::LockDevice`, which
also marks the device as owned by the calling thread at device +10760,
and returns the device) and gives it back after
(`VideoOutput::UnlockDevice`); the render thread holds it for the whole
frame (`Renderer::DrawFrame`, the device kept at renderer + 384 while
held). The earlier maps called the pair "the device's state saved and
restored" (`cameras.md`, `strings-and-font.md`): it's a lock, not a
state save. The object they reach it through, `[0x8266D088]`'s vtable
slot 7 then slot 3, is probably the app's renderer and then its video
output (`0x8266D088` stored by `sub_8209B378`, from the app's set-up
`sub_82106548`; the slots not read). Under the lock, this file has the
game's sync primitives it reaches (the semaphore, the event, the
critical section and scoped lock wrappers, the waits): the core's, as
`../rexglue-removal.md` puts threads and sync ("Shared sync objects"),
all of them on kernel imports (`RtlInitializeCriticalSection`,
`RtlEnterCriticalSection`, `RtlLeaveCriticalSection`,
`NtWaitForSingleObjectEx`, `NtSetEvent`, `NtCreateEvent` through
`CreateEvent`). How the frame ends is in `../gpu-functions.md` ("The end
of a frame"). Mapped by the B7 loop (`../loops/b7-loop.md`) from its lead
in `../loops/leads.md` (the graphics device's resource calls under the
camera passes), 2026-10-24.

The lock's callers seen in play are in other subsystems: the camera
passes (`CameraPasses::MakeRenderTarget`, `CameraPass::SetUpSurfaces`
and its type 0-2 chunk `sub_820A2F94`, `cameras.md`), the font
(`Font::Font`, `strings-and-font.md`), `Renderer::DrawFrame`
(`functions.md`), the scene's constructor `sub_82098A98` and
`sub_820AE5D0` (leads), the resource makers `sub_8209B070`,
`sub_82132CE0` and `sub_82138A18` (a lead), and `sub_820962B8`,
`sub_8209BE88`, `sub_820AE528`, `sub_8212F0B8`, `sub_82133C60`,
`sub_82138C38` (a lead added by this map). The device's thread word is
also taken and given back by the movie player's thread (`sub_82492E00`,
the boot movie's lead).

Graphics: what our Vulkan renderer needs stays native, the rest is
Dropped (the user's rule). The rows say which kind each looks like; none
is decided here. Kept-kind: the lock itself (it orders the main thread's
resource making and releasing against the render thread's frame, whose
replay our renderer runs and whose textures and buffers it reads from
guest memory), the front buffers' flip and the Swap (the renderer
presents there, `src/hooks/graphics_device.cpp`), the unbinding at the
frame's end (the references it drops are the game's). Dropped-kind,
probably: the device's thread word (+10760: written by the lock, read in
play only by `Resource::Lock`'s packet path, which its native code leaves
out, `graphics-resources.md`; its other readers in the library never
run), the GPU register split packets (`sub_822A8A60`: nothing reads the
library's packets since the emulated GPU went, "the library's remaining
packet writers" in `../native-renderer.md`), and the vblank count the
scene's draw clears (read only by the original `WaitForVblank`). The sync
primitives aren't graphics: making them native replaces kernel imports
(the user's call, as for `Edram::Alloc`'s critical section in
`graphics-resources.md`).

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
| `sub_8258DB18` | `VideoOutput::VideoOutput` | | r3 = the video output (renderer + 64), once at boot from `Renderer::Init` (`82092358`). +0 the vtable `0x8206CAF8`; the device lock (+224) made (`Semaphore::Init`); +140 (the device), +144, +148, +268 (the front buffer index), +272 and +276 (the front buffers), +280, +284, +288, +296 and +300 (the display's width and height) = 0; bytes +264, +265, +266, +292 = 0 and +267 = 255; the vblank count `0x8296BD30` = 0. Kept-kind: the fields are the game's (the display's size is read by the camera passes and `Viewport::Init`, `cameras.md`). Blocked by `Semaphore::Init` (kernel imports). |
| `sub_8258D568` | `VideoOutput::SetUpDisplay` | Overridden | `src/hooks/video_mode.cpp`, without `XGetVideoMode` (its answer went to +152..+199 and was never read: shown by overwriting it; left zeroed). Once at boot, from `Renderer::Init` (`820923F0`), r3 = the video output (`Renderer + 64`), r6 = the vblank interval (2; +304 = 1 for 0 or 1, 2, 3 for 4, 0 for `0x80000000`). Zeroes and fills the presentation parameters at +16 (124 bytes: 1280x720, render target format `0x18280186`, a depth surface `0x1A220197`, +60 = 1 for no library front buffer), two physical blocks (`sub_82296550`: 64 KB to +96, 1 MB to +104, with their sizes), makes the device (`sub_8229DB38`, to +140), +296/+300 = size, two front buffers (`sub_822A5D08`, format `0x28280106`, to +272 and +276), Z function 6, the viewport (0, 0, size, Z 0-1), the four render targets' format switches on, `sub_822A1EE8`/`sub_822A1F20`/`sub_822A1F58` with 0, the vblank callback (`sub_8229DA20(device, sub_8258D4D8)`), and the safe area (5%-95% of the size: +208 x, +212 y, +216 width, +220 height; 64, 36, 1152, 648). Returns the device, or 0. |
| `sub_8258D8C8` | `VideoOutput::LockDevice` | | r3 = the video output: waits for the device lock (`Semaphore::Wait` on +224), marks the device (+140) as the calling thread's (`Device::AcquireThreadOwnership`) and returns the device. Its callers keep the device in a local and pass that local's address to `VideoOutput::UnlockDevice`. Called by the render thread once a frame (`Renderer::DrawFrame`, holding it for the frame) and by the main thread around resource making and releasing (19 callers, 15 seen in play: the camera passes' render targets and surfaces, the font's shaders, the scene's constructor, the race's passes, the boot movie's end `sub_820AE528`, whose wait for the render thread is the boot stall with `--skip_intro`, "No GPU plugin" in `../native-renderer.md`). The earlier maps called it "the device's state saved". Kept-kind (the lock orders the main thread's resource calls against the render thread's frame); its thread word Dropped-kind (below). Blocked by `Semaphore::Wait` (kernel imports) and `Device::AcquireThreadOwnership` (the thread id). |
| `sub_8258D908` | `VideoOutput::UnlockDevice` | | r3 = the video output, r4 = the address of the local holding the device (`VideoOutput::LockDevice`'s answer): the device's thread word cleared (`Device::ReleaseThreadOwnership`), the local = 0, then the device lock given back (`Semaphore::Signal` on +224). Same callers as `LockDevice` (`Renderer::DrawFrame` passes renderer + 384), and `CameraPasses::MakeRenderTarget`'s case 0 (`sub_8209FC7C`). The earlier maps called it "the device's state restored". Kept-kind. Blocked by `Semaphore::Signal` (kernel imports); `ReleaseThreadOwnership` is a ready group. |
| `sub_8258D958` | | | r3 = the video output: `sub_822A8A60(device, 0, 32, 96)` (probably SetShaderGPRAllocation: 32 vertex and 96 pixel registers, written as packets into the device's command buffer, registers `0x5C8` and `0xD00`) and the vblank count `0x8296BD30` = 0. Once a frame, from `Scene::Draw` (`functions.md`). Dropped-kind, probably: nothing reads the library's packets since the emulated GPU went, and the vblank count is read only by the original `VideoOutput::WaitForVblank` (overridden). Blocked by `sub_822A8A60` (the device's state setters' lead). Not read in detail. |
| `sub_8258D998` | `VideoOutput::ResolveFrontBuffer` | Wrapped | `src/native/replay_hooks.cpp`: resolves render target 0 into a front buffer at the frame's end; the native renderer shows that target ("The end of a frame" in `gpu-functions.md`). |
| `sub_8258DAA8` | `VideoOutput::WaitForVblank` | Overridden | `src/frame_pacing/renderer_hooks.cpp`. Waited for the 2-vblank interval (on the event at `8296BD34`, with the vblank counts at `8296BD2C`/`8296BD30`); now only makes the preceding device call (`sub_822A0718`). The only reader of those counts and the only waiter on that event. |
| `sub_8258DC58` | `VideoOutput::Present` | | r3 = the video output: `sub_822A8A60(device, 0, 0, 0)` (probably the GPU register split put back to its default, packets), `VideoOutput::WaitForVblank` (overridden), then Swap (`sub_822A08A0`, overridden: the native renderer presents there) with the front buffer at +272 + 4 × (+268), and +268 flipped (0 and 1). Once a frame, from `Renderer::Present` (`functions.md`). Kept-kind (the flip and the Swap; `VideoOutput::ResolveFrontBuffer` reads +268); its packet call Dropped-kind, probably. Blocked by `sub_822A8A60` (the device's state setters' lead). |
| `sub_8258D9F8` | `VideoOutput::UnbindAll` | | r3 = the video output: every texture stage unset (`sub_822A6008(device, stage, 0, 1 << 63 >> (stage + 20))`, stages 0-15), every stream (`sub_822A3758(device, stream, 0, 0, 0, a mask)`, streams 0-3), the index buffer (`sub_822A3908(device, 0)`) and both shaders (`sub_822A8618`, `sub_822A8958` with 0, probably the pixel and vertex shader), dropping the frame's references. Once a frame after `VideoOutput::Present`, from `Renderer::Present`; also from the unseen `sub_8258DBA8` (the video output's release). Kept-kind (the references are the game's). Blocked by the device's state setters (their lead, and the primitive drawer's lead for `sub_822A8618`, `sub_822A8958`). |
| `sub_8229D9D0` | `Device::AcquireThreadOwnership` | | Probably. r3 = the device: device +10760 = the current thread's id (`sub_825B9A60`; when +10760 was already set it calls `sub_825B9A60` once more and drops the answer, probably a debug check's leftover). From `VideoOutput::LockDevice` and the movie player's thread (`sub_82492E00`); also the unseen `sub_8258DBA8` and `sub_82491A68`. Dropped-kind, probably: in play only `Resource::Lock`'s packet path read +10760 (to pick packets or the device's pending flush range), and its native code leaves that out (`graphics-resources.md`); the library's other readers (`sub_8229D8A0`, `sub_822B1CB0`) never run. Blocked by `sub_825B9A60` (the thread id, `functions.md`). |
| `sub_8229DA10` | `Device::ReleaseThreadOwnership` | | Probably. r3 = the device: an `eieio`, then device +10760 = 0. From `VideoOutput::UnlockDevice`, `Renderer::Init` (after `VideoOutput::SetUpDisplay`: the device made on the main thread let go) and the movie player's thread (`sub_82492E00`). Dropped-kind, probably (the thread word, as above). |
| `sub_825925F0` | `Semaphore::Init` | | r3 = the semaphore (40 bytes): +0 its event (`Event::Create`: auto-reset, not set), +4 a critical section (`InitializeCriticalSection`, 28 bytes), +32 the count = 1, +36 = 1 (probably the maximum; not read by `Wait` or `Signal`); returns r3. From `VideoOutput::VideoOutput` (the device lock) and the static set-ups `sub_825D9200`, `sub_825D9238`, `sub_825D9270`, `sub_825D92A8` (the archive loader's four, below). The destructor `sub_82592638` (the critical section's do-nothing `sub_82106540`, `Event::Close`) never runs. The core's sync: `NtCreateEvent` (through `CreateEvent`, `files-and-saves.md`) and `RtlInitializeCriticalSection`. |
| `sub_82592670` | `Semaphore::Wait` | | r3 = the semaphore: under its critical section (+4), while the count (+32) is 0 or less, leaves it, waits on the event (+0) with no timeout (`Event::Wait`) and enters again; then the count down by 1. From `VideoOutput::LockDevice` and the archive loader (`files-and-saves.md`: `sub_8209D490`, `sub_8209D730`, `sub_8209D7B0`, `sub_8209D950`, `sub_8209DB90`, `sub_8209DC18`, and the loader thread's override `sub_8209E118`, which calls it through a guest call); also the unseen `sub_8209DB20`, `Archive::Init` and `sub_8258DBA8`. The core's sync: `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`, `NtWaitForSingleObjectEx`. Shared between threads (the render and main threads for the device lock), so native and recompiled callers need one implementation. |
| `sub_825926E8` | `Semaphore::Signal` | | r3 = the semaphore: the count (+32) up by 1 under its critical section (+4), then its event (+0) set (`Event::Signal`). From `VideoOutput::UnlockDevice` and the archive loader (as `Semaphore::Wait`, with `sub_8209DFA8`; also the Dropped `sub_8209D808`). The core's sync: `RtlEnterCriticalSection`, `RtlLeaveCriticalSection`, `NtSetEvent`. |
| `sub_825D9200` | | | Static set-up: the archive loader's queue lock `0x826BAAE0` made (`Semaphore::Init`) and its clean-up registered (`sub_825A7068`, probably atexit, with `0x825DACC8`; the others `0x825DACD8`, `0x825DACE8`, `0x825DACF8`, not read). Once at start-up. Not read in detail. |
| `sub_825D9238` | | | Static set-up: the archive loader's semaphore `0x826BAA90` (its idle one, `kIdle` in `src/hooks/archive_streaming.cpp`) made (`Semaphore::Init`) and its clean-up registered (`sub_825A7068`). Once at start-up. Not read in detail. |
| `sub_825D9270` | | | Static set-up: the archive loader's semaphore `0x826BAAB8` (its work one, `kWork`) made (`Semaphore::Init`) and its clean-up registered (`sub_825A7068`). Once at start-up. Not read in detail. |
| `sub_825D92A8` | | | Static set-up: the archive loader's semaphore `0x826BAA68` (a request done, `kRequestDone`) made (`Semaphore::Init`) and its clean-up registered (`sub_825A7068`). Once at start-up. Not read in detail. |
| `sub_8258DCC8` | `Event::Create` | | r3 = where the event goes (a word: the handle, -1 when none): `CreateEvent(0, 0, 0, 0)` (`files-and-saves.md`: auto-reset, not set, no name); on failure the word = -1, two debug prints (`sub_825A1318`) and a store to address 0 (a deliberate crash). From `Renderer::Init` (the renderer's events at +388 and +392), `Semaphore::Init`, the vblank event's static set-up `sub_825DA470`, the worker thread `sub_82095AF0` and `sub_82094EE0`, `sub_82094F30`. |
| `sub_8258DD90` | `Event::Wait` | | r3 = event, r4 = pointer to timeout (-1 = forever): 0 when the event word is -1 (none), else `WaitForSingleObject(handle, timeout)` and 1 when it answered 0 (set), 0 otherwise (a timeout or failure). From `Semaphore::Wait`, `Renderer::WaitForFrame`, the worker threads `sub_820956F0`, `sub_82095AF0`. The core's sync (`NtWaitForSingleObjectEx`). |
| `sub_8258DD78` | `Event::Signal` | | r3 = event: nothing when the event word is -1, else `SetEvent` (`sub_825BA1E0`). From `Semaphore::Signal`, `sub_82091A58`, the worker threads `sub_82095A28`, `sub_82095AF0`. The core's sync (`NtSetEvent`). |
| `sub_822970A0` | `WaitForSingleObject` | | r3 = handle, r4 = timeout in milliseconds (-1 forever): `WaitForSingleObjectEx(handle, timeout, 0)`. From `Event::Wait`, `XGetOverlappedResult`, `GetOverlappedResult` (`files-and-saves.md`), `Voice::Init` (`system-services.md`), the movie player's `sub_82491758`; also unseen callers in the library. The core's sync (`NtWaitForSingleObjectEx`). |
| `sub_82297D58` | `WaitForSingleObjectEx` | | r3 = handle, r4 = timeout in milliseconds, r5 = alertable: `NtWaitForSingleObjectEx(handle, 1, alertable, the timeout from sub_82297D38)`, again while alertable and it answers 257 (`STATUS_ALERTED`); a negative status sets the last error (`SetLastErrorFromStatus`, `heap.md`) and returns -1, else the status (0 set, 258 a timeout). Only from `WaitForSingleObject`. The core's sync. |
| `sub_82297D38` | | | The timeout for the kernel's waits: r3 = where the 64-bit time goes, r4 = milliseconds: returns 0 (no timeout pointer: forever) for -1, else stores -10,000 × r4 (relative, in 100 ns units) and returns r3. Leaf. Only from `WaitForSingleObjectEx` (named in `src/frame_pacing/renderer_hooks.cpp`'s comments only). |
| `sub_825BA1E0` | `SetEvent` | | r3 = handle: `NtSetEvent(handle, 0)`; 1, or 0 with the last error set (`SetLastErrorFromStatus`). From `Event::Signal` and the voice thread's override (`src/hooks/voice.cpp`, through a guest call); also the Dropped `sub_8257B5D8`. The core's sync. |
| `sub_8258E548` | `InitializeCriticalSection` | | r3 = the critical section: `RtlInitializeCriticalSection`, returning r3. From `Semaphore::Init`, the worker threads' set-up (`sub_82094EE0`, `sub_82094F30`), `sub_82593DC0` and static set-ups (`sub_825DA178`, `sub_825DA438`, `sub_825DA4A8`). The core's sync. |
| `sub_825A6D28` | `EnterCriticalSection` | | A jump to `RtlEnterCriticalSection` (r3 the critical section). From `Semaphore::Wait`, `Semaphore::Signal`, `ScopedLock::ScopedLock` and the worker threads (`sub_820956F0`, `sub_82095A28`, `sub_82095AF0`); others unseen. The core's sync. |
| `sub_8258E578` | `LeaveCriticalSection` | | A jump to `RtlLeaveCriticalSection` (r3 the critical section). The same callers as `EnterCriticalSection`, with `ScopedLock::~ScopedLock`. The core's sync. |
| `sub_8258E580` | `ScopedLock::ScopedLock` | | Probably. r3 = the lock (a word on the caller's stack), r4 = a critical section: +0 = r4, then `EnterCriticalSection(r4)`; returns r3. From `sub_8258DFF0` (the critical section `0x8296BD38`), `sub_8258E098`, `sub_8258E138` (`models.md`), `sub_8258E1D8`, `sub_8258E410`, `sub_8258E498` and `sub_82593E78`, each with `ScopedLock::~ScopedLock`. The core's sync. |
| `sub_8258E5B8` | `ScopedLock::~ScopedLock` | | Probably. r3 = the lock: `LeaveCriticalSection(+0)`. The same callers as `ScopedLock::ScopedLock`. The core's sync. |
