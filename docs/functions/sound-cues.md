# Known functions: sound cues and 3D emitters

The game's sound manager and its use of XACT, the Xbox 360's sound
library linked into the game (recompiled guest code, like its XAudio;
see "How the game's audio works" in `../native-audio.md`). Three layers:

- **The game's own sound manager** (`82117348`-`82119B38`): an object
  of 1,624 bytes (`AudioManager`, made by `sub_82119B38` from the
  game's component factory `sub_82094590`) that loads the sound bank
  pairs by index (a wave bank and a sound bank each, loaded through
  `Archive::Load` with callbacks that create the XACT banks), plays cues
  as `Sound` objects (24 bytes, with a 3D emitter each) kept in a list,
  updates and removes them each frame, stops them by bank and cue,
  releases banks, and forwards the listener and the categories' volume
  and pause to the layer below. The game reaches it through its
  vtables (no direct callers of its playing, stopping and bank
  methods were found).
- **The game's thin wrapper over XACT and X3DAudio** (`8259C9B0`-
  `8259D588`, the "audio engine" at manager + 16, the 300-byte emitters,
  the bank holders and the listener): each function a few calls into
  XACT's engine, sound bank, wave bank or cue through their vtables, or
  X3DAudio, with a debug print (`sub_825A1318`, a source line number)
  and a deliberate write to address 0 when XACT fails. The XACT method
  each vtable slot is, is a guess ("probably") from the arguments and
  the Windows XACT3 interfaces; the 360's differ in order (the
  engine's slots +20 and +24 create the sound and wave banks, +40 to
  +52 are the categories', +16 the per-frame work).
- **The libraries:** XACT itself (the engine object, made once by
  `XACT3CreateEngine`, `sub_825BF5C8`, and everything its vtables reach
  at about `825BF000`-`825D5000`), X3DAudio (`82480710`-`82481A00`)
  and XAudio (`sub_8245A760`, which XACT's start-up also calls). Only
  the entry points the wrapper calls directly have rows here; the rest
  of XACT and X3DAudio seen in play (about 250 and 40 functions, mostly
  reached through vtables) is a lead in `../loops/leads.md`.

The game's 3D path (a `Sound` with its 3D byte +8 set, which runs
X3DAudio's calculation through `sub_8259D160` and hands the result to
the cue) never ran in play: every cue played went through
`sub_82119638`, which plays it with the 3D byte off. The engine's
start-up (`sub_8259D1F0`: `XACT3CreateEngine` into the audio engine's
+8, the global settings file `TXR_XACT.xgs` read whole into +0 and +4,
the engine's Initialize, slot +8, with those and the notification
callback `0x8259D1C0`, then the same DSP settings as `sub_8259CE10`)
and the settings file's load (`sub_82118638`, which the manager's
constructor registers) stay in [`files-and-saves.md`](files-and-saves.md).
Shared with other files, which keep their rows: `Archive::Load`
(`sub_8209DE48`) and those two (`files-and-saves.md`); `sub_82106540`
(an empty function, `blr` only, used here as the audio engine's
constructor, the sound bank holder's destructor, probably a message handler, and
`Sound`'s velocity setter; `audio.md`); `CloseHandle` (`sub_82294E20`)
and `sub_82094B10` (`functions.md`); `operator new`, `operator new[]`,
`operator delete`, `XPhysicalAlloc` and `XPhysicalFree` (`heap.md`).
The objects' fields are in `../known-data.md`.

Making it native: the game's manager and the wrapper are the game's
use of XACT; XACT, X3DAudio and XAudio are the Xbox's libraries. No
host library plays XACT's data (the banks are XACT2-era big-endian
files, and the engine sounds depend on XACT's runtime variables), so
translating the XACT code linked into the game to native C++ is
expected (the user's call, 2026-10-23), unlike the earlier "keep the
game's XACT" in `../native-audio.md`'s decisions. Until XACT is
translated, the wrapper's functions that call into it through vtables
can only be made native as adapters calling the recompiled methods.

Native so far (`--native_sound_cues`, `src/sound_cues/`, adapters in
`src/hooks/sound_cues.cpp`, harness `sound_cues`): the work that doesn't
call into XACT, bottom up. Functions whose work is a call through an XACT
vtable (the emitters' cue calls, the audio engine's bank and category
calls, `DoWork`) and their callers wait on XACT's translation.

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
| `sub_82119B38` | `AudioManager::Create` | | Allocates 1,624 bytes (`operator new`) and builds the sound manager there with `sub_821197E0` (r3, r4, r5 passed on as r4, r5, r6); returns it, or 0. From the game's component factory `sub_82094590`. |
| `sub_821197E0` | `AudioManager::AudioManager` | | r3 = the manager, r4 = its owner (kept at +12, probably the game's framework: its vtable slot +8 gives the message dispatcher the handlers below are registered with), r5 = a name (C string, into the `std::string` at +1556 by `sub_825931D0`), r6 = the bank count's address. Vtables `0x82008388`, `0x82008374`, `0x82008364` at +0, +4, +8 (the second and third are the interfaces the banks and the `Sound`s get); the audio engine at +16 (`sub_82106540`, nothing), the listener at +1456 (`sub_8259D478`), the bank pairs' vector at +1512 (count entries, zeroed; `sub_821194A0`), two lists at +1528 (the `Sound`s) and +1540 (`sub_820AD058`), +1552 = 1, a handler object at +1584 (vtables as the bank pair's, its delegate's vtable `0x8200833C`). Starts XAudio (`sub_8259CD50`) and X3DAudio (`sub_8259CE10`) on the audio engine, then registers delegates with the owner's dispatcher: the settings file's load `sub_82118638` on +1584, and `sub_82106540`, `sub_82117E88`, `sub_82119118`, `sub_82119188` with the ids 7, 1, 6 and 8 (probably messages). Not read in detail. |
| `sub_82119118` | `AudioManager::Update` | | r3 = the manager: XACT's per-frame work (`sub_8259D1A8` on the audio engine), `sub_82094B10` on +1584 when +1596 is 3, the released banks freed (`sub_821184E8`, twice), every `Sound` updated (`sub_82118FB0`) and the stopped ones removed (`sub_82118C18`). No direct callers: registered by the constructor (the menu scan's "component update 82119118"). Once a frame, probably. |
| `sub_82119188` | | | r3 = the manager, r4 = an object, r5 = a message's address: for id 6, the object's vtable slots +32, +28, +24 and +20 with 7, 1, 6 and 8 (probably subscribing to those messages); for id 5, `sub_82119018`, the manager's slot +92 and `sub_82118930` (never ran in play). Registered by the constructor. Not read in detail. |
| `sub_82118720` | | | Makes a 16-byte copy of a member-function delegate (`operator new`; vtable `0x8200833C`, +4 the object, +8 and +12 the member pointer): probably the delegate's clone, used by the manager's handler registration. No direct callers (a vtable). Not read in detail. |
| `sub_821180B8` | `AudioManager::LoadBank` | | r3 = the manager, r4 = the bank index's address, r5 = a name, r6 = a byte's address (the bank pair's +68), r7, r8 = the wave and sound bank file ids' addresses. Nothing when the slot (`[+1516 + 4 × index]`, range-checked with a trap) is already filled; otherwise a 112-byte bank pair (`sub_82117D08`, owner the manager + 4) into it. No direct callers (a vtable). |
| `sub_82118190` | `AudioManager::ReleaseBank` | Overridden | r3 = the manager, r4 = the bank index's address: marks the slot's bank pair for release (`sub_82117760`) when there is one. No direct callers (a vtable). Where the original traps (no vector, or the index past its end) the adapter runs the original. Native in `src/sound_cues/sound_cues.cpp` (`ManagerReleaseBank`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its banks group); runs in attract races. |
| `sub_82118200` | `AudioManager::IsBankLoading` | Overridden | r3 = the manager, r4 = the bank index's address: 0 for an empty slot, else `sub_82117670` (1 until both banks are made). No direct callers (a vtable). Where the original traps (no vector, or the index past its end) the adapter runs the original. Native in `src/sound_cues/sound_cues.cpp` (`ManagerIsBankLoading`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its banks group); runs in attract races. |
| `sub_821182E0` | `AudioManager::IsBankSlotEmpty` | Overridden | r3 = the manager, r4 = the bank index's address: 1 when the slot holds no bank pair. No direct callers (a vtable). Where the original traps (no vector, or the index past its end) the adapter runs the original. Native in `src/sound_cues/sound_cues.cpp` (`ManagerIsBankSlotEmpty`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its banks group); not seen in the attract race (the harness checks it). |
| `sub_82118400` | `AudioManager::GetSoundBank` | Overridden | On the manager's interface at +8 (r3 = manager + 8, so its +1508 is the manager's bank vector): r4 = the bank index's address; returns the slot's sound bank holder (`sub_82117770`, the pair's +76). The `Sound`s' owner vtable slot +4. No direct callers. Where the original traps (no vector, or the index past its end) the adapter runs the original. Native in `src/sound_cues/sound_cues.cpp` (`ManagerGetSoundBank`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its banks group); runs in attract races. |
| `sub_82117EA8` | `AudioManager::GetEngine` | Overridden | Returns r3 + 12: on the manager's interface at +4, the audio engine at the manager's +16. The bank pairs' owner vtable slot +12, from the bank callbacks (`sub_82117778` and the three after it). Native in `src/sound_cues/sound_cues.cpp` (`ManagerGetEngine`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its listener group); runs in attract races. |
| `sub_82117E08` | `AudioManager::SetListenerPosition` | Overridden | r3 + 1456 (the listener), then `sub_8259D588`. A vtable thunk; no direct callers. Native in `src/sound_cues/sound_cues.cpp` (`ManagerSetListenerPosition`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its listener group); runs in attract races. |
| `sub_82117E10` | `AudioManager::SetListenerOrientation` | Overridden | r3 + 1456 (the listener), then `sub_8259D550`. A vtable thunk; no direct callers. Native in `src/sound_cues/sound_cues.cpp` (`ManagerSetListenerOrientation`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its listener group); runs in attract races. |
| `sub_82117E18` | `AudioManager::SetListenerVelocity` | Overridden | r3 + 1456 (the listener), then `sub_8259CC80` (its +36). A vtable thunk; no direct callers. Native in `src/sound_cues/sound_cues.cpp` (`ManagerSetListenerVelocity`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its listener group); runs in attract races. |
| `sub_82117E68` | `AudioManager::GetCategory` | | r3 + 16 (the audio engine), then `sub_8259D0C8`. A vtable thunk; no direct callers. |
| `sub_82117E70` | `AudioManager::SetCategoryVolume` | | r3 + 16, then `sub_8259D100`. A vtable thunk; no direct callers. |
| `sub_82117E78` | `AudioManager::PauseCategory` | | r3 + 16, then `sub_8259D120`. A vtable thunk; no direct callers. |
| `sub_82117E80` | `AudioManager::ResumeCategory` | | r3 + 16, then `sub_8259D140`. A vtable thunk; no direct callers. |
| `sub_82117E88` | `AudioManager::DoWork` | | r3 + 16, then `sub_8259D1A8` (XACT's per-frame work). Registered by the constructor with id 1; no direct callers. |
| `sub_82119638` | `AudioManager::PlayCue2D` | | r3 = the manager, r4, r5 = the bank and cue indexes' addresses, r6 = passed on as the velocity (unused): `sub_82119500` with the position (0, 0, 0, 1) and the 3D byte 0. Every cue played in play went through it. No direct callers (a vtable). |
| `sub_82119500` | `AudioManager::PlayCue` | | r3 = the manager, r4, r5 = the bank and cue indexes' addresses, r6 = a position, r7 = a velocity (given to `sub_82106540`, which does nothing), r8 = the 3D byte's address. Nothing for a negative index or an empty bank slot; otherwise a 24-byte `Sound` (`sub_82117348`, owner the manager + 8), its 3D byte (`sub_82117568`) and position (`sub_82117560`) set, one update (`sub_82117438`), its start asked for (`sub_821173F0`), and pushed on the list at +1528 (`sub_82095958`). From `sub_82119638` and `sub_82119608` (never ran). |
| `sub_82118A90` | `AudioManager::StopCue` | | r3 = the manager, r4, r5 = the bank and cue indexes' addresses: stops (`sub_82117400`) every `Sound` in the list at +1528 playing that cue of that bank (`sub_821175B8`, `sub_82117578`). No direct callers (a vtable). |
| `sub_82118B58` | `AudioManager::IsCuePlaying` | | r3 = the manager, r4, r5 = the bank and cue indexes' addresses: 1 when a `Sound` for that cue of that bank is in the list (stopped ones are removed each update). No direct callers (a vtable). |
| `sub_82118FB0` | `AudioManager::UpdateSounds` | Wrapped | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. r3 = the manager: `Sound::Update` (`sub_82117438`, its address built in) on every `Sound` in the list at +1528, through `sub_82118F50`. From `AudioManager::Update`. |
| `sub_82118F50` | | | `std::for_each` over a list: r4, r5 = the first and end iterators (spilled pairs), r6 = a function called with each element (+8 of the node); returns the function in r3's word. From `sub_82118FB0` and `sub_821190B0` (never ran). |
| `sub_82118C18` | `AudioManager::RemoveStoppedSounds` | Wrapped | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. r3 = the manager: removes from the list at +1528 every `Sound` for which `sub_821185C8` (a member-function pointer, its address built in) gives 1, through `sub_82118850`. From `AudioManager::Update`. |
| `sub_82118850` | | | A list's `remove_if` with a member-function predicate: r3 = the list, r4, r5 = the predicate (function, this adjustment): erases each element (+8 of the node) it accepts (`sub_82098E80`). From `sub_82118C18`. |
| `sub_821185C8` | `AudioManager::DeleteIfStopped` | | r4 = a `Sound`: when its cue has stopped (`sub_82117408`), deletes it (`sub_82118490`) and returns 1; else 0. The predicate of `sub_82118C18`. |
| `sub_821184E8` | `AudioManager::FreeReleasedBanks` | Wrapped | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. r3 = the manager: for each bank pair in the vector at +1516 marked for release and no longer in use (`sub_821176E8`), deletes it (`sub_82118438`). From `AudioManager::Update` (twice in a row). |
| `sub_82118438` | | | r3 = a slot holding a bank pair: when not null, its destructor (`sub_82117B88`), `operator delete`, and the slot cleared. From `sub_821184E8` and `sub_82118930` (never ran). |
| `sub_82118490` | | | r3 = a slot holding a `Sound`: when not null, its destructor (`sub_82117630`), `operator delete`, and the slot cleared. From `sub_821185C8` and others never seen. |
| `sub_821193C0` | | | Probably a `std::vector` of words' allocation: r3 = the vector, r4 = a count: +4, +8, +12 cleared; 0 for no count; above 0x3FFFFFFF `sub_820A92D0` (probably the length error; never ran); otherwise `sub_82092B78(count, 0)` into +4 and +8, +12 = the end of the capacity; returns 1. From `sub_821194A0`. |
| `sub_821194A0` | | | Probably a `std::vector` of words made with a count of one value: r3 = the vector, r4 = the count, r5 = the value's address (`sub_821193C0`, then filled; +8 = the end). From the manager's constructor (the bank vector). |
| `sub_82117D08` | `BankPair::BankPair` | | r3 = the pair (112 bytes), r4 = a name (into the `std::string` at +40), r5 = a byte's address (+68: 1 skips the wave bank's load, probably one loaded elsewhere), r6 = its owner (+36, the manager + 4), r7 = the bank index (+80), r8 = the wave bank's file id (+84), r9 = the sound bank's (+88). Vtables `0x820066DC` (+0), `0x82002908` (+4), `0x82002850` (+24); +8, +12, +28, +32, +69, +72, +76 cleared, bytes +16 0, +17 1, +18 0. Then loads it (`sub_821178C8`). From `AudioManager::LoadBank`. |
| `sub_821178C8` | `BankPair::Load` | | r3 = the pair: clears the loaded bytes +108, +109; makes the wave bank holder (28 bytes, `sub_8259D418`) at +72 and the sound bank holder (12 bytes, `sub_8259D3D8`) at +76. For index 2 the holders' buffers are `[0x826C5C88]` and `[0x826C5C8C]` (kept), and the files are loaded into them (`Archive::Load` with the callbacks `sub_82117828` and `sub_82117878`); for the others, unless +68, the wave bank's size (`sub_820A0678`) into +92 and a buffer of it rounded up to 2 KB from `XPhysicalAlloc` into +100, loaded with `sub_82117778`; then the sound bank's size into +96, a buffer from `operator new[]` into +104, loaded with `sub_821177D0`. Each load's context is the pair. Inside a stack object made by `sub_820A6A00` and released by `sub_820A6A40` (probably the file layer's). From `BankPair::BankPair`. |
| `sub_82117778` | | | The wave bank's load callback (r6 = the pair): the audio engine from the owner (`[+36]`, slot +12), then `sub_8259CE78` (the holder +72, the buffer +100, the size +92) and +108 = 1. Reached through `Archive::Load`. |
| `sub_821177D0` | | | The sound bank's load callback (r6 = the pair): `sub_8259CF10` (the holder +76, the buffer +104, the size +96) and +109 = 1. Reached through `Archive::Load`. |
| `sub_82117828` | | | Bank 2's wave bank load callback (r6 = the pair): `sub_8259CFA8` (the holder +72, the size +92) and +108 = 1. Reached through `Archive::Load`. |
| `sub_82117878` | | | Bank 2's sound bank load callback (r6 = the pair): `sub_8259D038` (the holder +76, the size +96) and +109 = 1. Reached through `Archive::Load`. |
| `sub_82117670` | `BankPair::IsLoading` | Native | r3 = the pair: 0 when both loaded bytes (+108, +109) are set, else 1. From `sub_82117EB8` (never ran) and `AudioManager::IsBankLoading`. Native in `src/sound_cues/sound_cues.cpp` (`BankPairIsLoading`), harness `sound_cues` (its banks group); no adapter: its native callers `AudioManager::IsBankLoading` (`sub_82118200`; `sub_82117EB8`, which never ran, calls the original) call it directly, and `--native_sound_cues` now switches only them; runs in attract races. |
| `sub_82117760` | `BankPair::Release` | Native | Sets byte +69 (release asked for). From `AudioManager::ReleaseBank`. Native in `src/sound_cues/sound_cues.cpp` (`BankPairRelease`), harness `sound_cues` (its banks group); no adapter: its native callers `AudioManager::ReleaseBank` (`sub_82118190`) call it directly, and `--native_sound_cues` now switches only them; runs in attract races. |
| `sub_82117770` | `BankPair::SoundBank` | Native | Returns +76 (the sound bank holder). From `AudioManager::GetSoundBank`. Native in `src/sound_cues/sound_cues.cpp` (`BankPairSoundBank`), harness `sound_cues` (its banks group); no adapter: its native callers `AudioManager::GetSoundBank` (`sub_82118400`) call it directly, and `--native_sound_cues` now switches only them; runs in attract races. |
| `sub_821176E8` | `BankPair::CanFree` | | r3 = the pair: 1 when release was asked for (+69) and the sound bank (holder +76, its +8) exists and its slot +20 (probably GetState) succeeds with bit 0 clear (probably not in use); else 0. From `sub_821184E8`, `sub_82118320` and `sub_82118930` (never ran). |
| `sub_82117B88` | `BankPair::~BankPair` | | r3 = the pair: the wave bank (holder +72, its +8) destroyed (slot +0, probably Destroy) and the sound bank (holder +76, its +8) (slot +16, probably Destroy); except for bank 2, the wave bank's buffer freed (`XPhysicalFree`) and the sound bank's (`operator delete`); the holders deleted (`sub_82117AD8`, `sub_82117B30`), the name freed (`sub_82091328(+40, 1, 0)`) and the base vtables put back. From `sub_82118438`. |
| `sub_82117AD8` | | | r3 = a slot holding a wave bank holder: when not null, `sub_8259D438` (closes its handle), `operator delete`, and the slot cleared. From `BankPair::~BankPair`. |
| `sub_82117B30` | | | r3 = a slot holding a sound bank holder: when not null, `sub_82106540` (nothing), `operator delete`, and the slot cleared. From `BankPair::~BankPair` and `sub_821772D8` (twice; not in this file, not read). |
| `sub_82117348` | `Sound::Sound` | | r3 = the `Sound` (24 bytes), r4, r5 = the bank and cue indexes' addresses (into +16, +20), r6 = its owner (+0, the manager + 8). A 300-byte emitter (`sub_8259C9B0`) at +4; bytes +8 (3D), +9 (loop), +10 (was playing), +11 (start asked for) and +12 cleared. Then the owner's sound bank for the index (slot +4, `AudioManager::GetSoundBank`) and, when there is one, the cue prepared in the emitter (`sub_8259CB48`). From `AudioManager::PlayCue` and `sub_82119690` (never ran). |
| `sub_82117438` | `Sound::Update` | | r3 = the `Sound`. When a start was asked for (+11) and the cue is prepared (`sub_8259CCF8`), +11 cleared and the cue played (`sub_8259CC38`). With the 3D byte (+8) and the cue not stopped (`sub_8259CCA0`), X3DAudio's calculation on it (`sub_8259D160`, with the owner's slots +8 and +12: the engine's 3D state and the listener; never ran in play). Then +10 = 1 while the cue isn't stopped; once stopped, with +10 and the loop byte (+9) set, the cue is destroyed, prepared again and played (a loop), and +10 cleared. From `AudioManager::PlayCue` and, for every `Sound`, `AudioManager::UpdateSounds`. |
| `sub_821173F0` | `Sound::Start` | Overridden | Sets byte +11 (start asked for). From `AudioManager::PlayCue` and `sub_82118EA0` (never ran). Native in `src/sound_cues/sound_cues.cpp` (`SoundStart`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its sounds group); runs in attract races. |
| `sub_82117400` | `Sound::Stop` | | `sub_8259CC58` on the emitter (+4): the cue stopped at once. From `AudioManager::StopCue` and `sub_821189F0`, `sub_82118ED0` (never ran). |
| `sub_82117408` | `Sound::IsStopped` | | 1 when the emitter's (+4) cue exists and has stopped (the inverse of `sub_8259CCA0`). From `AudioManager::DeleteIfStopped`. |
| `sub_82117560` | `Sound::SetPosition` | Overridden | r4 = a vector: `sub_8259CC80` on the emitter (+4): its X3DAudio position. From `AudioManager::PlayCue` and others never seen. Native in `src/sound_cues/sound_cues.cpp` (`SoundSetPosition`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its sounds group); runs in attract races. |
| `sub_82117568` | `Sound::Set3D` | Overridden | Byte +8 = the byte at r4. From `AudioManager::PlayCue` and others never seen. Native in `src/sound_cues/sound_cues.cpp` (`SoundSet3D`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its sounds group); runs in attract races. |
| `sub_821175B8` | `Sound::IsBank` | Overridden | r4 = a bank index's address: 1 when it equals +16. From `AudioManager::StopCue`, `IsCuePlaying` and `sub_821189F0` (never ran). Native in `src/sound_cues/sound_cues.cpp` (`SoundIsBank`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its sounds group); runs in attract races. |
| `sub_82117578` | `Sound::IsCue` | Overridden | r4 = a cue index's address: 1 when it equals +20. From `AudioManager::StopCue` and `IsCuePlaying`. Native in `src/sound_cues/sound_cues.cpp` (`SoundIsCue`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its sounds group); runs in attract races. |
| `sub_82117630` | `Sound::~Sound` | | r3 = the `Sound`: the emitter's cue destroyed (`sub_8259CBE8`) and the emitter deleted (`sub_821175D8`). From `sub_82118490`. |
| `sub_821175D8` | | | r3 = a slot holding a 300-byte emitter: when not null, its cue destroyed (`sub_8259CBE8`), `operator delete`, and the slot cleared. From `Sound::~Sound`. |
| `sub_8259C9B0` | `Emitter::Emitter` | Overridden | r3 = the emitter (300 bytes): +0 the cue (null); an X3DAudio emitter at +8 in the 360's layout (no inner radius): +8 the cone (→ +100), front (0, 0, 1) at +12, top (0, 1, 0) at +24, position and velocity 0 at +36 and +48, one channel (+60), radius 0, the azimuths at +68 (→ +132, one 0.0), no curves (+72 to +88), distance and Doppler scalers 1 (+92, +96); the cone at +100 (0, 0, 0, 1, 0, 1, 0, 1: probably no cone). +136 to +300: a block handed to the cue with each prepare (probably the cue's 3D settings: six (index, 1.0) pairs from +164, counts and flags up to +296). Constants `0x82000A34` (probably 0.0) and `0x82000A30` (1.0). From `Sound::Sound`. Not read in detail past +136. Native in `src/sound_cues/sound_cues.cpp` (`EmitterConstruct`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its sounds group); runs in attract races. |
| `sub_8259CB48` | `Emitter::Prepare` | | r3 = the emitter, r4 = a sound bank holder, r5 = a 16-bit cue index's address: when the holder has a sound bank (+8) and the emitter no cue, the bank's slot +4 (probably Prepare: the index, flags 0, time 0, the cue into +0); on success the cue's slot +60 with the emitter's +136; returns 1, else 0. From `Sound::Sound` and `Sound::Update`. |
| `sub_8259CC38` | `Emitter::Play` | | The cue's (+0) slot +0 (probably Play), when there is a cue. From `Sound::Update`. |
| `sub_8259CC58` | `Emitter::Stop` | | The cue's slot +4 with 1 (probably Stop, at once), when there is a cue. From `Sound::Stop`. |
| `sub_8259CCA0` | `Emitter::IsNotStopped` | | The cue's slot +8 (probably GetState) into a word: 0 when bit 0x20 (probably stopped) is set, else 1; 1 with no cue. From `Sound::Update` and `Sound::IsStopped`. |
| `sub_8259CCF8` | `Emitter::IsPrepared` | | The cue's slot +8 (probably GetState): 1 when bit 4 (probably prepared) is set; 0 with no cue. From `Sound::Update`. |
| `sub_8259CBE8` | `Emitter::DestroyCue` | | The cue's slot +12 (probably Destroy) and +0 cleared, when there is a cue. From `Sound::Update`, `Sound::~Sound` and `sub_821175D8`. |
| `sub_8259CC80` | | Native | Copies the vector at r4 (three floats) into r3 + 36: an emitter's X3DAudio position, or the listener's velocity. From `Sound::SetPosition` and `AudioManager::SetListenerVelocity`. Native in `src/sound_cues/sound_cues.cpp` (`SetVector36`), harness `sound_cues` (its listener group); no adapter: its native callers `Sound::SetPosition` and `AudioManager::SetListenerVelocity` (`sub_82117560`, `sub_82117E18`) call it directly, and `--native_sound_cues` now switches only them; runs in attract races. |
| `sub_8259D478` | `Listener::Listener` | Overridden | r3 = the listener (the manager's +1456, 56 bytes): front (0, 0, 1) at +0, top (0, 1, 0) at +12, position (0, 0, `[0x82001FD8]`) at +24, velocity 0 at +36 (an X3DAudio listener), and +48 = `[0x82002A0C]`, +52 = `[0x820027A4]`. From the manager's constructor. Native in `src/sound_cues/sound_cues.cpp` (`ListenerConstruct`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its listener group); runs in attract races. |
| `sub_8259D550` | `Listener::SetOrientation` | Native | r4, r5 = the front and top vectors, into +0 and +12. From `AudioManager::SetListenerOrientation`. Native in `src/sound_cues/sound_cues.cpp` (`ListenerSetOrientation`), harness `sound_cues` (its listener group); no adapter: its native callers `AudioManager::SetListenerOrientation` (`sub_82117E10`) call it directly, and `--native_sound_cues` now switches only them; runs in attract races. |
| `sub_8259D588` | `Listener::SetPosition` | Native | r4 = a vector, into +24. From `AudioManager::SetListenerPosition`. Native in `src/sound_cues/sound_cues.cpp` (`ListenerSetPosition`), harness `sound_cues` (its listener group); no adapter: its native callers `AudioManager::SetListenerPosition` (`sub_82117E08`) call it directly, and `--native_sound_cues` now switches only them; runs in attract races. |
| `sub_8259CD50` | `AudioEngine::StartXAudio` | | Builds a parameter block on the stack (bytes 6, 1, 48 and two pointers to blocks with `0x820713E0` and `0x820713D8`; probably 6 channels, 48 kHz) and calls `sub_8245A760` (probably XAudioInitialize) with it; on failure a debug print (line 173) and a write to address 0. r3 (the audio engine) isn't used. From the manager's constructor. Not read in detail. |
| `sub_8259CE10` | `AudioEngine::Start3D` | | r3 = the audio engine: `X3DAudioInitialize` (`sub_82480710`) for the speaker mask 63 (5.1) and the speed of sound `[0x8207145C]` into the handle at +12; the DSP settings at +28: the matrix at +76 (six 1.0), one source and six destination channels (+36, +40). From the manager's constructor. |
| `sub_8259CE78` | `AudioEngine::CreateWaveBank` | | r3 = the audio engine, r4 = a wave bank holder, r5 = the buffer, r6 = its size (into the holder's +0, +4): the XACT engine's (+8) slot +24 (probably CreateInMemoryWaveBank: the buffer, size, flags 0, 0, the bank into the holder's +8); on failure a debug print (line 279) and a write to address 0. From `sub_82117778`. |
| `sub_8259CF10` | `AudioEngine::CreateSoundBank` | | As `sub_8259CE78` with the engine's slot +20 (probably CreateSoundBank; line 290), for a sound bank holder. From `sub_821177D0`. |
| `sub_8259CFA8` | | | As `sub_8259CE78` with the buffer already in the holder's +0 (r5 = the size; line 300). From `sub_82117828` (bank 2). |
| `sub_8259D038` | | | As `sub_8259CF10` with the buffer already in the holder's +0 (r5 = the size; line 308). From `sub_82117878` (bank 2). |
| `sub_8259D0C8` | `AudioEngine::GetCategory` | | The XACT engine's slot +40 (probably GetCategory, r4 = its name passed on), the result's low 16 bits. From `AudioManager::GetCategory`. |
| `sub_8259D100` | `AudioEngine::SetCategoryVolume` | | r4 = a category's address (16 bits), r5 = a float's: the engine's slot +48 (probably SetVolume). From `AudioManager::SetCategoryVolume`. |
| `sub_8259D120` | `AudioEngine::PauseCategory` | | r4 = a category's address: the engine's slot +52 (probably Pause) with 1. From `AudioManager::PauseCategory`. |
| `sub_8259D140` | `AudioEngine::ResumeCategory` | | As `sub_8259D120` with 0. From `AudioManager::ResumeCategory`. |
| `sub_8259D1A8` | `AudioEngine::DoWork` | Wrapped | Named in `--find_menu_selection`'s log (`ITC_MENU_SCAN_WRAP`): `Scene::Update`'s two screen calls and others. The XACT engine's (+8) slot +16 (probably DoWork). From `AudioManager::Update` and `AudioManager::DoWork`. |
| `sub_8259D3D8` | `SoundBankHolder::SoundBankHolder` | Overridden | Clears +0 (the buffer), +4 (its size), +8 (the XACT sound bank). From `BankPair::Load`. Native in `src/sound_cues/sound_cues.cpp` (`SoundBankHolderConstruct`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its banks group); runs in attract races. |
| `sub_8259D418` | `WaveBankHolder::WaveBankHolder` | Overridden | Clears +0 (the buffer), +4 (its size), +8 (the XACT wave bank) and sets +12 (a handle) to -1. From `BankPair::Load`. Native in `src/sound_cues/sound_cues.cpp` (`WaveBankHolderConstruct`) behind `--native_sound_cues` (adapter in `src/hooks/sound_cues.cpp`, harness `sound_cues`, its banks group); runs in attract races. |
| `sub_8259D438` | `WaveBankHolder::~WaveBankHolder` | | `CloseHandle` (`sub_82294E20`) on +12 when it isn't -1, then -1 (never set in what ran: probably a streaming wave bank's file). From `sub_82117AD8`. |
| `sub_825BF5C8` | `XACT3CreateEngine` | | XACT (library). Probably `XACT3CreateEngine`: r4 = where the engine goes; the engine is made once (196 bytes, `operator new`, `sub_825C09A8`) and kept at `[0x82971478]`, then handed out again; returns 0, or 0x8007000E (out of memory). From the engine's start-up `sub_8259D1F0` (`files-and-saves.md`). |
| `sub_825C09A8` | | | XACT (library). Probably the engine object's constructor: vtables `0x82075360` (+0) and `0x82075348` (+4), +8 = 1 (probably the reference count), +20 = -1, seven list heads (+28 to +112) pointing at themselves, a critical section (`RtlInitializeCriticalSection`). From `sub_825BF5C8`. Not read in detail. |
| `sub_8245A760` | `XAudioInitialize` | | XAudio (library). Probably `XAudioInitialize`: `sub_8245A410` with r4 = 0. From `sub_8259CD50`, XACT's `sub_825C2BB0` and `sub_82483770`. |
| `sub_8245A410` | | | XAudio (library). r3 = the parameters, r4: under the critical section at `0x8264D518`, when the XAudio engine `[0x82954F08]` exists, its slot +4 (probably AddRef) and 0; otherwise the function at `[0x826002AC]` (probably the engine's creation) with r5 = the parameters (a zeroed 12-byte block when none) and r6 = r4, returning its result. From `sub_8245A760`. Not read in detail. |
| `sub_82480710` | `X3DAudioInitialize` | | X3DAudio (library). Probably `X3DAudioInitialize`: r3 = the speaker mask, f1 = the speed of sound, r5 = the handle: +0 the channel count (the mask's bits), +4 the LFE channel's index (-1 when bit 3 is clear, else the count of bits 0-2), +8 the speed, +12 the float just below it. From `sub_8259CE10` and `sub_82110258` (another sound user). |
