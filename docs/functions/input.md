# Known functions: input

The game's input below the action mappers: the input manager (3,984
bytes, vtable `0x82000F48`, made once at boot by the framework's set-up
`sub_82090A08`, which keeps it at its +88) holding five logical pads
(the four players' and an "any pad" one summing them), four physical
pads (one per controller), five input processors (vtable `0x8206C0C4`:
dead zones, stick directions, button words and hold-to-repeat counters)
and five lists of listeners (one per logical pad; each action mapper's
listener is an input processor of the same class, added to its pad's
list when the mapper is bound); the manager's slots that hand them out,
the classes' constructors, the C++ library's `std::list` of pointers the
listener lists are (as far as the manager uses it), and the button tests
the app and the mappers read. The input path from SDL to the processors
is B1's native code (`itc_recomp/src/input/`, the update in
`itc_recomp/src/hooks/input_update.cpp`): Input::Update, the pad read,
the logical pads' build and the processors' update, and the XInput
wrappers below them (B1 in `../rexglue-removal.md`); what each object
holds and what the update does, step by step: "The input processors",
"Physical pad", "Logical pad" and "Who reads the input manager" in
`../pause-menu-fix.md`. The objects' fields: `../known-data.md`.

Callers outside: the framework's set-up (`sub_82090A08`, a lead) makes
the manager (`Input::Create`); the action mappers (`action-mappers.md`)
make their listeners with `InputProcessor::InputProcessor`
(`ActionMapper::ActionMapper`) and bind them through the manager's slots
2 and 3 (`ActionMapper::Bind`, from `InputClientList::BindAll`, which
gets the manager from its owner's vtable slot +36: the object
`ActionMappers::Init` is given, the app of `sub_82106548` rather than the
input manager, probably reaching `sub_82090788`, the framework's +88,
through the app's `sub_820944C8` and `sub_82094350`; not traced, a lead);
the first tick handler (`sub_82093DC0`, `functions.md`) gets processor 0
through slot 1 and tests its buttons; `InputProcessor::TestHeld` and
`AllHeld` (the mappers' listener tests) are in `action-mappers.md`, and
`sub_825A18D8` (the physical pad's two records) in `model-animation.md`.
The vtables are read from the image (`itc/xex_image/default.bin`,
`../known-data.md`): the manager's `0x82000F48` has six slots (0
`sub_82098FF8`, probably the destructor; 1 `GetProcessor`; 2
`GetLogicalPad`; 3 `AddListener`; 4 `sub_82099048`, probably
RemoveListener; 5 `GetPhysicalPad`), the processor's `0x8206C0C4` slot 0
`sub_8258CF40` and slot 1 the update (its base's `0x8206CC00`:
`sub_825925A8`, then the same update), the logical pad's `0x8206CBEC`
`sub_82591898` and `sub_82591B30`, the physical pad's `0x8206CBF4`
`sub_82592058` and `sub_820988B8`. Never seen in play (no rows): the
slot 0 and 4 functions, the four classes' slot 0 (probably their
destructors), `sub_82592400` (probably a processor's reset: called by
`AddListener` only when its pad's list already has a listener, and by
slot 4), the manager's handler for message 7 (`sub_82098CE8`: on a word
of 1, its dispatcher's slots +16 with 5 and +12 with 7, probably the
two handlers removed), and the list's error paths (`bad_alloc`,
`length_error`). The rows were drafted by the B7 loop's mapping agent
(2026-10-24) from the generated code, except the rows moved from
`functions.md` (B1's, as they were, with what's new added).

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
| `sub_820993E8` | `Input::Create` | | r3 = the set-up's argument block (+0 the message dispatcher, +4 a pointer into the framework, its +16): a 3,984-byte input manager (`sub_825A6880`, operator new) built by `InputManager::InputManager` with it, or 0 when the allocation fails; returns it. Once at boot, from the framework's set-up `sub_82090A08` (`82090B80`), which keeps it at +88. |
| `sub_820991E0` | `InputManager::InputManager` | | r3 = the manager (3,984 bytes), r4 = the argument block: vtable `0x82000F48`; five logical pads (196 bytes each from +8, `LogicalPad::LogicalPad`), four physical pads (212 bytes from +988, `PhysicalPad::PhysicalPad`), five processors (416 bytes from +1840, `InputProcessor::InputProcessor`) and five listener lists (12 bytes from +3920, `PtrList::PtrList`). +4 = the block's +4. Registers two handlers with the dispatcher (block +0), each a 12-byte functor on the stack (vtable `0x82002850`, the manager, the target; set back to `0x82000ABC` after): message 5 through its slot +8 with Input::Update (`sub_820990F0`, probably the tick), message 7 through its slot +4 with `sub_82098CE8` (not seen in play). Then binds the pads (`LogicalPad::Bind`): logical pad `i` (0-3) its binding 0 to physical pad `i`, and the "any pad" (4, +792) its bindings 0-3 to logical pads 0-3. Returns r3. Once at boot, from `Input::Create`. Matches what "Who reads the input manager" in `../pause-menu-fix.md` found in memory. |
| `sub_82591B38` | `LogicalPad::LogicalPad` | | r3 = the pad (196 bytes): vtable `0x8206CBEC`; the four bindings' 8 words at +4 to +32 = 0 (each binding an enabled byte at +4 + 8i and a device at +8 + 8i). Leaf; the values (+36) aren't touched. Five calls at boot, from `InputManager::InputManager`. |
| `sub_82591B68` | `PhysicalPad::PhysicalPad` | | r3 = the pad (212 bytes): vtable `0x8206CBF4`, +4 = 0, its two 12-byte records at +168 and +180 cleared (`sub_825A18D8`, `model-animation.md`: +0 = 0.0, +4, +8 = 0). Returns r3. Four calls at boot, from `InputManager::InputManager`. |
| `sub_8258CE60` | `InputProcessor::InputProcessor` | | r3 = the processor (416 bytes, 16-byte aligned): the base's constructor (`InputProcessorBase::InputProcessorBase`), then vtable `0x8206C0C4` (whose slot 1 is the update, `sub_82592188`, as the base's). Returns r3. From `InputManager::InputManager` (its five processors) and `ActionMapper::ActionMapper` (the mapper's listener at +2464, `action-mappers.md`): nine calls at boot. |
| `sub_825920D0` | `InputProcessorBase::InputProcessorBase` | | r3 = the processor: vtable `0x8206CC00` (slot 0 `sub_825925A8`, slot 1 the update `sub_82592188`); the dead zones +16, +20 = 0.5 (`0x82001BE4`) and +24, +28 = 0.0 (`0x82000A34`); the w of the six vectors at +32 to +112 (+44, +60, +76, +92, +108, +124) = 1.0 (`0x82000A30`); pressed, released and held (+264, +268, +272) = 0; the repeat delay +276 = 20 and period +280 = 4; the 32 hold counters (+284, 128 bytes) cleared with `memset` (`sub_825A71D0`, `c-runtime.md`). Returns r3. The values at +32 to +263 otherwise aren't set (the first update writes them). Only caller `InputProcessor::InputProcessor`. |
| `sub_825918F0` | `LogicalPad::Bind` | | r3 = the logical pad, r4 = a binding index's address (0-3), r5 = a device (a physical pad or another logical pad): the binding's enabled byte (+4 + 8i) = 1 and its device (+8 + 8i) = r5. Leaf. Eight calls at boot, from `InputManager::InputManager`. |
| `sub_82098CB8` | `InputManager::GetProcessor` | | Slot 1 of the manager's vtable `0x82000F48`: r3 = the manager, r4 = an index's address: returns processor `i` (manager + 1840 + 416i). Leaf. Reached through the vtable: the first tick handler `sub_82093DC0` (`functions.md`) gets processor 0 with it, every pass. |
| `sub_82098CD0` | `InputManager::GetLogicalPad` | | Slot 2: r3 = the manager, r4 = an index's address: returns logical pad `i` (manager + 8 + 196i). Leaf. Reached through the vtable: `ActionMapper::Bind` (`action-mappers.md`) gets its player's pad with it, then the pad's values (its slot 1, `LogicalPad::Values`) into mapper +2880. |
| `sub_82099430` | `InputManager::AddListener` | | Slot 3: r3 = the manager, r4 = a pad index's address `i`, r5 = a listener (an input processor): when pad `i`'s list (manager + 3920 + 12i) isn't empty, its last listener is reset first (`sub_82592400`, not seen in play: each list gets one listener); the listener pushed at the end (`PtrList::Insert` before the head node) and processor `i` copied into it (`InputProcessor::Assign`). Reached through the vtable: `ActionMapper::Bind` (`action-mappers.md`), once a mapper at boot. |
| `sub_82098FE0` | `InputManager::GetPhysicalPad` | | Slot 5: r3 = the manager, r4 = an index's address: returns physical pad `i` (manager + 988 + 212i). Leaf. Reached through the vtable; its callers in play aren't known (none direct). |
| `sub_82098D60` | `InputProcessor::Assign` | | r3 = the destination processor, r4 = the source: copies everything from +16 to +411 (the dead zones, the values and normalized values, the stick directions, lengths, the 32 values, the button words, the repeat delay and period and the 32 counters); +0 to +15 (the vtable) aren't touched. Leaf. From `InputManager::AddListener` (in play) and `sub_82099048` (slot 4, not seen in play). |
| `sub_820AD058` | `PtrList::PtrList` | | The C++ library's `std::list` of pointers (12-byte nodes: +0 next, +4 previous, +8 the value; the list +4 its head node, +8 its size), as instantiated across the game: r3 = the list: a head node (`PtrList::AllocateNodes` with 1) linked to itself, at +4; the size +8 = 0. Returns r3. Shared: 9 call sites, from `InputManager::InputManager` (the five listener lists), `AudioManager::AudioManager` (two, `sound-cues.md`), `sub_820AD118` and `sub_820ADBD8` (two each), `sub_82094EE0` and `sub_82094F30` (not read). |
| `sub_82099C08` | `PtrList::AllocateNodes` | | r3 = a count `n`, r4 unused: `n` × 12 bytes from `sub_825A6880` (operator new, `heap.md`), returned. When `n` is too large for 32 bits (`0xFFFFFFFF / n` below 12) it first builds a `bad_alloc` on the stack (vtables `0x82000CCC`, `0x82000CC0`) and calls `sub_82091200` (its copy, `std-string.md`'s never-ran list): a path that never ran. From `PtrList::PtrList`, `PtrList::Insert` and `sub_8209A0E8` (not read). |
| `sub_82095958` | `PtrList::Insert` | | r3 = the list, r4 = an iterator passed by value (8 bytes: its node in the low word, `where`), r5 = the value's address: a node (`PtrList::AllocateNodes` with 1) with +0 = `where`, +4 = `where`'s previous and +8 = the value; the size up by one (`PtrList::IncSize`), then linked in before `where` (`where`'s previous and that node's next = the new node). Shared: 9 callers, 6 seen in play: `InputManager::AddListener` (the listener at the end of its pad's list), `AudioManager::PlayCue` (`sound-cues.md`), `sub_82095AF0` (`functions.md`), `sub_820ADA08`, `sub_820ADC60` and `sub_82095A28` (not read); `sub_82119690`, `sub_820959D0` and `sub_82095DF8` not seen in play. |
| `sub_820958B8` | `PtrList::IncSize` | | `std::list`'s `_Incsize`: r3 = the list, r4 = `n`: when `0x3FFFFFFF` minus the size (+8) is below `n`, makes the "list<T> too long" `std::string` (`sub_825931D0`, the C string at `0x82000D2C`) and throws `length_error` (`sub_820919B8`, `sub_82091200`, `sub_820915F8`, `sub_82091328`): a path that never ran. Then the size += `n`. Only caller in play `PtrList::Insert`. Was in the C++ containers' lead (`../loops/leads.md`). |
| `sub_820990F0` | `Input::Update` | Overridden | Input manager update; skipped on render-only passes so presses aren't seen several times. Native (`src/hooks/input_update.cpp`): reads the physical pads and builds the logical pads in plain C++, then updates the processors and listeners (`sub_82592188` natively for their class, any other object called). See B1 in `docs/rexglue-removal.md`. B1's native code. The manager's tick handler: registered for message 5 by `InputManager::InputManager` (a functor with the manager), so r3 = the manager (2026-10-24). |
| `sub_82591C18` | `PhysicalPad::Read` | Native | Reads one pad (r3 = pad, r4 = pointer to the user index), once a tick per pad from Input::Update; see "Physical pad" in `docs/pause-menu-fix.md`. Plain C++ (`src/input/physical_pad.cpp`) calling our XInput directly, called by our Input::Update; same bytes as the recompiled read. No longer called. B1's native code. |
| `sub_825BA3F8` | `XInputGetState` | Native | Wrapper (r3 = user, r4 = state) jumping to `XamInputGetState`. No longer called: its only caller (`sub_82591C18`) is native and calls our own XInput (`src/input/xinput.cpp`, plain C++, reading `src/input/gamepads.cpp`) directly, with the SDK's struct, codes and mapping; the SDK's input system isn't created. See B1 in `docs/rexglue-removal.md`. B1's native code. |
| `sub_825BA3F0` | `XInputGetCapabilities` | Native | Wrapper (r3 = user, r4 = flags, r5 = capabilities) for `XamInputGetCapabilities`; no longer called (see above). Ours: fixed capabilities (a wired gamepad, every button but the guide). B1's native code. |
| `sub_825BA408` | `XInputSetState` | Native | Wrapper (r3 = user, r4 = vibration) for `XamInputSetState`; no longer called (see above). Ours: rumble through SDL, held until the next call. B1's native code. |
| `sub_82591918` | `LogicalPad::Build` | Native | Sums a logical pad's bound devices' values (vtable slot 1) and clamps them; see "Logical pad" in `docs/pause-menu-fix.md`. Plain C++ (`src/input/logical_pad.cpp`), called by our Input::Update; same bytes as the recompiled build. No longer called. B1's native code. Its bindings are set at boot by `LogicalPad::Bind` (2026-10-24). |
| `sub_820988B8` | `PhysicalPad::Values` | Wrapped | The input processors' button tests, and the pads' values (`sub_82591B30` a logical pad's, `sub_820988B8` a physical pad's): logged by caller and pass type for `--log_input_reads` (`ITC_INPUT_READ`, `ITC_INPUT_VALUES`). Seen reading (a page-protection read watch, 2026-10-19; see "Who reads the input manager" in `docs/pause-menu-fix.md`): `sub_8258CEB0` (→ `sub_82592500`, inlined in our build) tests processor 0's pressed word (manager +2104) for `sub_82093DC0`, at boot and in the front end; `sub_8258CF10` the listeners' held word (+272) for the mappers; `sub_8258CEE0` (→ `sub_82592520`: a held bit on its first tick or repeating) processor 0's held word, counters and delay, for `sub_82093DC0`. Read (2026-10-24): slot 1 of the physical pad's vtable `0x8206CBF4`: returns r3 + 8 (the pad's 40 values). Leaf. Done natively for this class by our Input::Update (`kPhysicalPadValues` in `src/hooks/input_update.cpp`). |
| `sub_82591B30` | `LogicalPad::Values` | Wrapped | The input processors' button tests, and the pads' values (`sub_82591B30` a logical pad's, `sub_820988B8` a physical pad's): logged by caller and pass type for `--log_input_reads` (`ITC_INPUT_READ`, `ITC_INPUT_VALUES`). Seen reading (a page-protection read watch, 2026-10-19; see "Who reads the input manager" in `docs/pause-menu-fix.md`): `sub_8258CEB0` (→ `sub_82592500`, inlined in our build) tests processor 0's pressed word (manager +2104) for `sub_82093DC0`, at boot and in the front end; `sub_8258CF10` the listeners' held word (+272) for the mappers; `sub_8258CEE0` (→ `sub_82592520`: a held bit on its first tick or repeating) processor 0's held word, counters and delay, for `sub_82093DC0`. Read (2026-10-24): slot 1 of the logical pad's vtable `0x8206CBEC`: returns r3 + 36 (the pad's 40 values). Leaf. Done natively for this class by our Input::Update (`kLogicalPadValues` in `src/hooks/input_update.cpp`); called through the vtable by `ActionMapper::Bind` (`action-mappers.md`), once a mapper at boot. |
| `sub_82592188` | `InputProcessor::Update` | Overridden | Slot 1 of vtable `0x8206C0C4` (the processors and the listeners' first objects): dead zones, stick lengths and directions, button masks and hold-to-repeat counters; see "The input processors" in `docs/pause-menu-fix.md`. Plain C++ (`src/input/input_processor.cpp`), called directly by our Input::Update; the override is the adapter for any recompiled caller (`src/hooks/input_update.cpp`). B1's native code. The base class's slot 1 too (`0x8206CC00`, `InputProcessorBase::InputProcessorBase`), read from the image (2026-10-24). |
| `sub_8258CEB0` | `InputProcessor::TestPressed` | Wrapped | The input processors' button tests, and the pads' values (`sub_82591B30` a logical pad's, `sub_820988B8` a physical pad's): logged by caller and pass type for `--log_input_reads` (`ITC_INPUT_READ`, `ITC_INPUT_VALUES`). Seen reading (a page-protection read watch, 2026-10-19; see "Who reads the input manager" in `docs/pause-menu-fix.md`): `sub_8258CEB0` (→ `sub_82592500`, inlined in our build) tests processor 0's pressed word (manager +2104) for `sub_82093DC0`, at boot and in the front end; `sub_8258CF10` the listeners' held word (+272) for the mappers; `sub_8258CEE0` (→ `sub_82592520`: a held bit on its first tick or repeating) processor 0's held word, counters and delay, for `sub_82093DC0`. Read (2026-10-24): r3 = the processor, r4 = a pointer to the mask: copies the mask to the stack and calls `InputProcessor::AnyPressed`. From `sub_82093DC0` (processor 0, from the manager's slot 1). |
| `sub_82592500` | `InputProcessor::AnyPressed` | Wrapped | The input processors' button tests, and the pads' values (`sub_82591B30` a logical pad's, `sub_820988B8` a physical pad's): logged by caller and pass type for `--log_input_reads` (`ITC_INPUT_READ`, `ITC_INPUT_VALUES`). Seen reading (a page-protection read watch, 2026-10-19; see "Who reads the input manager" in `docs/pause-menu-fix.md`): `sub_8258CEB0` (→ `sub_82592500`, inlined in our build) tests processor 0's pressed word (manager +2104) for `sub_82093DC0`, at boot and in the front end; `sub_8258CF10` the listeners' held word (+272) for the mappers; `sub_8258CEE0` (→ `sub_82592520`: a held bit on its first tick or repeating) processor 0's held word, counters and delay, for `sub_82093DC0`. Read (2026-10-24): r3 = the processor, r4 = a pointer to the mask: true when any bit of the mask is set in the pressed word (+264). Leaf. Only caller `InputProcessor::TestPressed`. |
| `sub_8258CEE0` | `InputProcessor::TestRepeat` | Wrapped | The input processors' button tests, and the pads' values (`sub_82591B30` a logical pad's, `sub_820988B8` a physical pad's): logged by caller and pass type for `--log_input_reads` (`ITC_INPUT_READ`, `ITC_INPUT_VALUES`). Seen reading (a page-protection read watch, 2026-10-19; see "Who reads the input manager" in `docs/pause-menu-fix.md`): `sub_8258CEB0` (→ `sub_82592500`, inlined in our build) tests processor 0's pressed word (manager +2104) for `sub_82093DC0`, at boot and in the front end; `sub_8258CF10` the listeners' held word (+272) for the mappers; `sub_8258CEE0` (→ `sub_82592520`: a held bit on its first tick or repeating) processor 0's held word, counters and delay, for `sub_82093DC0`. Read (2026-10-24): as `InputProcessor::TestPressed`, calling `InputProcessor::PressedOrRepeat`. From `sub_82093DC0`. |
| `sub_82592520` | `InputProcessor::PressedOrRepeat` | Wrapped | The input processors' button tests, and the pads' values (`sub_82591B30` a logical pad's, `sub_820988B8` a physical pad's): logged by caller and pass type for `--log_input_reads` (`ITC_INPUT_READ`, `ITC_INPUT_VALUES`). Seen reading (a page-protection read watch, 2026-10-19; see "Who reads the input manager" in `docs/pause-menu-fix.md`): `sub_8258CEB0` (→ `sub_82592500`, inlined in our build) tests processor 0's pressed word (manager +2104) for `sub_82093DC0`, at boot and in the front end; `sub_8258CF10` the listeners' held word (+272) for the mappers; `sub_8258CEE0` (→ `sub_82592520`: a held bit on its first tick or repeating) processor 0's held word, counters and delay, for `sub_82093DC0`. Read (2026-10-24): r3 = the processor, r4 = a pointer to the mask: for the mask's lowest set bit `i` (false when none), false when held bit `i` (+272) is clear; otherwise counter `i` (+284 + 4i) against the repeat delay (+276): true when it equals the delay, or, below it, when it's 1 (the first tick held). Leaf. Only caller `InputProcessor::TestRepeat`. |
