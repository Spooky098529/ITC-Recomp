# Known functions: the morph jobs

The **morph jobs**: the 224-byte job `Model::MorphShape` (`sub_82595110`,
[`car-draw.md`](car-draw.md)) has made for each per-draw morph of the
wheels (up to 18 a pass for each car drawn with morphed wheels: the
tires, the front end's rims and the brakes), so the blend
(`Morph::Blend` `sub_8258BD28`, [`second-set.md`](second-set.md)) runs
on a worker thread of the second worker pool instead of the draw's. The
second pool maker (`[0x8296B914]`, `sub_820AC848`'s object, vtable
`0x82002840`, [`framework.md`](framework.md)) makes it with its slot 1
(`sub_820AC7F0`): `operator new(224)` and the job's constructor
(`sub_820ACC08`), which copies the 128-byte morph record into the job,
builds the job's work item at +184 and posts it to the second pool (the
pool's slot 1, `sub_82095A28`). `Model::MorphShape` then puts the job on
the render queue (`Model::QueueJobRelease` `sub_82594E88`,
`car-draw.md`): during the frame's replay, command 22 (`sub_82591798`,
`functions.md`) waits for the job (its slot 1, `Job::Wait` on the work
item), pushes it onto the release list and marks it (slot 3), and the
replay deletes it after the frame's draws (slot 0 with r4 = 1;
`device-shaders.md`'s release list). The job's class also serves the
first pool maker's skinning job (`sub_820AC8A0`, `sub_820ACAA8`: the
same vtable, +220 = 0, from `Model::SkinShape` `sub_82594FD0`, never
seen in play).

The vtables, read from the image (`itc/xex_image/default.bin`; the
compiler folds vtables so they overlap): the job's `0x8200286C` (0
`sub_820ACB90` the deleting destructor, 1 `sub_820AC9B8` the wait, 2
`sub_820AC9D0` the released byte, 3 `sub_820AC9D8` setting it, 4
`sub_820AC998` the run; the word after, `sub_820ACCF0`, is the next
vtable's); its base `0x8200285C` (0 `sub_820AC9E8` the base's deleting
destructor, 1 `sub_825A7098` (probably `_purecall`: the handler at `[0x82971178]` if set, then an exit with code 25; no row), then the same 2 and 3; it
begins at the delegate vtable `0x82002850`'s fourth word, so
`app-states.md`'s "3 `sub_820AC9E8`" under `Delegate::Clone` is this
base's slot 0, not the delegate's). The work item at +184 is the 36-byte
job every worker pool job holds (`DebugText`'s, the frame passes' boot
job, `Movie`'s command, the bank pairs', the accessories' skeletons',
the archive loader's parameters): vtable `0x820066DC` at +0 (0
`sub_82090E40` the deleting destructor, 1 `sub_82090DD0` posted, 2
`sub_82090DE8` running, 3 `sub_82090DF8` done, 4 `sub_82090E08` the
skip byte, 5 `sub_82090E18` the run, 6 `sub_82090CB8` the skip byte's
read, 7 `sub_820ACA30` the delegate's call), the interface `0x82002908`
at +4 (0 `sub_82090E38`, 1 `sub_82090DC8`, thunks to slots 0 and 6;
`0x82000AB4` written there first), and a 12-byte delegate at +24
(vtable `0x82002850`: slot 1 `Delegate::Invoke` `sub_8223A3F0`,
`functions.md`). Its rows are here, as no other file has them; the
pools that call them (the post `sub_82095A28`, the worker loop
`sub_82095AF0` in `functions.md`, the pool's wait that `Job::Wait`
calls) are the worker pools' lead in `../loops/leads.md`, and
`Job::Wait` (`sub_82094B10`) is in [`frame-passes.md`](frame-passes.md).
Mapped by the B7 loop (`../loops/b7-loop.md`) from its lead in
`../loops/leads.md` (2026-11-13).

On a worker thread the chain is: the worker loop takes the item, calls
its slot 2 (running), then slot 5 (`sub_82090E18`, unless slot 6 says
it's skipped), which calls slot 7 (`sub_820ACA30`): the delegate's slot
1 (`Delegate::Invoke`: `sub_820AC9C0` with the job), which calls the
job's slot 4 (`sub_820AC998`): `Morph::Blend(job + 56)` when +220 is 1
(morph), the skinning blend `sub_8258BCD8(job + 8)` when 0 (skin; not
seen in play, 50 lines, not read); then slot 3 (done). With
`[0x82600308]` = 1 (the image's value; never written), the pool's post
calls the item's slot 1 (posted, +20 = the pool's waiter interface,
pool − 4), queues it under the pool's lock and signals its event.

`Model::MorphShape` is native (`car_draw::ModelMorphShape`,
`src/car_draw/morph.cpp`, `--native_car_draw`) and called the pool
maker's slot 1 (`sub_820AC7F0`) as a guest call from its frame
(returning to `0x825951A4`); since the morph jobs were made native
(B7 loop, 2026-11-13, `--native_morph_jobs`, `src/morph_jobs/`,
adapters in `src/hooks/morph_jobs.cpp`, harness `morph_jobs`), it calls
the maker's plain C++ instead when the slot is `sub_820AC7F0`. The work
item's slots run in attract races (every pool job: the debug font's
load at boot); the morph job's own only with morphed wheels. `sub_820AC7F0` and the job's constructor are ready, with the
pool's post (its slot 1) as a guest call from the constructor's frame,
as `DebugText::DebugText` and `FramePasses::FramePasses` post theirs
(`framework.md`, `frame-passes.md`); `operator new` and `memcpy` are
ours. The job's run reaches `Morph::Blend`'s plain C++
(`second_set::MorphBlend`); its slots are called through vtables from
recompiled code (the worker loop, `Delegate::Invoke`, command 22 and
the replay's release list), so made native they keep adapters until
those callers are ours. Shared, in other files: `operator new` and
`operator delete` (`heap.md`), `memcpy` (`c-runtime.md`), `Job::Wait`
(`frame-passes.md`), `Morph::Blend` (`second-set.md`), the pool makers
and their slot 0 `sub_820AC950` (no row; never seen in play, as
`PoolMaker`'s, `framework.md`). Fields: `../known-data.md` ("morph
job", "work item", `0x8296B910`). The rest of the game's functions are
in [`functions.md`](functions.md).

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
| `sub_820AC7F0` | `PoolMaker2::MakeMorphJob` (probably) | Overridden | The second pool maker's (`sub_820AC848`, vtable `0x82002840`, `framework.md`) slot 1. r3 = the maker, r4 = a morph record (128 bytes, set up by `Morph::SetUp` `sub_82594C18`, `second-set.md`): `operator new(224)` (`sub_825A6880`, `heap.md`); when it gives a block, `MorphJob::MorphJob(block, the maker's +4 (the second worker pool's interface), r4)` (`sub_820ACC08`) and returns the job; else returns 0. 112-byte frame (r30, r31 saved). Only caller: `Model::MorphShape` (`sub_82595110`, `car-draw.md`) through `[0x8296B914]`'s vtable, its native body making the call as a guest call from its frame (returning to `0x825951A4`), up to 18 times a pass for each car drawn with morphed wheels. Read in full (B7 loop, 2026-11-13). Seen in play; ready (`MorphJob::MorphJob` under it). Before 2026-11-01, `framework.md` had it never running. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::MakeJob`, `src/morph_jobs/morph_jobs.cpp`, harness `morph_jobs`): `operator new`'s plain C++ (with `--native_c_runtime_heap` over the native heap, else the original), the 112-byte frame's back chain, then the constructor's plain C++; the original where `operator new` declines. `Model::MorphShape`'s plain C++ calls it (`car_draw::Calls::make_morph_job`) when the maker's slot 1 is it, with the flag, instead of the guest call. |
| `sub_820ACC08` | `MorphJob::MorphJob` (probably) | Native | r3 = the job (224 bytes), r4 = a worker pool's interface, r5 = the morph record. +0 = vtable `0x8200286C`, the byte +4 (released) = 0; the work item at +184 (36 bytes): +184 `0x820066DC`, +188 `0x82000AB4` then `0x82002908`, +192 and +196 = 0, the bytes +200 = 0, +201 = 1, +202 = 0, +208 the delegate's vtable `0x82002850`, +212 and +216 = 0; then `memcpy(job + 56, r5, 128)` (`sub_825A7410`, `c-runtime.md`); then +220 = 1 (morph), +192, +196 and the bytes +200 to +202 written again (0, 1, 0), +204 (the item's +20) = the word at its own frame's r1 + 100, never set (an uninitialized local, as `DebugText::DebugText`'s and `FramePasses::FramePasses`' jobs' +20), +216 = `sub_820AC9C0`, +212 = the job; then the pool interface's slot 1 (`sub_82095A28` for the second pool, the worker pools' lead in `../loops/leads.md`) with r4 = the item (job + 184), r5 = 1: the post (with `[0x82600308]` = 1, the item's slot 1 sets its +20 to the pool's waiter, so the uninitialized word doesn't stay). Returns the job. Bytes +5 to +55 and +221 to +223 not written. 176-byte frame, `__savegprlr_27`. Only caller `sub_820AC7F0`. Read in full (B7 loop, 2026-11-13). Seen in play; ready (the post a guest call from its frame, as `DebugText::DebugText`'s, `framework.md`). Native under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::ConstructJob`, harness `morph_jobs`; no adapter: only `PoolMaker2::MakeMorphJob`, native, calls it): the stores in the original's order, the copy through `native::Buffers_Memcpy` (memcpy's original from its frame, returning to `0x820ACC94`, where it declines), the stack word read from its frame's +100 after the copy, the post (the interface's slot 1) a guest call from its 176-byte frame returning to `0x820ACCE0`; r3 left as it came (all 64 bits). |
| `sub_820ACB90` | `MorphJob::~MorphJob` (deleting, probably) | Overridden | Slot 0 of `0x8200286C`. r3 = the job, r4 = flags: the work item's destructors inline (+208 = `0x82000ABC`, the functors' base; +188 = `0x82000BA8` then `0x82000AB4`; +184 = `0x82000EB8`), +0 = `0x8200285C` (the base's vtable), and with bit 0 `operator delete` (`sub_825A70E8`, `heap.md`). Returns r3. 96-byte frame. Called through the vtable by the render queue's replay on the release list's objects (r4 = 1, `device-shaders.md`), after the frame's draws: once a job. Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::DestroyJob`, harness `morph_jobs`): the stores and the 96-byte frame's back chain, then with bit 0 `operator delete`'s plain C++ (the original from the frame, returning to `0x820ACBF0`, without the native heap or where the layer refuses the block); r3 as it came. The adapter stays: the replay's release list calls it through the vtable. |
| `sub_820AC9B8` | `MorphJob::Wait` (probably) | Overridden | Slot 1 of `0x8200286C`: `Job::Wait(r3 + 184)` (`sub_82094B10`, `frame-passes.md`, ours behind its adapter), a tail call: waits for the work item when it's posted. Called through the vtable by command 22 (`sub_82591798`, `functions.md`, Wrapped) during the replay, when the job's slot 2 gives 0 and the command's item +4 is 1. Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; harness `morph_jobs`): r3 + 184, then `Job::Wait` through the function table (its adapter, inside `--find_menu_selection`'s wrap). The adapter stays: command 22 calls it through the vtable. |
| `sub_820AC9D0` | `MorphJob::IsReleased` (probably) | Overridden | Slot 2 of `0x8200286C` (and of the base `0x8200285C`): returns the byte +4. Leaf. Called through the vtable by command 22 (`sub_82591798`), which waits and releases only when it's 0. Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::IsReleased`, harness `morph_jobs`). The adapter stays (command 22, through the vtable). |
| `sub_820AC9D8` | `MorphJob::SetReleased` (probably) | Overridden | Slot 3 of `0x8200286C` (and of the base): byte +4 = 1. Leaf. Called through the vtable by command 22 (`sub_82591798`) after pushing the job onto the release list (`sub_82591540`). Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::SetReleased`, harness `morph_jobs`; r11 = 1 as the original leaves it). The adapter stays (command 22, through the vtable). |
| `sub_820AC998` | `MorphJob::Run` (probably) | Overridden | Slot 4 of `0x8200286C`. r3 = the job: when +220 isn't 0, `Morph::Blend(job + 56)` (`sub_8258BD28`, `second-set.md`, its plain C++ `second_set::MorphBlend` with its own fallbacks), else the skinning blend `sub_8258BCD8(job + 8)` (the 48-byte record `Model::SkinShape` copies; not seen in play, not read); tail calls. Leaf otherwise. Only caller `sub_820AC9C0` (through the vtable), on a worker thread of the second pool. Read in full (B7 loop, 2026-11-13). Seen in play; ready (its unseen callee a guest call). Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::Run`, harness `morph_jobs`): r11 the +220 word, r3 + 56 (all 64 bits) and `Morph::Blend`'s register step (`hooks::SecondSetMorphBlend`, with `--native_second_set`, else its original); the skinning blend's original (r3 + 8) when +220 is 0 (not in the harness: not read). The adapter stays (`MorphJob::RunThunk`, through the vtable). |
| `sub_820AC9C0` | `MorphJob::RunThunk` (probably) | Overridden | The work item's delegate target (stored at job + 216 by both job constructors): r3 = the job, a tail call to its vtable's slot 4 (+16; `sub_820AC998`). Called by `Delegate::Invoke` (`sub_8223A3F0`, `functions.md`) from the work item's slot 7 (`sub_820ACA30`) on a worker thread. Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::RunThunk`, harness `morph_jobs`): the job's slot 4 as a tail call through the function table, r12 and r11 as the original leaves them, r3 as it came. The adapter stays (`Delegate::Invoke`, through the delegate). |
| `sub_820AC9E8` | `Job::~Job` (deleting, probably) | Overridden | Slot 0 of the job's base vtable `0x8200285C`: r3 = it, r4 = flags: +0 = `0x8200285C`, and with bit 0 `operator delete` (`heap.md`). Returns r3. `app-states.md` lists it as the delegate vtable `0x82002850`'s slot 3: the two vtables overlap, and this is the base's slot 0. Not seen in play (the jobs are deleted through `0x8200286C`'s slot 0). Read in full (B7 loop, 2026-11-13). Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::DestroyJobBase`, harness `morph_jobs`; ported though not seen in play, the user's 2026-11-12 call): the store, the frame's back chain, `operator delete` with bit 0 as `MorphJob::~MorphJob`'s (returning to `0x820ACA14`). Reached only through vtables. |
| `sub_820AC8A0` | `PoolMaker::MakeSkinJob` (probably) | Overridden | The first pool maker's (`PoolMaker::PoolMaker` `sub_820AC8F8`, vtable `0x82002848`, `framework.md`) slot 1: as `sub_820AC7F0` with `sub_820ACAA8`: r3 = the maker, r4 = the 48-byte skinning record; `operator new(224)`, then the constructor with the maker's +4 (the second worker pool's interface), or 0. Only caller `Model::SkinShape` (`sub_82594FD0`, `car-draw.md`, never seen in play) through `[0x8296B910]`. Not seen in play. Read in full (B7 loop, 2026-11-13). Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::MakeJob`, harness `morph_jobs`; ported though not seen in play, the user's 2026-11-12 call): as `MakeMorphJob`'s. Its caller, `Model::SkinShape`, is recompiled. |
| `sub_820ACAA8` | `SkinJob::SkinJob` (probably) | Native | As `MorphJob::MorphJob` (`sub_820ACC08`, the same vtable `0x8200286C`, work item and post), but `memcpy(job + 8, r5, 48)` and +220 = 0 (so `MorphJob::Run` takes the skinning blend `sub_8258BCD8(job + 8)`). Only caller `sub_820AC8A0`. Not seen in play. Read in full (B7 loop, 2026-11-13). Native under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::ConstructJob`, harness `morph_jobs`; ported though not seen in play; no adapter: only `PoolMaker::MakeSkinJob`'s plain C++ calls it): as `MorphJob::MorphJob`'s, memcpy returning to `0x820ACB34`, the post to `0x820ACB80`. |
| `sub_820ACA30` | `WorkItem::Call` (probably) | Overridden | Slot 7 of the work item's vtable `0x820066DC` (the word at `0x820066F8`; the lead had it as a vtable of its own): r3 = the item: the delegate at its +24, a tail call to the delegate's slot 1 (`Delegate::Invoke` `sub_8223A3F0` for `0x82002850`: the target at +8 with the object at +4). r4 (the item + 4, from slot 5) not read. Only caller `WorkItem::Run` (`sub_82090E18`), on the worker threads, for every pool job (the morph jobs' `sub_820AC9C0`, the debug font's load, `FramePasses::Boot`, the bank pairs' loads, ...). Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::WorkItemCall`, harness `morph_jobs`): r3 + 24 (all 64 bits), the delegate's slot 1 as a tail call through the function table (r11 the function). Runs in attract races (the debug font's load at boot). The adapter stays (`WorkItem::Run`, through the vtable). |
| `sub_82090DD0` | `WorkItem::Posted` (probably) | Overridden | Slot 1 of `0x820066DC`. r3 = the item, r4 = the pool's waiter interface (pool − 4, or 0), r5 = the post's r5: +20 = r4 (the object `Job::Wait` calls), the byte +17 = r5, +12 (the state) = 1 (queued), the byte +18 (posted) = 1. Leaf. Called through the vtable by the worker pools' post (`sub_82095A28`, with `[0x82600308]` = 1). Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::WorkItemPosted`, harness `morph_jobs`; r11 = 1). Runs in attract races. The adapter stays (the worker pools' post, through the vtable). |
| `sub_82090DE8` | `WorkItem::Started` (probably) | Overridden | Slot 2 of `0x820066DC`. r3 = the item, r4 = a worker's value (its frame + 80, or 0): +8 = r4, +12 = 2 (running). Leaf. Called through the vtable by the worker loop (`sub_82095AF0`, `functions.md`) when it takes the item. Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::WorkItemStarted`, harness `morph_jobs`; r11 = 2). Runs in attract races. The adapter stays (the worker loop, through the vtable). |
| `sub_82090DF8` | `WorkItem::Finished` (probably) | Overridden | Slot 3 of `0x820066DC`: +12 = 3 (done). Leaf. Called through the vtable by the worker loop (`sub_82095AF0`) after the run. Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::WorkItemFinished`, harness `morph_jobs`; r11 = 3). Runs in attract races. The adapter stays (the worker loop). |
| `sub_82090E18` | `WorkItem::Run` (probably) | Overridden | Slot 5 of `0x820066DC`. r3 = the item: r4 = the item + 4 (0 for a null item), then a tail call to its slot 7 (+28; `WorkItem::Call` `sub_820ACA30` for every item). Called through the vtable by the worker loop (`sub_82095AF0`), unless slot 6 says the item is skipped. Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::WorkItemRun`, harness `morph_jobs`): r4 = r3 + 4 (all 64 bits; 0 for a null item), the slot 7 tail call through the function table (r11 the function), r3 as it came. Runs in attract races. The adapter stays (the worker loop). |
| `sub_82090CB8` | `WorkItem::IsSkipped` (probably) | Overridden | Slot 6 of `0x820066DC`: returns the byte +16 (set by slot 4, `sub_82090E08`). Leaf. Called through the vtable by the worker loop (`sub_82095AF0`) before the run (and through `sub_82090DC8`, the +4 interface's slot 1). Read in full (B7 loop, 2026-11-13). Seen in play; ready. Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::WorkItemIsSkipped`, harness `morph_jobs`). Runs in attract races. The adapter stays (the worker loop). |
| `sub_82090E08` | `WorkItem::Skip` (probably) | Overridden | Slot 4 of `0x820066DC`: the byte +16 = 1. Leaf. Callers not traced (through the vtable). Not seen in play. Read in full (B7 loop, 2026-11-13). Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::WorkItemSkip`, harness `morph_jobs`; r11 = 1; ported though not seen in play). |
| `sub_82090E40` | `WorkItem::~WorkItem` (deleting, probably) | Overridden | Slot 0 of `0x820066DC`. r3 = the item, r4 = flags: +24 = `0x82000ABC` (the functors' base), +4 = `0x82000BA8` then `0x82000AB4`, +0 = `0x82000EB8`, and with bit 0 `operator delete` (`heap.md`); the same stores every holder's destructor makes inline (`MorphJob::~MorphJob`, `AccessorySkeleton::~AccessorySkeleton`, `FramePasses::~FramePasses`). Returns r3. Also reached through `sub_82090E38`. Not seen in play. Read in full (B7 loop, 2026-11-13). Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; `morph_jobs::WorkItemDestroy`, harness `morph_jobs`; ported though not seen in play): the stores, the frame's back chain, `operator delete` with bit 0 (returning to `0x82090E90`) as `MorphJob::~MorphJob`'s; r3 as it came. |
| `sub_82090E38` | | Overridden | Slot 0 of the work item's +4 interface `0x82002908`: r3 − 4, then `WorkItem::~WorkItem` (`sub_82090E40`), a tail call. Not seen in play. Read in full (B7 loop, 2026-11-13). Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; harness `morph_jobs`; ported though not seen in play): r3 − 4, then `WorkItem::~WorkItem`'s register step. |
| `sub_82090DC8` | | Overridden | Slot 1 of the work item's +4 interface `0x82002908`: r3 − 4, then `WorkItem::IsSkipped` (`sub_82090CB8`), a tail call. Not seen in play. Read in full (B7 loop, 2026-11-13). Overridden under `--native_morph_jobs` (B7 loop, 2026-11-13; harness `morph_jobs`; ported though not seen in play): `WorkItem::IsSkipped`'s plain C++ on r3 − 4. |
