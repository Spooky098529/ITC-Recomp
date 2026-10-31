# Known functions: the menu sounds and the sound manager's interface

The small helpers the game's screens play their sounds and set the
sound options through, and the rest of the sound manager's interface
they reach. The object at `[0x826C5C80]` (kept by `SoundGlobals::Init`,
`car-sounds.md`, and called "the music player" there and in
`mode-tick.md`) is the game's sound manager itself: the app's
interface's slot +48 answers app + 124, the `AudioManager` that
`AudioManager::Create` made (`sound-cues.md`), so its vtable is
`0x82008388`, the manager's first (the sound manager's methods and
fields are described in `sound-cues.md`'s intro and
`../known-data.md`). `Music::Play` (`mode-tick.md`) calls its slot 4,
`AudioManager::LoadBank`: the music is a bank pair loaded by name
(bank pair 2, into the buffers `SoundGlobals::Init` makes).

- **The menu sound** (`sub_821103F0`): cue r3 of bank 0 played through
  the manager's slot 10 (`AudioManager::PlayCue2D`, at full volume, no
  3D). About 270 functions call it (112 seen in play), nearly all in
  the front end (`82179DF0`-`82289768`: the title screen's update in
  `functions.md`, the menus, the career's parking area in
  `location-screen.md`) and the race's screens (the race HUD's
  countdown, warning and gauges in `race-hud-object.md`); most pass cue 0, 2, 4 or 5 (probably the cursor, accept and
  back sounds). The callers are their screens' subsystems' and have no
  rows here.
- **The bank and cue sound** (`sub_82175850`): the same with the bank
  and cue indexes' addresses given, from 29 functions (15 seen in play:
  the race's sound handlers `821767A8`-`82177130`, a lead, and front-end
  screens).
- **The volume options** (`sub_821B9C20`, `sub_821B9CF0`): a 0-15
  setting turned into a gain and given to the XACT categories "Default"
  and "Music", kept in the volume globals `SoundGlobals::Init` sets to
  1.0 (`0x82616C38`, `0x82616C3C`, `0x82616BEC`, `../known-data.md`;
  `CarSound::Update3D` scales the car sounds by `0x82616C38`), from the
  options screens and the settings' load.
- **The sound manager's other methods**, the vtable's slots with no row
  in `sound-cues.md` (none seen in play): the destructor, whether any
  bank loads, a bank's index by name, the first free bank slot, whether
  a bank is loaded, a cue's index by name, the 3D play, stopping a
  bank's sounds, and a second list of sounds kept by handle (+1540,
  their counter at +1552: play one, move it, stop it, remove it, clear
  and update the list), and one listener setter.

Shared with other files, which keep their rows: the manager's slots
seen in play and their callees, in `sound-cues.md`
(`AudioManager::LoadBank` slot 4, `ReleaseBank` 5, `IsBankLoading` 6,
`IsBankSlotEmpty` 8, `PlayCue2D` 10, `StopCue` 13, `IsCuePlaying` 14,
`SetListenerPosition`, `SetListenerOrientation`,
`SetListenerVelocity` 25 to 27, `GetCategory`, `SetCategoryVolume`,
`PauseCategory`, `ResumeCategory` 29 to 32; `AudioManager::PlayCue`
`sub_82119500`, the `Sound` and `Emitter` functions under them; their
XACT methods in `xact.md`); `SoundGlobals::Init` and
`SoundGlobals::Shutdown` (`sub_82110258`, `sub_82110388`,
`car-sounds.md`); `Music::Play` (`sub_821063B0`, `mode-tick.md`);
`operator new` and `operator delete` (`heap.md`); `List::Clear`
(`sub_820ACF88`, `app-states.md`); `PtrList::Insert` and
`PtrList::Erase` (`sub_82095958`, `input.md`; `sub_82098E80`,
`app-states.md`); `std::string::_Tidy` (`sub_82091328`,
`std-string.md`); `RigidBody::SetFloat48` (`sub_8259D5A8`,
`car-physics.md`, a store folded with the listener's); and the empty
`sub_82106540` (`audio.md`). Its neighbors at `82110448`-`82111140`
aren't sound code (the car objects' makers and the career, in
`career.md`, `race-setup.md`, `save-interface.md` and a lead).

Not here: the race's sound handlers (`82175908`-`82177130`, the music
fades, the pause and resume of the car sounds and the music category,
the listener's update), with the race mode's sound set-up
`sub_82177378`, in `../loops/leads.md`; the options screens that call
the volume setters (`sub_821BBF00`, `sub_821C1498`, `sub_8220E988`,
`sub_82210E58`), in `../loops/leads.md`.

Making it native: the helpers are small and reach the manager only
through its vtable (guest calls until the manager's methods are ours);
`sub_821103F0` is ready by itself. The four helpers are native
(`src/menu_sounds/`, `--native_menu_sounds`, B7 loop 2026-10-31), their
adapters kept for their recompiled callers, the manager's slots called
through the function table until they're ours. The sound manager is the game's use
of XACT, translated (the user's call, 2026-10-23, "Precedent first" in
`../loops/b7-loop.md`).

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
| `sub_821103F0` | `MenuSound::Play` | Overridden | r3 = a cue index: plays that cue of bank 0 through the sound manager's (`[0x826C5C80]`) slot 10 (`AudioManager::PlayCue2D`, `sound-cues.md`), r4 = the bank's address (a word 0 at its frame's +88), r5 = the cue's (r3 stored at +84), r6 = a float 1.0 (`[0x82000A30]`, at +80; PlayCue2D's velocity, unused). A 112-byte frame; returns r3 as the slot leaves it. 13 instructions besides the frame. About 270 callers (112 seen in play), nearly all in the front end (`82179DF0`-`82289768`) and the race's screens (the intro); most pass 0, 2, 4 or 5 (probably the cursor, accept and back sounds; the race HUD's countdown 8 and 9, its warning 10). Seen in play. Read in full (B7 loop, 2026-10-31). Native in `src/menu_sounds/menu_sounds.cpp` (`PlayMenuSound`) behind `--native_menu_sounds` (adapter in `src/hooks/menu_sounds.cpp`; about 270 recompiled callers, so it stays; harness `menu_sounds`, a fake manager whose slots are leaf recorders); slot 10 through the function table (`PlayCue2D` `sub_82119638` isn't ours yet). Not seen in the attract race (a temporary counter, 2026-10-31): the menus' replay is the validation agent's. |
| `sub_82175850` | `Sound::Play` | Overridden | r3, r4 = the bank and cue indexes' addresses: the sound manager's slot 10 (`AudioManager::PlayCue2D`) with them as r4, r5 and r6 = a float 1.0 (`[0x82000A30]`) at its 96-byte frame's +80. Returns r3 as the slot leaves it. From 29 functions (15 seen in play): the race's sound handlers `sub_821767A8`, `sub_821769D0`, `sub_82176C10`, `sub_82176F28`, `sub_82177130` (the lead in the intro), and front-end screens (`sub_8218C9C8`, `sub_82197B00`, `sub_821BA380`, `sub_82223AB8`, `sub_82230050`, `sub_82234190`, `sub_82235F28`, `sub_82244610`, `sub_822463D8`, `sub_8227B440` and 14 not seen in play). Seen in play. Read in full (B7 loop, 2026-10-31). Native in `src/menu_sounds/menu_sounds.cpp` (`PlaySound`) behind `--native_menu_sounds` (adapter in `src/hooks/menu_sounds.cpp`; about 29 recompiled callers, so it stays; harness `menu_sounds`, a fake manager whose slots are leaf recorders); slot 10 through the function table. Runs in attract races (once, at the race's start: a temporary counter, 2026-10-31). |
| `sub_821B9C20` | `SoundOptions::SetEffectsVolume` | Overridden | r3 = a volume setting (a byte, sign-extended): the gain 0.0 (`[0x82000A34]`) for 0, 1.0 (`[0x82000A30]`) for 15, else the setting times `[0x82008060]` (1/15), at its 112-byte frame's +80. The category "Default" (`0x8200A6A4`) looked up through the sound manager's slot 29 (`AudioManager::GetCategory`) into +88; when it isn't -1, its volume set (slot 30, `AudioManager::SetCategoryVolume`, r4 = +88, r5 = +80). The gain is then stored at `0x82616C38` and `0x82616C3C` either way (the first scales the car sounds' output, `CarSound::Update3D` in `car-sounds.md`; the race's sound handlers write `0x82616C38` and read `0x82616C3C`; not read in detail). From `sub_821BBF00` (the setting at `[0x826D8478]` + 64), `sub_821C1498` (+256 and +64), `sub_8220E988` (`[0x826D84D4]` + 56 and + 20; the options screens and the settings' load, probably: the lead in the intro), `sub_82210E58` (not seen in play), each followed by `SoundOptions::SetMusicVolume` with the word before. Seen in play. Read in full (B7 loop, 2026-10-31). Native in `src/menu_sounds/menu_sounds.cpp` (`SetEffectsVolume`) behind `--native_menu_sounds` (adapter in `src/hooks/menu_sounds.cpp`, its callers recompiled; harness `menu_sounds`, a fake manager whose slots are leaf recorders); slots 29 and 30 through the function table (`AudioManager::GetCategory` and `SetCategoryVolume`, `sound-cues.md`, and XACT's `GetCategory` and `SetVolume` under them, `xact.md`, aren't ours yet), only r3 and r4 set for slot 29 and r3 to r5 for slot 30 as the original; the doubleword at +88 left as its `std` leaves it (its low word under the category's store). Not seen in the attract race (a temporary counter, 2026-10-31). |
| `sub_821B9CF0` | `SoundOptions::SetMusicVolume` | Overridden | The same as `SoundOptions::SetEffectsVolume` for the category "Music" (`0x82008348`), the gain stored at `0x82616BEC` (the music's current volume, which the race's sound handlers fade out and put back from `0x82616C34`; not read in detail). From the same four callers, with the settings at +60, +252, +52 and +16. Seen in play. Read in full (B7 loop, 2026-10-31). Native in `src/menu_sounds/menu_sounds.cpp` (`SetMusicVolume`, the same steps as `SetEffectsVolume`) behind `--native_menu_sounds` (adapter in `src/hooks/menu_sounds.cpp`; harness `menu_sounds`). Not seen in the attract race (a temporary counter, 2026-10-31). |
| `sub_82119450` | `AudioManager::~AudioManager` (deleting) | | Slot 0 of the manager's vtable `0x82008388`: `AudioManager::Destroy` (`sub_82119278`), then `operator delete` (`sub_825A70E8`, `heap.md`) when r4's bit 0 is set; returns the manager. No direct callers. Not seen in play (the game never shuts down in play). Read in full (B7 loop, 2026-10-31). |
| `sub_82119278` | `AudioManager::Destroy` | | r3 = the manager: its three vtables written back (`0x82008388`, `0x82008374`, `0x82008364` at +0, +4, +8), `sub_82119018` and `sub_82118930` on it, `sub_8259D370` on the audio engine (+16), the handler object at +1584 reset (its +0, +4 and +24 words), the name freed (`std::string::_Tidy(+1556, 1, 0)`, `std-string.md`), the two lists of sounds (+1540, +1528) cleared (`List::Clear`, `app-states.md`) and their head nodes freed, then the bank pairs' vector (+1512). From `AudioManager::~AudioManager` (deleting). Not seen in play. Not read in detail (70 instructions; its callees not read). |
| `sub_82117EB8` | `AudioManager::IsAnyBankLoading` | | Slot 1: 1 when any filled slot of the bank pairs' vector (+1516 to +1520) holds a pair still loading (`BankPair::IsLoading` `sub_82117670`, `sound-cues.md`), else 0 (also with no vector). Each index range-checked with a trap (`twi`), never reached. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82117FF0` | `AudioManager::FindBank` | | Slot 2: r4 = a name (C string): the index of the first filled slot whose pair's name is it (`BankPair::HasName`), or -1. Traps as `IsAnyBankLoading`. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82117D00` | `BankPair::HasName` | | r3 = a bank pair, r4 = a C string: whether the pair's `std::string` at +40 equals it (its length by a byte loop, then the compare `sub_820A6D28`, not read, with 0 and the string's +20; 1 for equal). From `AudioManager::FindBank`. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82117F78` | `AudioManager::FindFreeBankSlot` | | Slot 3: r4 = the first index's address: the first empty slot from there, or -1 past the vector's end (or with no vector). Leaf. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118270` | `AudioManager::IsBankLoaded` | | Slot 7: r4 = the bank index's address: 0 for an empty slot, else `BankPair::IsLoaded` (a tail call). Traps on no vector or an index past its end. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82117698` | `BankPair::IsLoaded` | | r3 = a bank pair: 1 when both loaded bytes (+108, +109) are 1, else 0 (`BankPair::IsLoading`'s opposite). From `AudioManager::IsBankLoaded`. Leaf. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118320` | `AudioManager::GetCueIndex` | | Slot 9: r4 = the bank index's address, r5 = a cue name (probably): -1 for an empty slot or a pair being released (`BankPair::CanFree`, `sound-cues.md`), else `BankPair::GetCueIndex` (r4 = r5), -1 for its 0xFFFF. Traps as `IsBankLoaded`. A 128-byte frame. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_821176C0` | `BankPair::GetCueIndex` | | r3 = a bank pair, r4 passed on: `SoundBankHolder::GetCueIndex` on the pair's sound bank holder (+76), its answer cut to 16 bits. From `AudioManager::GetCueIndex`. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_8259D3F0` | `SoundBankHolder::GetCueIndex` | | r3 = a sound bank holder: with an XACT sound bank (+8), a tail call to its slot +0 (probably GetCueIndex, r4 the name), else -1. In the wrapper's range (`sound-cues.md`'s intro). From `BankPair::GetCueIndex`. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82119608` | `AudioManager::PlayCue3D` | | Slot 11: `AudioManager::PlayCue` (`sub_82119500`, `sound-cues.md`) with r4 to r7 as given (the bank and cue indexes' addresses, a position, a velocity) and r8 = a byte 1 at its 96-byte frame's +80 (the 3D byte): `PlayCue2D` with a position and 3D on. No direct callers. Not seen in play (the 3D path never ran, `sound-cues.md`'s intro). Read in full (B7 loop, 2026-10-31). |
| `sub_821189F0` | `AudioManager::StopBank` | | Slot 12: r4 = the bank index's address: `Sound::Stop` on every `Sound` in the list at +1528 of that bank (`Sound::IsBank`); `AudioManager::StopCue` without the cue. The list's iterator checks trap (`twi`), never reached. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82119690` | `AudioManager::PlayHandle` | | Slot 15: r4, r5 = the bank and cue indexes' addresses: -1 for a negative index or an empty slot (a trap with no vector or past its end); else a `Sound` (`operator new(24)`, `Sound::Sound` with the owner manager + 8, 0 for a null manager; 0 when new fails, still used), `Sound::Set3D` 0, `Sound::SetPosition` (0, 0, 0, 1), its velocity 1.0 to `sub_82106540` (nothing), `Sound::SetHandle` with +1552, `Sound::Update`, then appended to the list at +1540 (`PtrList::Insert` before its head, `input.md`); returns +1552 before adding one to it (the handle). `PlayCue2D`'s sound kept by a handle for the slots below. A 160-byte frame, f31 saved. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82117550` | `Sound::SetHandle` | | r4 = a word's address: the `Sound`'s +12 = the word. Leaf. From `AudioManager::PlayHandle`. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118CF0` | `AudioManager::FindHandle` | | r4 = a handle's address: the first `Sound` in the list at +1540 with that handle (`Sound::HasHandle`), or 0. The iterator checks trap, never reached. From the slots 16 to 22. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82117598` | `Sound::HasHandle` | | r4 = a handle's address: 1 when it equals the `Sound`'s +12. Leaf. From `AudioManager::FindHandle`. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118D90` | `AudioManager::SetHandlePosition` | | Slot 16: r4 = a handle's address, r5 = a position: `Sound::SetPosition(r5)` on its `Sound`, when found (`AudioManager::FindHandle`). No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118DD0` | `AudioManager::SetHandleVelocity` | | Slot 17: as slot 16 with `sub_82106540` (nothing: the velocity setter is the empty function). No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118E10` | `AudioManager::SetHandle3D` | | Slot 18: as slot 16 with `Sound::Set3D(r5)`. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118E50` | `AudioManager::SetHandle3DParams` | | Slot 19: as slot 16 with all three: `Sound::SetPosition(r5)`, `sub_82106540(r6)` (nothing), `Sound::Set3D(r7)`. A 128-byte frame. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118EA0` | `AudioManager::StartHandle` | | Slot 20: `Sound::Start` (`sub_821173F0`, `sound-cues.md`) on the handle's `Sound`, when found. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118ED0` | `AudioManager::StopHandle` | | Slot 21: `Sound::Stop` (`sub_82117400`, `sound-cues.md`) on the handle's `Sound`, when found. No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118F00` | `AudioManager::RemoveHandle` | | Slot 22: the handle's `Sound` (into its 112-byte frame's +80); when found, removed from the list at +1540 (`HandleList::Remove`) and freed (`sub_82118490`, `sound-cues.md`: its destructor, `operator delete`, the slot cleared). No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118790` | `HandleList::Remove` | | r3 = a list of `Sound` pointers (the manager's +1540), r4 = a value's address: every node holding that value erased (`PtrList::Erase`, `sub_82098E80`, `app-states.md`), probably `std::list::remove`. The iterator checks trap, never reached. From `AudioManager::RemoveHandle`. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82118C70` | `AudioManager::ClearHandles` | | Slot 23: every `Sound` in the list at +1540 freed (`sub_82118490` on its node's +8), then the list cleared (`List::Clear`, `app-states.md`). No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_821190B0` | `AudioManager::UpdateHandles` | | Slot 24: `Sound::Update` (`0x82117438`, `sound-cues.md`) on every `Sound` in the list at +1540, through the list's `std::for_each` (`sub_82118F50`, `sound-cues.md`; the first and end iterators built in its 112-byte frame). No direct callers (`AudioManager::Update` updates only the list at +1528). Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_82117E20` | `AudioManager::SetListenerParams` | | Slot 28: on the listener (+1456), its +48 = the float at r4 (`RigidBody::SetFloat48`, `sub_8259D5A8`, `car-physics.md`: the same store, folded) and +52 = the float at r5 (`Listener::SetFloat52`); probably the listener's two last floats (`../known-data.md`). No direct callers. Not seen in play. Read in full (B7 loop, 2026-10-31). |
| `sub_8259D5B8` | `Listener::SetFloat52` | | r3 + 52 = the float at r4. Leaf; in the wrapper's range, probably folded with other classes' setters. From `AudioManager::SetListenerParams`. Not seen in play. Read in full (B7 loop, 2026-10-31). |
