# Known functions: the app framework

The game's **app framework**: the 136-byte framework object `main`
(`sub_82090108`) makes (`Framework::Create`, kept on `main`'s stack), the
**run loop** `App::Run` drives it with (a stack object, vtable
`0x82000FE8`: two `std::map`s of handlers by key and a vector of pending
requests), and the framework's set-up (`Framework::Init`, its vtable slot
1, once at boot from `App::Run`), which makes its services through
factories and keeps them at +68 to +128. Each factory is given the run
loop and one of the framework's sixteen interfaces (its vtables at +0 to
+60; each interface's slots are getters for some of the services, the
this-adjustments worked out from the image below and in
`../known-data.md`). Each pass of `App::Run` calls every tick handler in
key order (`ForEachTickHandler`, `TickHandler::Invoke`), then applies the
handlers added and removed during the pass (`RunLoop::ApplyRequest`):
key 4 the frame start (`FrameStart::OnTick`: the debug text reset, then
`sub_8258E1D8`), 7 the framework's own (`Framework::OnTick`,
every pass in play), 8 the app's first tick handler (`sub_82093DC0`,
`functions.md`), 3 and 20 the object at +108; the renderer's and the
others' keys are theirs.

The services are other subsystems', named here only as far as the
framework makes them: the users and sign-in object (+128,
`system-services.md`, which calls it "the app"), the save devices (+124,
`SaveDevices::Update` in `system-services.md`), two worker thread pools
(+68 and +72, the worker loop `sub_82095AF0` in `functions.md`), the
renderer (+76, `Renderer::Init` and the rest in `functions.md`), the
scene (+80, its constructor `sub_82098A98` making the command list,
`command-list.md`), the input manager (+88, `Input::Create` in
`input.md`), the debug text (+92; the debug font
it loads is `sub_82090CC0` in `files-and-saves.md`), the frame start (+84,
8 bytes, `FrameStart::FrameStart`), the graphics object
at +108 (`sub_82098630`), the two pool-2 makers (+112, +116, also at
`0x8296B910` and `0x8296B914`) and the app (+120, `App::Create`; the
app's start-up `sub_82106548` is a lead in `../loops/leads.md`). Their
constructors are leads unless named as in another file.

Which object `InputClientList::BindAll` (`action-mappers.md`) gets the
input manager from, worked out from the code and the image (2026-10-24):
its owner is the app's interface at app + 16 (vtable `0x82000E38`); its
slot +36 is `sub_820944C8` (this − 12 = app + 4), which tail-calls
`sub_82094350`: the word at app + 4 + 20 = app + 24 is the framework's +36
interface (the app's constructor keeps its set-up block's +4 there), and
that interface's slot 1 (`0x82000AFC` + 4) is `sub_82090830` (this − 12 =
framework + 24), which tail-calls `sub_82090778`: the word at framework +
24 + 64 = framework + 88, the input manager. So natively: the manager is
`[[owner + 8] + 52]` (`[owner + 8]` is framework + 36). It doesn't reach
`sub_82090788` (the lead's guess): that's the +36 interface's slot 10
(framework + 124, the save devices).

The functions here run once at boot (the set-up, the factories, the
constructors) or every pass of the run loop (`App::Run`'s helpers, the
tick handlers' thunks and `Framework::OnTick`). Nothing here calls the
kernel; the worker pools' constructor (events, critical sections,
threads) is a lead.

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
| `sub_82090108` | `main` | | Guest entry point on the main thread (from `xstart`): `sub_820909B8`, App::Run through `sub_82090378` and `sub_8209ABE8`, `sub_82090350`; returns App::Run's answer. Never returns in play: closing the window ends the title's threads (the SDK's TerminateTitle) and exits, so `xstart`'s exit, `DbgPrint` and `XamLoaderTerminateTitle` never run. The holder on its stack (r1 + 80) is `Framework::Create`'s r3; `sub_82090350` (never ran) deletes the framework through its slot 0 with 1. |
| `sub_820909B8` | `Framework::Create` | | r3 = the holder (a word): allocates 136 bytes (`sub_825A6880`, operator new), builds the framework there (`Framework::Framework`) and stores it (or 0) at +0. Once at boot, from `main`. |
| `sub_82090420` | `Framework::Framework` | | r3 = the framework: its sixteen vtables at +0 to +60, set first to the base classes' (`0x82010B5C`, `0x82000AB4`, `0x82000ABC`, `0x82000A50`, `0x82000A74`, `0x82000D48`, `0x82000A88`) and then to its own (+0 `0x82000B90`, +4 `0x82000B8C`, +8 `0x82000B84`, +12 `0x82000B80`, +16 `0x82000B7C`, +20 `0x82000B70`, +24 `0x82000B4C`, +28 `0x82000B38`, +32 `0x82000B28`, +36 `0x82000AFC`, +40 `0x82000AF4`, +44 `0x82000AEC`, +48 `0x82000AE4`, +52 `0x82000AD4`, +56 `0x82000ACC`, +60 `0x82000AC8`); zeroes +64 to +80, +88 to +104, +124, +128 and the bytes +132 to +134 (not +84, +108 to +120). Returns r3. Leaf. Once at boot, from `Framework::Create`. Its destructor `sub_820905E0` (the vtables put back, from slot 0 `sub_82090790`) never ran. |
| `sub_82090378` | | | r3 = the holder: tail-calls `sub_8209ABE8` with the framework (+0). Once at boot, from `main`. |
| `sub_8209ABE8` | | | r3 = the framework: builds the run loop on its stack (r1 + 80, `RunLoop::RunLoop`), runs `App::Run` (r3 = the run loop, r4 = the framework) and destroys the run loop (`sub_8209A6E0`, never ran: `App::Run` doesn't return in play); returns `App::Run`'s answer. Once at boot, from `sub_82090378`. |
| `sub_8209A760` | `App::Run` | | Main loop: calls every handler in the tick list at this+4, repeats until the quit flag at this+12 is set. One logic tick per pass, with no measured time. r3 = the run loop, r4 = the framework (2026-10-24, B7 loop): first the framework's slot 1 (`Framework::Init`, r4 = the run loop); when that returns 0 (never), returns 0. Then its slot 2 (`Framework::Start`), and each pass: `ForEachTickHandler` over the map at +4 with `TickHandler::Invoke` on the run loop (a member functor, `0x82099A98`); `ForEachRequest` over the vector at +28 with `RunLoop::ApplyRequest` (`0x8209A458`); the vector cleared (`RequestVector::Clear`); the framework's slot 5 (+134): when set, the run loop's slot 7 (`sub_8209A008`, never ran: an event sent to the listeners at +16) with 1; else its slot 4 (+133): when set, slot 7 with 0, then `Framework::Start` again. The "quit flag at this+12" is the tick handler map's size: the loop runs while any tick handler is left. At the end (never), the framework's slot 3 (`sub_82090910`, releasing the services) and 0. |
| `sub_8209A9F0` | `RunLoop::RunLoop` | | r3 = the run loop (on `sub_8209ABE8`'s stack, fields to +50): vtable `0x82000FE8` (slots: 0 `sub_8209AA58`, 1 `AddListener`, 2 `AddTickHandler`, 3 `sub_8209ABA8` (never ran, probably removes a listener: request type 3), 4 `sub_8209AB08` (never ran: removes a tick handler, request type 1), 5 and 6 folded getters, 7 `sub_8209A008` (never ran: sends an event, its word into +44, to every listener through `sub_82099AD8`)); the tick handler map at +4 (`HandlerMap::HandlerMap`: its head at +8, its size at +12), the listener map at +16 (+20, +24), zeroes the request vector at +28 (+32 first, +36 last, +40 end) and the bytes +48 to +50. Returns r3. Once at boot. |
| `sub_8209AAA8` | `RunLoop::AddTickHandler` | | Vtable slot 2. r3 = the run loop, r4 = the key (a word, read), r5 = a delegate: queues the request {0, key, a copy of the delegate (its slot 2)} on the vector at +28 (`RequestVector::PushBack`); applied at the end of the pass. From `Framework::Init` (key 7), `FrameStart::FrameStart` (4), the app's constructor `sub_82094590` (8), `sub_82098630` (3, 20) and others; once each at boot. |
| `sub_8209AB48` | `RunLoop::AddListener` | | Vtable slot 1. As `AddTickHandler` with request type 2 (the listener map at +16). From `FrameStart::FrameStart` (event 2), the app's constructor (9), `sub_82098630` (8) and others; once each at boot. |
| `sub_8209A930` | `RequestVector::PushBack` | | Probably `std::vector<Request>::push_back` (12-byte requests: +0 the type, +4 the key, +8 the delegate). r3 = the vector (+4 first, +8 last, +12 end), r4 = the request: copied in at the end when there's room, else `sub_8209A600`. From `AddTickHandler`, `AddListener`. |
| `sub_8209A600` | | | Probably the vector's `_Insert_n` (r3 = the vector, r4 / r5 = the position as a checked iterator, r6 = the value; traps on a bad iterator). Calls `sub_8209A0E8`. Not read in detail. From `RequestVector::PushBack` when full. |
| `sub_8209A0E8` | | | Probably the vector's growth and insert (496 lines: a new block from `sub_82099C08`, the elements copied 12 bytes at a time, the old freed by `sub_825A70E8`; `sub_82091CD0`, `sub_820A92D0` on paths that never ran). Not read in detail. From `sub_8209A600`. |
| `sub_82099EE8` | `ForEachRequest` | | Probably `std::for_each` over the request vector with a member functor: r4 / r5 the range (12-byte steps), r6 the functor (function and object); calls it with each request's three words (r4 = type and key, r5 = the delegate); returns the functor at r3. Every pass, from `App::Run`. |
| `sub_8209A458` | `RunLoop::ApplyRequest` | | r3 = the run loop, the request in r4 / r5: type 0 inserts the delegate under the key in the tick handler map (+4, `HandlerMap::Insert`), 2 in the listener map (+16); 1 and 3 find the key (`sub_82091F58`), release the delegate (`sub_82090380`) and erase the node (`sub_82093000`) (never ran: no handler was removed in play). Called through `ForEachRequest`, every pass with a request (at boot). |
| `sub_8209A080` | `RequestVector::Clear` | | r3 = the vector: erases [first, last) (`sub_82092EF8`). Every pass, from `App::Run`. |
| `sub_82092EF8` | | | Probably the vector's `erase(first, last)` for 12-byte elements: moves the elements after the range down, sets last; checked iterators (trap 22 on a bad one). From `RequestVector::Clear`. |
| `sub_82099FB0` | `HandlerMap::HandlerMap` | | Probably `std::map<unsigned, Delegate*>`'s constructor: r3 = the map: its head node (`sub_82099E88`) at +4 with left, parent and right pointing to itself and the nil byte (+21) set, size (+8) 0. From `RunLoop::RunLoop` (twice) and the app's constructor `sub_82094590` (four maps at +28, +40, +52, +64). |
| `sub_82099E88` | | | Probably the map's `_Buynode` for the head: one 24-byte node (`sub_82099C80`), +0, +4, +8 zeroed, the color (+20) 1, nil (+21) 0. From `HandlerMap::HandlerMap`. |
| `sub_82099C80` | | | Probably `allocator<node>::allocate` for the maps' 24-byte nodes (+0 left, +4 parent, +8 right, +12 the key, +16 the delegate, +20 the color, +21 nil): r3 = the count: count × 24 bytes from operator new (`sub_825A6880`); past 0x0AAAAAAA it builds an exception object first (`sub_82091200`, probably `bad_alloc`; never ran). From `sub_82099E88`, `sub_82099E20` and others. |
| `sub_82099E20` | | | Probably the map's `_Buynode`: a 24-byte node (`sub_82099C80`) with r4, r5, r6 as left, parent and right, the 8-byte value from r7 at +12, the color byte r8 at +20, nil 0. From `sub_82093400`. |
| `sub_82093740` | `HandlerMap::Insert` | | Probably `std::map::insert` (unique keys, compared unsigned): r3 = the result (+0, +4 the iterator, +8 the inserted byte), r4 = the map, r5 = the key and delegate. Walks from the root to the leaf, steps back once (`sub_82091AE8`) to check for the key, and inserts with `sub_82093400` when it isn't there. 6 call sites: `RunLoop::ApplyRequest` (2), `sub_82091FF0`, `sub_82093BE0` (2), `sub_82093C2C`. |
| `sub_82093400` | | | Probably the map's `_Insert`: the new node (`sub_82099E20`) hung left or right of r6, the size up (a `length_error` on a path that never ran), and the red-black fix-up with `sub_82092A00` and `sub_82092B10`. Not read in detail. Only from `HandlerMap::Insert`. Was in the containers lead (`../loops/leads.md`). |
| `sub_82091AE8` | | | Probably the map's checked iterator `operator--` (r3 = the iterator: +0 the map, +4 the node; trap 22 on a null map): the previous node in order. From `HandlerMap::Insert`. |
| `sub_82092A00` | | | Probably the red-black tree's left rotation (r3 = the map, r4 = the node). Leaf. From `sub_82093400` (2) and `sub_82093000` (3, never ran). |
| `sub_82092B10` | | | Probably the right rotation (as `sub_82092A00`). Leaf. From `sub_82093400` (2) and `sub_82093000` (3). |
| `sub_82099D50` | `ForEachTickHandler` | | Calls each handler in a list through a member function pointer. In order over the tick handler map (by key; 2026-10-24, B7 loop), each node's key and delegate passed to the functor (`TickHandler::Invoke` on the run loop); `sub_82099B18` steps to the next node. Every pass, from `App::Run`; `sub_8209A008` (never ran) over the listener map. |
| `sub_82099B18` | | | Probably the handler maps' checked iterator `operator++` (r3 = the iterator: +0 the map, +4 the node; trap 22 on a null map or at the end): the next node in order. Leaf. From `ForEachTickHandler` every pass (once a handler), 4 call sites. |
| `sub_82099A98` | `TickHandler::Invoke` | Wrapped | r3 = the app, r4 = the handler (target at +8). Skips some tick handlers on render-only passes (`SkipTickHandlerOnRenderOnlyPass`, `src/frame_pacing/game_messages.cpp`); the diagnostics' before and after hooks (`--log_tick_handlers`, `--find_per_frame_state` and others). r3 is the run loop (2026-10-24, B7 loop), r4 the map node's key (high word) and delegate (low word): the delegate's slot 1 (`Delegate::Invoke` or `MemberDelegate::Invoke`, `functions.md`) with r4 = the run loop. |
| `sub_82090A08` | `Framework::Init` | | Vtable slot 1 (no direct callers): r3 = the framework, r4 = the run loop (kept at +64). Makes the services, each factory given a block on its stack (+0 the run loop, +4 one of the framework's interfaces, and settings): +128 `Users::Create` (interface +60), +124 `SaveDevices::Create` (+56; 1, 1, r4 = 100), +68 and +72 `WorkerPool::Create` (+12; the first with 13, 4 and one thread from the table {2, 2}, the second with 12, 3 and two from {3, 2, 5, 2}), +76 `Renderer::Create` (+4), +84 `FrameStart::FrameStart` (8 bytes, operator new; +40), +80 `Scene::Create` (+8), +88 `Input::Create` (+16, `input.md`), +92 `DebugText::Create` (+20 and the first pool), +108 `sub_82098630` (44 bytes; +52), +112 `PoolMaker::PoolMaker` (8 bytes; +44) and +116 `sub_820AC848` (8 bytes; +48), both also into `0x8296B910` and `0x8296B914`, +120 `App::Create` (+36); a failed allocation leaves 0. Then adds `Framework::OnTick` as tick handler 7 (a member delegate, vtable `0x8200833C`, on the framework) through the run loop's slot 2. Returns 1. Once at boot, from `App::Run`. Its group under it is the whole start-up (79 functions, the app's set-up among them). |
| `sub_820902B0` | `Framework::Start` | | Vtable slot 2: tail-calls the app's (+120, the app's interface at app + 20, vtable `0x82000E08`) slot +36 (`sub_82092878`: the game's first state set up, the app's). From `App::Run`: once at boot after `Framework::Init`. |
| `sub_820902C8` | `Framework::OnTick` | | Tick handler 7 (a member delegate's target; r3 = the framework, r4 = the run loop): calls the input manager's (+88) slot 1 (`InputManager::GetProcessor`, `input.md`) with r4 = &0 (the result not used), clears the byte +133, and asks the app (+120) its slot +44 (`sub_82092928`, the app's): when that returns 0, removes tick handler 7 (the run loop's slot 4) and sets +134 (which ends `App::Run`'s pass with an event). Every pass; slot 4 never ran in play, so the app's answer was always nonzero. |
| `sub_82090760` | | Overridden | Vtable slot 4: returns the byte at +133. Leaf. From `App::Run`, every pass. Plain C++ `framework::Flag133` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `Flag133 (sub_82090760)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090768` | | Overridden | Vtable slot 5: returns the byte at +134. Leaf. From `App::Run`, every pass. Plain C++ `framework::Flag134` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `Flag134 (sub_82090768)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090770` | | Overridden | Returns the word at +60. Leaf. A getter the linker folded: the scene at framework + 80 directly as the +20 interface's slot 2, and through the this-adjusting thunks from +24, +28, +36 (`sub_82090908`) and +52 (`sub_82090868`). Not read who calls it otherwise. Plain C++ `framework::Word60` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `Word60 (sub_82090770)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090778` | | Overridden | Returns the word at +64. Leaf. Folded: on the framework, the run loop; through the +24 interface (its slot 3) and the thunks from +28, +32 and +36 (`sub_82090830`), the input manager at framework + 88. Plain C++ `framework::Word64` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `Word64 (sub_82090778)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090788` | | Overridden | Returns the word at +88. Leaf. Folded: on the framework, the input manager; the +36 interface's slot 10 (framework + 124, the save devices), and also at `0x82076CD0` (another class's vtable, beside `0x82076CE8` of `sub_825D82E8`). Not read who calls it. Plain C++ `framework::Word88` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `Word88 (sub_82090788)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090830` | | Overridden | The +36 interface's slot 1: this − 12, then `sub_82090778`: the input manager (framework + 88). What `InputClientList::BindAll` reaches through the app (`sub_82094350`, intro). Plain C++ `framework::InputManagerFrom36` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `InputManagerFrom36 (sub_82090830)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090828` | | Overridden | The +52 interface's slot 2: this − 44, then `sub_8248F030`: the renderer (framework + 76). Plain C++ `framework::RendererFrom52` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `RendererFrom52 (sub_82090828)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090838` | | Overridden | The +52 interface's slot 1: this − 28, then `sub_824892F8`: the first worker pool (framework + 68). Plain C++ `framework::WorkerPoolFrom52` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `WorkerPoolFrom52 (sub_82090838)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090840` | | | The +36 interface's slot 5: this − 12, then `sub_824936E8` (`files-and-saves.md`): the second worker pool (framework + 72). |
| `sub_82090860` | | Overridden | The +24 interface's slot 4 and the +40 interface's slot 1: this − 16, then `sub_8248F030`: from +24 the renderer (framework + 76), from +40 the debug text (framework + 92; `FrameStart::OnTick`). Plain C++ `framework::RendererOrTextFrom24` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `RendererOrTextFrom24 (sub_82090860)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090868` | | Overridden | The +52 interface's slot 3: this − 32, then `sub_82090770`: the scene (framework + 80). Plain C++ `framework::SceneFrom52` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `SceneFrom52 (sub_82090868)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090890` | | | The +44 interface's slot 1: this − 20, then `sub_824936E8`: the second worker pool (framework + 72); `PoolMaker::PoolMaker` keeps it. |
| `sub_820908C0` | | | The +48 interface's slot 1: this − 24, then `sub_824936E8`: the second worker pool (framework + 72); `sub_820AC848` keeps it. |
| `sub_820908D8` | | | The +56 interface's slot 1: this − 20, then `sub_82493700` (`system-services.md`): the users (framework + 128). |
| `sub_820908E0` | | Overridden | The +36 interface's slot 2: this − 28, then `sub_8248F030`: the renderer (framework + 76). Plain C++ `framework::RendererFrom36` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `RendererFrom36 (sub_820908E0)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_820908E8` | | Overridden | The +36 interface's slot 4: this − 12, then `sub_824892F8`: the first worker pool (framework + 68). Plain C++ `framework::WorkerPoolFrom36` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `WorkerPoolFrom36 (sub_820908E8)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82090908` | | Overridden | The +36 interface's slot 3: this − 16, then `sub_82090770`: the scene (framework + 80). Plain C++ `framework::SceneFrom36` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `SceneFrom36 (sub_82090908)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_8248F030` | | Overridden | Returns the word at +68. Leaf. Folded: the +8 interface's slot 1 and the +24's slot 6 directly (framework + 76 and + 92), and through the thunks above (the renderer, the debug text); also from `sub_82491C50` (2), `sub_824928A8`, `sub_82492E00` (another class's +68, near `MovieSource::AtEnd`; not read). Plain C++ `framework::Word68` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `Word68 (sub_8248F030)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_824892F8` | | Overridden | Returns the word at +44. Leaf. Folded: the +24 interface's slot 1 directly (framework + 68, the first worker pool), and `sub_82090838`, `sub_820908E8`. Plain C++ `framework::Word44` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `Word44 (sub_824892F8)`); ran in the attract race (2026-10-25, B7 loop). |
| `sub_82099A08` | `Users::Create` | | r3 = the set-up block (the run loop, the framework's +60 interface, 0, 0): allocates 256 bytes and builds the users and sign-in object there (`sub_820998B8`, `system-services.md`: vtable `0x82000FA0`, the listener at +240; the object that file's `App::ReadUsers` and `App::PollSignIn` run on), or returns 0. Once at boot, from `Framework::Init` (+128). |
| `sub_820A63C8` | `SaveDevices::Create` | | r3 = the block (the run loop, the +56 interface, 1, 1), r4 = 100: allocates 116 bytes and builds the save devices there (`sub_820A6160`, a lead: a notification listener `sub_822952B8`, `sub_820A4560`; `SaveDevices::Update` in `system-services.md`), or 0. Once at boot (+124). |
| `sub_82095620` | `WorkerPool::Create` | | r3 = the block (the run loop, the +12 interface, two words, a table of 8 bytes a thread, the thread count): allocates 296 bytes and builds a worker pool there (`sub_82094F30`, a lead: vtables `0x82000F0C`, `0x82000F04`, `0x82000EE8`; events, critical sections and threads, the threads running `sub_82095AF0`, `functions.md`); returns it + 8 (its `0x82000EE8` interface), or 0. Twice at boot (+68, +72). |
| `sub_820925A0` | `Renderer::Create` | | r3 = the block (the run loop, the +4 interface): allocates 400 bytes and builds the renderer there (`sub_82092358`, `Renderer::Init` in `functions.md`); returns it + 20, or 0. Once at boot (+76). |
| `sub_82098C70` | `Scene::Create` | | r3 = the block (the run loop, the +8 interface): allocates 80 bytes and builds the scene there (`sub_82098A98`, a lead: vtable `0x82000F3C`, the command list at scene + 8, `command-list.md`, the primitive drawer `sub_825915E0`); returns it or 0. Once at boot (+80). `Scene::Update` and `Draw` are in `functions.md`. |
| `sub_82091160` | `DebugText::Create` | | r3 = the block (the framework's +20 interface, the first worker pool's interface): allocates 52 bytes and builds the debug text there (`DebugText::DebugText`), or 0. Once at boot (+92). |
| `sub_82090EA8` | `DebugText::DebugText` | | r3 = it, r4 = the block: vtable `0x82000BC8` (slots 0 `sub_82091110`, 1 `sub_82090F88` (never ran: a printf through `sub_825A71B0` drawn with `sub_8258C3A8`, when +20, the job's state, is 3), 2 `DebugText::SetPosition`, 3 `sub_82091060`); +4 the block's interface; at +8 a 36-byte job (vtables `0x820066DC`, `0x82002908`, delegate `0x82002850` calling `sub_82090CC0` with the debug text: the debug font's load, `files-and-saves.md`); +48 0; itself into `0x8266D080`; then hands the job to the pool (its slot 1). Returns r3. Once at boot. |
| `sub_82091050` | `DebugText::SetPosition` | Overridden | Vtable slot 2: r4, r5 = pointers to x and y: tail-calls `sub_8258BE08` with the two words. From `FrameStart::OnTick`, every pass (64, 64). Plain C++ `framework::DebugTextSetPosition` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `DebugText::SetPosition (sub_82091050)`); ran in the attract race (2026-10-25, B7 loop). The adapter leaves r3, r4 the words and r11 as the original. |
| `sub_8258BE08` | `DebugFont::SetPosition` | Overridden | r3 = x, r4 = y: into `0x8296BCE0` (+0, +4) and +8, +12 (probably the cursor and the line start). Leaf. From `DebugText::SetPosition`, every pass. Beside the debug font's set-up `sub_8258BD58`, `sub_8258BE48` (`files-and-saves.md`'s `sub_82090CC0`). Plain C++ `framework::DebugFontSetPosition` (`src/framework/framework.cpp`) behind an adapter (`src/hooks/framework.cpp`, `--native_framework`; the same results, harness `framework`: `DebugFont::SetPosition (sub_8258BE08)`); ran in the attract race (2026-10-25, B7 loop). The adapter leaves r11 at the position, as the original. |
| `sub_82092710` | `FrameStart::FrameStart` | | r3 = it (8 bytes from `Framework::Init`), r4 = the block (the run loop, the +40 interface): vtable `0x82000D84` (slots 0 `FrameStart::OnTick`, 1 `sub_82092650`, 2 `sub_820926C8`), +4 the interface; adds tick handler 4 (a delegate, vtable `0x82002850` (`Delegate::Invoke`), calling `sub_82096330` with it) and a listener for event 2 (calling `sub_82096340`: slot 1, `sub_82092650`, never ran: on the event's value 1, removes both). Returns r3. Once at boot (+84). |
| `sub_820925F8` | `FrameStart::OnTick` | | Vtable slot 0, through tick handler 4: the debug text (the +40 interface's slot 1) set to (64, 64) through its slot 2 (`DebugText::SetPosition`), then `sub_8258E1D8` (the `ScopedLock` users' lead: VirtualFree, XPhysicalFree and XMemFree under a lock, probably frees put off until the pass's start). Every pass, the first tick handler run (key 4). |
| `sub_82096330` | | | A delegate's target: calls r3's vtable slot 0 (tail call). From `Delegate::Invoke`: `FrameStart`'s tick handler 4, every pass, and others not read. |
| `sub_82096340` | | Wrapped | A tick handler thunk (vtable slot 1 of r3); names its target for `--find_per_frame_state`. A delegate's target like `sub_82096330` (2026-10-24, B7 loop): `FrameStart`'s event 2 listener (never sent in play) and others not read. |
| `sub_820AC8F8` | `PoolMaker::PoolMaker` | | Probably. r3 = it (8 bytes from `Framework::Init`), r4 = the framework's +44 interface: vtable `0x82002848` (slots 0 `sub_820AC950`, 1 `sub_820AC8A0`: makes a 224-byte object over `sub_820ACAA8` with the pool, never ran), +4 the interface's slot 1 (the second worker pool). Returns r3. Once at boot (+112, `0x8296B910`); `0x8296B910`'s only reader `sub_82594FD0` never ran. |
| `sub_820AC848` | | | As `PoolMaker::PoolMaker` with the +48 interface and vtable `0x82002840` (slot 1 `sub_820AC7F0`: a 224-byte object over `sub_820ACC08`, never ran). Once at boot (+116, `0x8296B914`, which nothing else reads as a word). |
| `sub_82094AB8` | `App::Create` | | r3 = the block (the run loop, the framework's +36 interface): allocates 132 bytes and builds the app there (`sub_82094590`, the app's start-up lead's: vtables at +0 to +20, the block's +4 at +24, its maps, the camera and the sound manager, then `sub_82106548`); returns it + 20 (its `0x82000E08` interface), or 0. Once at boot (+120). |
| `sub_820944C8` | | | The app's: slot +36 of its interface at app + 16 (vtable `0x82000E38`): this − 12, then `sub_82094350`. `InputClientList::BindAll`'s call for the input manager (once at boot; logged 2026-10-24, `action-mappers.md`). Taken from the app's lead for that path. |
| `sub_82094350` | | | The app's: r3 = app + 4: the word at +20 (app + 24, the framework's +36 interface), its slot 1 (`sub_82090830`: the input manager). Leaf but for the tail call. From `sub_820944C8`. |
