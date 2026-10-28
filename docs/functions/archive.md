# Known functions: the archive

The game's archive streaming: the two archives `BUILD.DAT` (asset ids
0-23010) and `BUILD_CRS.DAT` (ids from 23011, the course data, id
- 23011 in its own table), their tables of contents (`BUILD.TOC`,
`BUILD_CRS.TOC`: a 16-byte header, then a 16-byte entry an asset, +0 the
start sector (2,048 bytes), +4 the stored size, +8 the unpacked size or
0 when stored as is; 23,011 and 1,359 entries, the counts at
`0x82600AA8` and `0x82600AB8` in the image), the request queue the main
thread fills (`Archive::Load`, about 55 call sites) and the loader thread
that empties it (reads, UCL unpacks into a 10 MB staging ring, then the
request's callback on the loader thread). The callers ask an asset's
size (`Archive::AssetSize`, 41 call sites) and allocate it before
`Archive::Load`; the loading screens and the scenery streaming ask the
queue's length (`Archive::QueueLength`), the front end waits for it to
drain (`Archive::WaitIdle`) or cancels what's still queued
(`Archive::CancelQueued`). Its globals are in `../known-data.md`
(`0x826A5100`-`0x826A5168`, the request slots, the queue at
`0x826BAB08` and the four semaphores).

Most of the archive's functions were met in B4 and have their rows in
[`files-and-saves.md`](files-and-saves.md), where they stay (shared with
this file): `Archive::Init` (`sub_8209E210`, Overridden) and
`Archive::Shutdown` (`sub_8209D3B0`), `Archive::Load` (`sub_8209DE48`),
`Archive::AssetSize` (`sub_820A0678`, this map's lead),
`Archive::FindLooseFile` (`sub_8209D8D8`), the queueing
(`sub_8209DC18`), the slot lookup (`sub_8209D818`), the cancel of one
request (`sub_8209DB20`), the loader thread (`sub_8209E118`,
Overridden: its file reads are ours since B4 step 2,
`src/hooks/archive_streaming.cpp`), its next request (`sub_8209DD98`)
with the picker (`sub_8209D950`), the set-up (`sub_8209D490`) and the
finish (`sub_8209D688`, which calls the callback), the loose read
(`sub_8209DFA8`), the Dropped archive read and its completion
(`sub_8209DD00`, `sub_8209D808`), and `Ucl::Unpack` (`sub_82294610`,
`sub_822947F8`). This file holds the rest: the TOC lookup, the queue's
queries, the loader thread's functor, the request queue's list class and
the static set-ups and clean-ups of the queue, the slots and the
semaphores (the semaphores' set-ups themselves, `sub_825D9200`,
`sub_825D9238`, `sub_825D9270`, `sub_825D92A8`, are in
`video-output.md` with the `Semaphore` class).

The sync: the archive has four of the game's semaphores (`Semaphore::`,
`video-output.md`; 40 bytes each): the queue lock `0x826BAAE0` (count
1), the loader's work `0x826BAAB8`, a request done `0x826BAA68` and the
idle one `0x826BAA90` (the last three taken once by `Archive::Init`, so
they start at 0). Every user of the four is an archive function in this
file or `files-and-saves.md` (and their static set-ups and clean-ups):
no other code names their addresses. So the user's call for the device
lock (2026-10-24: a lock whose users are made native together goes over
a host semaphore, `video-output.md`; also "Precedent first" in
`../loops/b7-loop.md`) covers them: each one a host semaphore, taken by
every user, once those users are ours together. Two users read a
semaphore's count (+32) directly, not only through `Wait` and `Signal`:
the queueing (`sub_8209DC18`) wakes the loader only when the queue is
empty and the work semaphore's count is 0, and the picker
(`sub_8209D950`) takes the work semaphore again when its count isn't 0
(so a host semaphore here must keep a count it can read). The loose read
(`sub_8209DFA8`) signals request-done and is blocked by the file
object (`sub_820A6A00`, `sub_820A6A40`, a lead), the open by name
(`sub_820A6AB8`, `files-and-saves.md`) and sprintf under it
(`c-runtime.md`). The callbacks `Archive::Load` is given are called
indirectly from the loader thread (`sub_8209D688`): about 20 of the
loads' own functions, some already ours (`car-specs.md`).

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
| `sub_8209D868` | `Archive::Toc` | Overridden | Native (`archive::Toc`, `src/archive/archive.cpp`, behind `--native_archive`; harness `archive`), its adapter kept for the recompiled callers. r3 = an asset id: the TOC's address, `[0x826A5120]` (`BUILD.TOC`) for ids below 23011, else `[0x826A5128]` (`BUILD_CRS.TOC`); a leaf. The caller indexes it itself (entry id + 1, or id - 23011 + 1 in `BUILD_CRS`, 16 bytes each). From `Archive::AssetSize` (`sub_820A0678`, `files-and-saves.md`) and 21 more call sites: `Params::Init` and its loads' callbacks (`car-specs.md`, guest calls from their adapters), `DataTables::Init` (`data-tables.md`), and the call sites at `821802F0` and `8217664C` (not mapped). `Archive::Load` (`sub_8209DE48`) has the same two loads inlined. Read in full. |
| `sub_8209D730` | `Archive::WaitIdle` | | No arguments: under the queue lock (`Semaphore::Wait`, `Signal` on `0x826BAAE0`), when the queue (`0x826BAB08`) has requests (+8), `0x826A5108` = 1 (the loader signals the idle semaphore once it's drained, `sub_8209E118`); then, when `0x826A5108` is set, waits on the idle semaphore `0x826BAA90`. From the app's start-up `sub_82106548`, the front-end screens' loads `sub_82197278` and `sub_8222FD68` (also `Strings::Load` callers, `files-and-saves.md`), `sub_82278058` and `Archive::CancelQueued`. Read in full. |
| `sub_8209D7B0` | `Archive::QueueLength` | | No arguments: the queue's count (`0x826BAB10`, the queue's +8) read under the queue lock (`0x826BAAE0`). From the course scenery's streaming (`SceneryList::LoadAll`, `Stream`: loads only while it's below 480 or 960, `course-scenery.md`), the race's loading waits (`RaceStates::StageLoadTick`, `RollingLoadTick`, `ReplayLoadTick`, `sub_82168018`, `sub_821683A8`, `race-states.md`) and the call sites at `821515C4`, `82151744`, `82221354` and `822204D0` (not mapped): 13 call sites. Read in full. Was a lead (course scenery, 2026-10-24). |
| `sub_8209DB90` | `Archive::CancelQueued` | | r3 = wait: under the queue lock, every request in the queue still waiting (state +52 0) with flag 0x4000 (+12; `Archive::Load` sets it on every request) is taken off (`List::Remove`, `sub_82294C00`) and its slot freed (+8 = -1); then with r3 not 0, `Archive::WaitIdle`. Only from `sub_82229678` (not mapped). Read in full. |
| `sub_8209DD80` | `LoaderFunctor::Invoke` | | Slot 1 of the functor vtable `0x82001DDC` (no direct callers): r3 = the functor, r4 = an argument: a tail call of the functor's +4 with r4 as r3. Its one instance is at `0x82600B68` (inside the loader thread's parameters at `0x82600B50`, +24): +4 = `0x8209E118`, the loader thread, written by `Archive::Init`; so it starts the loader thread (probably from the thread class the streaming object's vtable +4 reaches). Once in play. Read in full. |
| `sub_8209DF50` | `LoaderFunctor::Clone` | | Slot 2 of the functor vtable `0x82001DDC` (no direct callers): r3 = the functor: an 8-byte copy (`operator new`, `sub_825A6880`) with the vtable and +4; 0 when the allocation fails. Not seen in play. Read in full. |
| `sub_82094D20` | | | A functor's deleting destructor (r3 = it, r4 = flags): the vtable `0x82000ABC` (the functors' base, probably) and, with bit 0, `operator delete`. Slot 0 of `0x82001DDC` (the loader's functor) and of four other vtables (`0x82000ABC`, `0x82002850`, `0x8200833C`, `0x82018DE0`): shared. Not seen in play. Read in full. |
| `sub_82294B80` | `List::List` | Native | `archive::ListConstruct` (`src/archive/archive.cpp`), called by the queue's native static set-up (`archive::QueueStaticInit`), which `--native_archive` switches; no adapter (its one caller is ours); harness `archive`. r3 = the list (20 bytes), r4 = whether it deletes its nodes: +0 the vtable `0x820128C8`, +4 = r4, +8 the count, +12 the head, +16 the tail = 0. Only from the queue's static set-up (`sub_825D9160`, r4 = 0). Read in full. |
| `sub_82294BA8` | `List::Append` | Overridden | Native (`archive::ListAppend`, behind `--native_archive`; harness `archive`, on lists made in the scratch area), its adapter kept for the recompiled queueing. r3 = the list, r4 = a node (linked by its +4): the node after the tail (+16) or as the head (+12) when empty, its +4 = 0, the tail = it, the count (+8) up by one; 1, or 0 for no node. Only from the queueing (`sub_8209DC18`). Read in full. Was a lead (scene models, 2026-10-25). |
| `sub_82294C00` | `List::Remove` | Overridden | Native (`archive::ListRemove`, behind `--native_archive`; harness `archive`), its adapter kept for the recompiled picker, `sub_8209DB20` and `Archive::CancelQueued`. r3 = the list, r4 = a node: finds it from the head by +4; unlinks it (the previous node's +4, the head and the tail fixed), the count down by one; 1, or 0 when it isn't there (or r4 is 0). From the picker (`sub_8209D950`, twice), `sub_8209DB20` and `Archive::CancelQueued`. Read in full. |
| `sub_82294C88` | `List::DeleteNodes` | | r3 = the first node, r4 = whether to delete them: walks the nodes by +4 and, with r4, calls each one's deleting destructor (vtable slot 0, flags 1). Only from `List::Clear`. Not seen in play. Read in full. |
| `sub_82294CF0` | `List::Clear` | | r3 = the list: `List::DeleteNodes(head, +4)`, then the count, head and tail = 0. The base destructor's body (from `RequestQueue::~RequestQueue` and `sub_82294D48`). Not seen in play. Read in full. |
| `sub_82294D48` | `List::~List` (deleting) | | Slot 0 of `0x820128C8`: r3 = the list, r4 = flags: the base vtable, `List::Clear`, and with bit 0 `operator delete`. No direct callers. Not seen in play. Read in full. |
| `sub_82294D38` | `RequestQueue::~RequestQueue` | | r3 = the queue: the base vtable `0x820128C8`, then a tail call of `List::Clear`. From the queue's clean-up (`sub_825DACB8`) and its deleting destructor (`sub_8209D888`). Not seen in play. Read in full. |
| `sub_8209D888` | `RequestQueue::~RequestQueue` (deleting) | | Slot 0 of the queue's vtable `0x82001DD4`: r3 = the queue, r4 = flags: `RequestQueue::~RequestQueue` and with bit 0 `operator delete` (`sub_825A70E8`). No direct callers. Not seen in play. Read in full. |
| `sub_82294B68` | `ListNode::ListNode` | Native | `archive::NodeConstruct`, called by the slots' native static set-up (`archive::SlotsStaticInit`), which `--native_archive` switches; no adapter; harness `archive`. r3 = the node, r4: +0 the vtable `0x82010B5C`, +4 (the link) = r4. Only from the slots' static set-up (`sub_825D91B0`, r4 = 0). Read in full. |
| `sub_8224FB10` | `ListNode::~ListNode` (probably) | | r3 = the node: +0 = `0x82010B5C`, the node base's vtable. 9 call sites (the slots' clean-up `sub_825DAD38`, the slot's deleting destructor `sub_8223A2F8`, and others at `82230B7C`, `822383B8`-`822383F4`, `82246C2C`): shared with other node classes. Not seen in play. Read in full. |
| `sub_8223A2F8` | `RequestSlot::~RequestSlot` (deleting) | | Slot 0 of the request slot's vtable `0x82001DD8` (and of one more vtable at `0x8207FD00`, not read): r3 = the slot, r4 = flags: `ListNode::~ListNode` and with bit 0 `operator delete`. No direct callers. Not seen in play. Read in full. |
| `sub_825D9160` | `RequestQueue::StaticInit` | Overridden | Native (`archive::QueueStaticInit`, atexit through `c_runtime::AtexitFrom`, the original when the native atexit can't, as `SignInWatch::StaticInit`'s; behind `--native_archive`; harness `archive`, on the game's queue under its lock, put back), its adapter kept for `_cinit`'s table. Static set-up (`_cinit`'s table, once at start-up): the request queue at `0x826BAB08` made (`List::List(0)`, then its vtable `0x82001DD4`) and its clean-up `sub_825DACB8` registered (`atexit`, `c-runtime.md`). Read in full. |
| `sub_825D91B0` | `RequestSlots::StaticInit` | Overridden | Native (`archive::SlotsStaticInit`, as the queue's; harness `archive`), its adapter kept for `_cinit`'s table. Static set-up: the 960 request slots at `0x826A5168` (92 bytes each) made (`ListNode::ListNode(0)`, then the vtable `0x82001DD8`) and their clean-up `sub_825DAD38` registered (`atexit`). Read in full. |
| `sub_825DACB8` | | | The queue's clean-up (registered by `sub_825D9160`, at exit): `RequestQueue::~RequestQueue(0x826BAB08)`. Not seen in play. Read in full. |
| `sub_825DAD38` | | | The slots' clean-up (registered by `sub_825D91B0`, at exit): `ListNode::~ListNode` on the 960 slots, last first. Not seen in play. Read in full. |
| `sub_825DACC8` | | | The queue lock's clean-up (registered by `sub_825D9200`, `video-output.md`, at exit): `Semaphore::~Semaphore(0x826BAAE0)`. Not seen in play. Read in full. |
| `sub_825DACD8` | | | The idle semaphore's clean-up (registered by `sub_825D9238`): `Semaphore::~Semaphore(0x826BAA90)`. Not seen in play. Read in full. |
| `sub_825DACE8` | | | The work semaphore's clean-up (registered by `sub_825D9270`): `Semaphore::~Semaphore(0x826BAAB8)`. Not seen in play. Read in full. |
| `sub_825DACF8` | | | The request-done semaphore's clean-up (registered by `sub_825D92A8`): `Semaphore::~Semaphore(0x826BAA68)`. Not seen in play. Read in full. |
| `sub_82592638` | `Semaphore::~Semaphore` | | r3 = the semaphore (`video-output.md` has the class): its critical section's destructor (+4; `sub_82106540`, the empty shared destructor, `audio.md`), then its event closed (`sub_8258DD60`, probably `Event::Close`, `functions.md`). From the four clean-ups above (tail calls) and `sub_8258D538` (probably the video output's destructor). Not seen in play. Read in full. |
