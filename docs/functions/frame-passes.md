# Known functions: the frame passes

The **frame passes** object: 44 bytes the framework makes once at boot
and keeps at its +108 (`Framework::Init`, [`framework.md`](framework.md);
its constructor `FramePasses::FramePasses` given the run loop and the
framework's +52 interface, whose slots 1 to 3 give the first worker pool
(framework + 68), the renderer (+76) and the scene (+80)). Its vtable
`0x82000F20` (read from the image, `itc/xex_image/default.bin`): 0
`FramePasses::Boot`, 1 `FramePasses::Record`, 2 `FramePasses::FlipWorkMem`,
3 `FramePasses::OnEvent`, 4 the deleting destructor, 5
`FramePasses::RecordConstants`, 6 `FramePasses::RecordSamplers`. At
boot the constructor runs slot 0 on the first worker pool as a job and
waits for it (`Job::Wait`): the post effects' boot set-up under the
device lock. It registers slot 1 as tick handler 3, slot 2 as tick
handler 20 and slot 3 as the listener for event 8, each through a
member delegate (vtable `0x82002850`) whose target is one of the
framework's slot thunks (`sub_82096340`, `sub_82096350`,
`sub_82096320`). Every pass, `FramePasses::Record` records the frame's
shared pass chunks on the scene's command list and adds them to the
render queue at 58 buckets (the passes' state resets: targets put back
and cleared, default render and sampler states, the shared shader pair,
the viewport, depth bias and color write masks), sets the render
queue's sort keys `0x8296B90C`, `0x8296B900` and `0x8296B908` to 168,
270 and 163, then records the scene target's passes
(`SceneTarget::RecordPasses`, [`scene-target.md`](scene-target.md),
buckets 150, 602, 121) and the primitive reset states at bucket 0;
`FramePasses::FlipWorkMem` swaps the graphics library's two work memory
buffers (`WorkMem::AllocVertexBuffer` in [`car-draw.md`](car-draw.md))
each pass. Mapped by the B7 loop (`../loops/b7-loop.md`) from its lead
in `../loops/leads.md` (2026-11-05).

They are command list recorders: the list and its allocators
(`CommandList::OpenChunk`, `AllocateSlots`, `AllocateBytes`,
`CloseChunk`) are in [`command-list.md`](command-list.md), the render
queue (`RenderQueue::Add`, the queue's slot 4: r4 the chunk, r5 the
bucket, f1 the key, 0.0 from `0x82000A34` everywhere here) in
[`device-shaders.md`](device-shaders.md), the commands and their items
in `../gpu-functions.md` ("Command table", "Render states and sampler
states"). Every command recorded here is replayed and read by our
renderer: 1 and 2 (render and sampler states: alpha blend, alpha test,
reference and function, Z enable, write and function, fill mode, the
per-target high-precision blend 308-320, depth bias 204 and 208, color
write mask 212, 324, primitive reset 328 and 332; samplers 0-3 and 6
address modes, filters and LOD bias), 7 (vertex constant c86 and pixel
constant c76), 8 and 9 (the shared shader pair 0, `sub_825896D8` and
`sub_82589700` in [`models.md`](models.md)), 23 (render targets 0-3
unbound), 26 (Clear: flags 49, color `0xFF000000`, Z 0.0, stencil 0),
29 (targets and depth back from the saved set, mask `0x8F`), 30 (the
viewport 0, 0, 1280 × 720, Z 0.0 to 1.0), 34 and 35 (the viewport and
targets saved and restored) and 36 (a chain's end); each chunk starts
with a kind-5 head (halfword +2 = 5). So by the graphics call ("native
if the renderer needs it, else Dropped", `../loops/b7-loop.md`'s
"Precedent first") nothing here is Dropped; EDRAM isn't emulated, but
these are the states and binds the renderer works from. The boot job's
device lock is the host semaphore's (`VideoOutput::LockDevice` and
`UnlockDevice`, [`video-output.md`](video-output.md); the user's call,
2026-10-24). No frame interpolation hook is on these functions;
`Job::Wait`'s menu scan wrap (`ITC_MENU_SCAN_WRAP`,
`src/debug/menu_selection_scan.cpp`) is a diagnostic kept around a
native body. Shared, in other files: `FramePasses::Boot`'s
`PostEffects::InitBoot` (`post-effects.md`), `Record`'s
`SceneTarget::RecordPasses` (`scene-target.md`), the delegate slot
thunks `sub_82096330` and `sub_82096340` (`framework.md`), the run
loop's `AddTickHandler` and `AddListener` (`framework.md`), `operator
delete` (`heap.md`). Not read: the scene's slots 1 and 2 (its command
list and render queue, as the render manager's in `mode-tick.md`), the
worker pool's post (its slot 1; the pools are a lead in
`../loops/leads.md`) and the object `Job::Wait` calls through the job's
+20. Fields: `../known-data.md` ("frame passes", "work memory",
`0x8296B8FC`-`0x8296B90C`). The rest of the game's functions are in
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
| `sub_82098630` | `FramePasses::FramePasses` | | r3 = it (44 bytes from `Framework::Init`, kept at framework + 108), r4 = the block (+0 the run loop, +4 the framework's +52 interface). +0 = vtable `0x82000F20`; +4 = the interface; at +8 a 36-byte job as `DebugText`'s (`framework.md`): +8 `0x820066DC`, +12 `0x82000AB4` then `0x82002908`, +16 and +20 = 0, the bytes +24 = 0, +25 = 1, +26 = 0, +28 a word read from its own frame at r1 + 164, never set (an uninitialized local), +32 a member delegate (vtable `0x82002850`, +36 this, +40 `sub_82096330`: slot 0, `FramePasses::Boot`); the interface's slot 1 (the first worker pool, framework + 68), that pool's slot 1 with r4 = the job and r5 = 1 (probably the post), then `Job::Wait` on the job. Then three member delegates built on its stack (vtable `0x82002850`, this, a target; each slot reset to `0x82000ABC`, the functors' base, after its use): tick handler 3 (`sub_82096340`, slot 1, `FramePasses::Record`) and tick handler 20 (`sub_82096350`, slot 2, `FramePasses::FlipWorkMem`) through the run loop's slot 2 (`RunLoop::AddTickHandler`), and the listener for event 8 (`sub_82096320`, slot 3, `FramePasses::OnEvent`) through its slot 1 (`RunLoop::AddListener`). Returns r3. 123 lines, 5 indirect calls. Once at boot. Read in full 2026-11-05. |
| `sub_820962B8` | `FramePasses::Boot` | Overridden | Slot 0, run once at boot as the constructor's job on the first worker pool. r3 = it: the +4 interface's slot 2 (the renderer, framework + 76), that object's slot 3 (probably the video output, as `PostEffects::InitBoot`'s own path), `VideoOutput::LockDevice` on it (`video-output.md`) into a local, `PostEffects::InitBoot` with the device (`post-effects.md`), `VideoOutput::UnlockDevice` with the local's address. 36 lines, 2 indirect calls. Also in the device lock callers' lead in `../loops/leads.md`. Read in full 2026-11-05. Plain C++ `frame_passes::Boot` (`src/frame_passes/frame_passes.cpp`, with `VideoOutput::LockDevice` and `UnlockDevice`'s plain C++) behind an adapter (`src/hooks/frame_passes.cpp`, its work `hooks::FramePassesBoot`: the interface's and the renderer's getters guest calls from a 112-byte frame with the original's return addresses, `PostEffects::InitBoot` through `hooks::PostEffectsInitBoot`, its adapter's work; r3 the video output, as the native `UnlockDevice` leaves it; the original without `--native_video_output`) (`--native_frame_passes`; harness `frame_passes`: `FramePasses::Boot (sub_820962B8)`, the game's video output and device lock, effect 14 there); ran at boot in the attract race (2026-11-05, B7 loop). |
| `sub_82096418` | `FramePasses::Record` | | Slot 1, tick handler 3 (through `sub_82096340`), every pass. r3 = it: the +4 interface's slot 3 (the scene, framework + 80), the scene's slot 1 (its command list) and slot 2 (its render queue). Sets the render queue's sort keys `0x8296B90C` = 168, `0x8296B900` = 270, `0x8296B908` = 163. Then one `CommandList::OpenChunk`, about 20 chunks recorded one after another, each a kind-5 head (+2 = 5), its commands and a command 36, the heads kept on its stack: the first command 29 (mask `0x8F`) and a Clear (command 26 with `AllocateBytes(280)`: flags 49, color `0xFF000000`, Z 0.0 from `0x82000A34`, stencil 0); several state resets: command 1 with 9 to 12 render states (60 alpha blend, 96 alpha test, 100 alpha reference, 104 alpha function, 40 Z enable, 48 Z write, 52 fill mode, 44 Z function 6, 308-320 the high-precision blend per target; values differ by chunk), command 2 with the sampler states of samplers 0-3 (28 LOD bias from `[0x8296B8F4]`, 20 and 16 the filters, 24 the mip filter) and of sampler 6 (filters, address U and V 2), and its slot 5 or 6 recorded in (`FramePasses::RecordConstants`, `RecordSamplers`; 6 calls); chunks with 34 (save), 23 (render targets 0-3 = 0), 8 and 9 (the shared shader pair 0, `sub_825896D8(0)` and `sub_82589700(0)`, `models.md`), 35 (restore), 30 (viewport 0, 0, 1280 × 720, Z 0.0 to 1.0 from `0x82000A34` and `0x82000A30`); render states 204 (slope-scaled depth bias, -8.0 from `0x82000F18`) and 208 (depth bias, -0.0001 from `0x82000F1C`); and one-state chunks of 212 (color write mask 1, 2, 4, 8, 15) and 40. `CommandList::CloseChunk`, then the chunks added to the render queue (its slot 4, `RenderQueue::Add`, f1 = 0.0) at 58 buckets from 4 to 603 (one chunk at 15: 50 to 98 by 6, 151, 161, 167, 372, 382, 388). Last, `SceneTarget::RecordPasses` (`[0x8266D08C]`, the list, 150, 602, 121; `scene-target.md`) and `FramePasses::RecordPrimitiveReset`. 2,658 lines with `sub_82589990` (over 1,500: the migration split), 68 indirect calls. Read: its structure, the commands and the buckets; which chunk goes to which bucket not read in detail. |
| `sub_82096360` | `FramePasses::RecordConstants` | Overridden | Slot 5 (no direct callers; from `FramePasses::Record`, through this's +20). r3 = it (not read), r4 = the command list: four slots (`CommandList::AllocateSlots`, 1 each): command 7 (+4 = 1, +8 = `0x00015601`: vertex stage, float constants, start 86, count 1) and its 16 bytes copied from `0x8265ED20`; command 7 (`0x01014C01`: pixel stage, float, start 76, count 1) and 16 bytes from `0x8265ED30` (both zero in the image; written at run time by others, not read). Leaf but for `AllocateSlots`. 57 lines. Read in full 2026-11-05. Plain C++ `frame_passes::RecordConstants` (`src/frame_passes/frame_passes.cpp`) behind an adapter (`src/hooks/frame_passes.cpp`; r3 the last slot, r4 = 1 as the original leaves it; the original where the list hasn't room, or with the command list's diagnostics on) (`--native_frame_passes`; harness `frame_passes`: `FramePasses::RecordConstants (sub_82096360)`); ran in the attract race (2026-11-05, B7 loop). |
| `sub_82096030` | `FramePasses::RecordSamplers` | Overridden | Slot 6 (no direct callers; from `FramePasses::Record`, through this's +24). r3 = it (not read), r4 = the command list: command 2 (+4 = 8) with samplers 0-3's address U (state 0) and V (4) = 2; command 2 (+4 = 12) with samplers 0-3's min (20) and mag (16) filters = 0 and mip filter (24) = 2; command 1 (+4 = 1) with render state 324 = 0. 24 slots (1 + 8 + 1 + 12 + 2; the map said 15) (`CommandList::AllocateSlots`, 1 each). 146 lines. Read in full 2026-11-05. Plain C++ `frame_passes::RecordSamplers` (`src/frame_passes/frame_passes.cpp`) behind an adapter (`src/hooks/frame_passes.cpp`; r3 the last slot, r4 = 1; the original where the list hasn't room, or with the command list's diagnostics on) (`--native_frame_passes`; harness `frame_passes`: `FramePasses::RecordSamplers (sub_82096030)`); ran in the attract race (2026-11-05, B7 loop). |
| `sub_82589990` | `FramePasses::RecordPrimitiveReset` (probably) | Overridden | Only caller `FramePasses::Record`, last, every pass. No arguments: on the command list `[0x8296B918]` a chunk (`OpenChunk`, a kind-5 head, command 1 with 2 items: render state 328, primitive reset enable, = 1 and 332, the reset index, = `0xFFFF`; command 36; `CloseChunk`), added to the render queue `[0x8296B91C]` (its slot 4) at bucket 0 with f1 = 0.0 (`0x82000A34`). In `FramePasses::Record`'s group. Read in full 2026-11-05. Plain C++ `frame_passes::RecordPrimitiveReset` (`src/frame_passes/frame_passes.cpp`; declines where a chunk is open or the list hasn't its 5 slots) behind an adapter (`src/hooks/frame_passes.cpp`, its work `hooks::FramePassesRecordPrimitiveReset`: the render queue's Add a guest call from a 128-byte frame, return address `0x82589A6C`) (`--native_frame_passes`; harness `frame_passes`: `FramePasses::RecordPrimitiveReset (sub_82589990)`); ran in the attract race (2026-11-05, B7 loop). |
| `sub_82096218` | `FramePasses::FlipWorkMem` | Overridden | Slot 2, tick handler 20 (through `sub_82096350`), every pass. Tail call to `WorkMem::Flip` (r3 not read). Read in full 2026-11-05. Plain C++ `frame_passes::FlipWorkMem` (`src/frame_passes/frame_passes.cpp`, calling `WorkMemFlip`) behind an adapter (`src/hooks/frame_passes.cpp`) (`--native_frame_passes`; harness `frame_passes`: `FramePasses::FlipWorkMem (sub_82096218)`); ran in the attract race (2026-11-05, B7 loop). |
| `sub_82589A78` | `WorkMem::Flip` (probably) | Native | Only caller `FramePasses::FlipWorkMem`. No arguments: the work memory's current buffer `[0x8296B780]` ^= 1, then for the new one (i) `[0x8296B784 + 4i]`, the headers used `[0x8296B798 + 4i]` and the bytes used `[0x8296B7A0 + 4i]` = 0 (`WorkMem::AllocVertexBuffer`, `car-draw.md`, takes from them). Leaf. Read in full 2026-11-05. Plain C++ `frame_passes::WorkMemFlip` (`src/frame_passes/frame_passes.cpp`) with no adapter: only `FramePasses::FlipWorkMem`'s plain C++ calls it, and `--native_frame_passes` switches that (harness `frame_passes`: `WorkMem::Flip (sub_82589A78)`); ran in the attract race (2026-11-05, B7 loop). |
| `sub_82096350` | | Overridden | A delegate's target like `sub_82096330` (`framework.md`): calls r3's vtable slot 2 (tail call). `FramePasses`' tick handler 20 every pass (`FramePasses::FlipWorkMem`), and others not read; listed in `../frame-interpolation.md`'s frame structure. 8 lines. Read in full 2026-11-05. Plain C++ `frame_passes::DelegateSlot2Target` (`src/frame_passes/frame_passes.cpp`) with its target as a tail call behind an adapter (`src/hooks/frame_passes.cpp`), as `sub_82096330`'s (`--native_frame_passes`; harness `frame_passes`: `Delegate slot 2 thunk (sub_82096350)`, its objects' slot 2 small guest functions, as the framework harness's); ran in the attract race (2026-11-05, B7 loop). |
| `sub_82094B10` | `Job::Wait` (probably) | Overridden | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. r3 = a worker job (as at `FramePasses` + 8): when its byte +18 is set (posted, probably), the object at its +20's slot 1 with r4 = the job (probably the pool's wait), then +18 = 0. 29 lines, 1 indirect call. Callers: `FramePasses::FramePasses` (once at boot), `Movie::Play` (`movies.md`), `AudioManager::Update` (`sound-cues.md`); `AccessorySkeleton::WaitForStep` (`accessories.md`, not seen in play). Moved from `functions.md` (2026-11-05, B7 loop); read in full then. Plain C++ `frame_passes::JobWait` (`src/frame_passes/frame_passes.cpp`) with the object's slot 1 a guest call from a 96-byte frame (return address `0x82094B48`), in `hooks::FramePassesJobWait` (`src/hooks/frame_passes.cpp`), which `--find_menu_selection`'s wrap (`src/debug/menu_selection_scan.cpp`, the override) calls between its notes (`--native_frame_passes`; harness `frame_passes`: `Job::Wait (sub_82094B10)`, the job's object's slot 1 small guest functions); ran in the attract race (2026-11-05, B7 loop). The diagnostic's wrap kept around the native body, as the twenty-ninth run's. |
| `sub_82096220` | `FramePasses::OnEvent` | | Slot 3, the listener for event 8 (through `sub_82096320`). r3 = it, r4 = the run loop, r5 = the event's word: when it is 1, removes tick handlers 3 and 20 (the run loop's slot 4) and the listener for event 8 (its slot 3). Not seen in play (event 8 never sent). Read in full 2026-11-05. |
| `sub_82096320` | | | A delegate's target like `sub_82096330`: calls r3's vtable slot 3 (tail call). `FramePasses`' listener for event 8 (`FramePasses::OnEvent`). Not seen in play. Read in full 2026-11-05. |
| `sub_820987E8` | `FramePasses::~FramePasses` (deleting) | | Slot 4. r3 = it, r4 = flags: +0 = `0x82000F20`, the job's destructors inline (+32 `0x82000ABC`, +12 `0x82000BA8` then `0x82000AB4`, +8 `0x82000EB8`) and, with bit 0, `operator delete` (`heap.md`). Doesn't remove its handlers. Not seen in play (the framework's services are never released in play). Read in full 2026-11-05. |
