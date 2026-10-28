# Known functions: system services

The game's functions that call system service kernel imports, and the
game code around them met along the way: sign-in and the user, the
profile, the system dialogs, sessions, achievements and stats (XGI),
Xbox Live (XLiveBase), the player's own music (XMP), network and system
link, voice chat, notifications, the loader and start-up, and the
console's settings. Worked on as B5 in `docs/rexglue-removal.md`, which
has the survey (what runs, what the SDK answers). Xbox-only features are
mapped here but not ported ("The approach" there). The rest of the
game's functions, and the prose sections, are in [`functions.md`](functions.md).

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
| `sub_820943C8` | | Overridden | The app's vtable +8: tail-calls its +8 object's vtable +36 (`sub_82493700`). From the Live connection reader. Ours (B7 loop, 2026-10-28): plain C++ in `src/save_interface/` behind its adapter in `src/hooks/save_interface.cpp` (`--native_save_interface`, falling back to the original); harness `save_interface` (8 cases, two with the +36 interface's slots swapped: the original then); taken with the save interface (`save-interface.md`), whose `SaveInterface::UserCount` and `UserAt` reach it; reached through a vtable, so the adapter stays. |
| `sub_82099580` | | | The app's Live connection state (vtable +36 of the object at `82000FA0`): r4 = out; copies +252 to it and returns +248. From the Live connection reader (no longer called). |
| `sub_82099600` | `App::ReadUsers` | | For users 0-3: the sign-in state (`sub_82295028`) at +8 + 40n, and when signed in the name (`sub_82295020`, 16 bytes) and XUID (`sub_82295268`); a list of signed-in users (+200, count +216) and of Live ones (state 2, +168, count +220). In play: user 0 state 1, "User", `B13EBABEBABEBABE`. |
| `sub_82099828` | `App::PollSignIn` | Overridden | Reached through a vtable from `sub_82099A98`, every frame. Reads one notification from the listener at +240: sign-in changed (`0xA`) sets +244 and rebuilds the users (`sub_82099600`); Live connection changed (`0x02000002`, never sent) sets +248 and keeps its parameter at +252. In play only the SDK's start-up sign-in changed (twice) reached it. Ours (B5 step 6, `src/hooks/notify.cpp`): sets +244 and builds the users once, on the first call with a listener; harness `notify`: the same app object and r3. |
| `sub_820998B8` | | | Creates the app's listener (`sub_822952B8`, mask 3: the first with the system bit, so it gets the SDK's start-up notifications). Clears +244, +248 and +252. Not read in detail. |
| `sub_820A4610` | `SaveDevices::Update` | Overridden | Reached through a vtable from `sub_82099A98`, every frame; r3 = the save devices. Only with a listener (+56): gets the device list (+12's vtable +4), each slot's device state from it (its +8 with the slot's index, into +956; slots of 1,000 bytes at +44, count +48); on storage devices changed (`0xB`, which the SDK never sends) re-checked each slot's device (`SaveSlot::CheckDevice`, `sub_820A45C0`, in [`files-and-saves.md`](files-and-saves.md)); then for each slot whose state isn't 0-3 asks +28 (slot index) and clears +36, +960-+972, +984 and +988 when that's under 5, and runs the slot's handler (the object's +76 + 4 x its +988; r4 the slot) for its next state (+988). Ours (`src/hooks/notify.cpp`): the same without the notification read. Harness `notify`: 60 made-up cases the same. |
| `sub_82148868` | | | The network start-up, from `sub_8214E190`: `sub_82575B08` (WSAStartup), two listeners (`sub_822952B8`, masks 2 and 1, kept at `826C6AB4` and `826C6AB8`), clears state (60 bytes at `826C6650`, 28 at `826C6080`, 384 at `826C7670`, the machine id's block) and keeps r3 at `826C7300`. Not read in detail. |
| `sub_82148958` | | Dropped | Makes the system link's socket: `socket(2, 2, 254)` (UDP, protocol 254) kept at `826C6AA0`, bound to port 1000, then FIONBIO; on Linux the SDK's bind and ioctlsocket failed (-1). Once at boot, from `sub_8214E190`. Dropped (B5 step 4, `src/hooks/network.cpp`): stores INVALID_SOCKET (-1), so system link stays off (the user's call: the SDK gave a handle). |
| `sub_82148A80` | | | Another socket set-up like `sub_82148958` (socket, bind, ioctlsocket). Never called in play. |
| `sub_82149080` | | Dropped | From `Network::Update` every frame: reads Live connection changed (`0x02000002`, never sent) into `826C72F8` (+`826C72FC` = 1), or asks the app (`826C7300`; its +8, `sub_820943C8`, reaches `sub_82099580`) for the state, which copies the app's +252 into `826C72F8` and answers its +248; returns 1 once (`826C7304`). Both are 0 (the app's set-up `sub_820998B8`; only App::PollSignIn wrote them, on a Live connection change), so it returned 0 in play. Its only caller, Network::Update, no longer calls it. |
| `sub_82149178` | | | Reads the profile for user r3: clears the overlapped at `826C6080` (Network::Update skips while it's 997), frees the last results (`826C6AD0`) and the 26 per-setting buffers, asks the size (`sub_82576E78`), allocates and clears the results, allocates the per-setting buffers (256 for the first 23, 1,000 for the title-specific ones), then reads the 26 settings (`82008EA0`) with the overlapped. From `sub_8214E268` on sign-in changes. Network::Update then reads setting 3's value (the gamercard region). |
| `sub_8214AA70` | | | A system link send: `sendto` (`sub_82459CF8`) on the socket at `826C6AA0`. Never called in play. Not read in detail. |
| `sub_8214AB48` | | | Another system link send (`sendto`, then `WSAGetLastError` on failure). Never called in play. Not read in detail. |
| `sub_8214D7A0` | | | The system link receives, from `Network::Update`'s state machine: `accept`, `select`, `__WSAFDIsSet` and `recvfrom` on the socket at `826C6AA0`, then the message handlers. Its socket calls never ran in play. Not read in detail. |
| `sub_8214E190` | | | The network manager's set-up (once at boot): `sub_8214F1C0(0, 4, 0, 1, r3)`, the start-up (`sub_82148868`), the system link socket (`sub_82148958`), XNetGetTitleXnAddr (looped until nonzero) and XNetXnAddrToMachineId (into `826C7670`), `sub_8214F6E8(826C74A0, the id)`, then clears five words. Not read in detail. B5. |
| `sub_8214E268` | `Network::Update` | Overridden | Every tick, from `sub_82107910` (r3 unused by it). The original: the sign-in helper (`sub_8214F290`), the users' names and XUIDs into the user table (`826C7670`: +12 count, +14 32-byte UTF-16 names, +144 XUIDs) after a sign-in poll, the profile read (`sub_82149178`, for player 1's controller's user from `826BABA4`, else the first signed-in user) when the front end sets `826C7938`, and after each read the gamercard region from the results into `826C77AC`..`826C77B8`, `826C6728` and `826C7288`; and Xbox-only parts: the Live users' voice registration (`sub_8214F810`), XNet, Live connection changes (`sub_82149080`), notification `0x11`, voice chat data (`sub_8214FA78`), and system link code the game has no mode for. Ours (`src/hooks/network.cpp`): user 0 signed in (`src/user/user.h`) written on the first call, the game's profile read on the first call and on each request, the region copied after; nothing else. Its writes in play the same as the recompiled one's, word for word (a session each, `itc_304.log` and `itc_306.log`, 2026-10-21). |
| `sub_8214F1C0` | | | The sign-in helper's set-up, once from `sub_8214E190` with (0, 4, 0, 1, the app): the fewest and most users (`826C7944`, `82618B7C`), `826C7948`, `82618B80`, the app (`826C799C`), and a listener (`826C794C`, mask 3). |
| `sub_8214F290` | `SignIn::Update` | Dropped | Every frame, from `sub_8214E268`. Counts the signed-in users (`826C7954`..`826C795C`), the first one (`82618B84`), the sign-in UI (`sub_822952C0`), a "sign in" message box (`sub_822952E8`, text by `swprintf`, its only caller: the "VS MODE" format at `0x82009218`) whose button 1 launches the dashboard (`sub_822967E0`), and XN_SYS_UI (`9`) from its listener (`826C794C`). The message box never ran. Not read in detail. B5. Its sign-in poll (one user: `826C7950`, `826C7954`, `826C7958`, `826C796C`, `826C7998` 1, `826C795C` 0, `82618B84` 0) is written by our Network::Update, which no longer calls it; its dialog and message box never ran (the bounds `826C7944`/`82618B7C` are 0 and 4, set once by `sub_8214F1C0`). |
| `sub_8214F690` | | | Releases the voice engine (table +4) when the network manager's +0 isn't 0, then `sub_82294E20`. From `sub_825DB040`. The only engine user that checks for null. Not read in detail. |
| `sub_8214F6E8` | | | Called by `sub_8214E190` with the network manager (`826C74A0`) and the machine id (0 in play). Not read. |
| `sub_8214F790` | | | Through the voice engine: +48 (the remote talkers), then +44 (unregister a remote talker). From `sub_8214B330` (system link). Not read in detail. |
| `sub_8214FA78` | | | Every frame, from `Network::Update`: the voice engine's +72 (`GetDataReadyFlags`, 0 with no talker); when the local user's bit is set, +76 (`GetLocalChatData`) into the manager's buffer (+36, count at +436); returns 1 when 70 bytes are waiting or 200 ms passed (`sub_822956F0`), else 0. Not read in detail. |
| `sub_8214FDF8` | | | Through the voice engine: +48 (the remote talkers), XMsg `FC 5800E` (`sub_82576EE0`), `sub_8214FD68`, +80. From `sub_8214AC68` (system link). Not read in detail. |
| `sub_8226BCB8` | | | UI code (350 instructions) that reads the network manager (probably the voice engine's +52, headset present). Not read in detail. |
| `sub_8226EEC8` | | | UI code (361 instructions) like `sub_8226BCB8`, from seven callers. Not read in detail. |
| `sub_82294E58` | `XUserWriteAchievements` | | XMsg `FB B0008` (count, list; overlapped r5). Never called. |
| `sub_82294ED8` | `XUserSetContextEx` | Dropped | XMsg `FB B0006` (user, context, value; overlapped r6): with an overlapped it answers 997, else GetLastError (`sub_82296EC0`) turned into 0 or 1627. The SDK answered 0 and cleared the last error. Only `sub_82294F70` calls it, so it isn't called since that was dropped (B5 step 2; no override). |
| `sub_82294F70` | `XUserSetContextEx` | Dropped | XUserSetContextEx without an overlapped: `sub_82294ED8` with r6 0. Every SetContextEx call: `sub_82293504` (context `8001`, probably presence), `sub_822934B8`, `sub_822917B8`, `sub_82148FD8`, `sub_82148B60` (not read). Dropped (B5 step 2, `src/hooks/user.cpp`): returns 0 and clears the last error, as the SDK's answer did; harness `user`: the answer and the last error the same. |
| `sub_82294F78` | `XUserSetPropertyEx` | | XMsg `FB B0007`; `sub_82295018` passes no overlapped. Never called. |
| `sub_82295020` | `XUserGetName` | Overridden | `XamUserGetName` (user, buffer, size); returns an HRESULT. The SDK: users 4 and up `0x80070057`, 1-3 `0x80070525`, nothing written; user 0 gets 0 and "User" in at most `min(size, 16) - 1` characters and a terminator. In play: "User" (5 calls). Ours: `user::Name` (`src/user/user.h`), adapter in `src/hooks/user.cpp`; harness `user`: the same for users 0-5 and sizes 0-20. B5 step 2. |
| `sub_82295028` | `XUserGetSigninState` | Overridden | `XamUserGetSigninState` (user). In play: 1 for user 0, 0 for 1-3 (20 calls). Ours: `user::SigninState` (1 for user 0, else 0); harness `user`: the same for users 0-5. B5 step 2. |
| `sub_82295030` | `XUserCheckPrivilege` | | On systems before `20096B00` answers "not logged on" (1245, result 0) when the user (or any, for 255) is signed in locally; else `XamUserCheckPrivilege`. Never called. |
| `sub_822950C0` | | | XMsg `FB B0021`. Never called. |
| `sub_822951A8` | | | `XamUserCreateStatsEnumerator` (r4 1) after `sub_82296EE0`. Never called. |
| `sub_82295208` | | | `XamUserCreateStatsEnumerator`, another kind. Never called. |
| `sub_82295268` | `XUserGetXUID` | Overridden | `XamUserGetXUID(user, 7, out)`; errors in facility 7 come back as their code, others as 1627. The SDK: user 0 `B13EBABEBABEBABE` and 0, users 1-3 an XUID of 0 and 1317, 4 and up 0 and 87, a null out 87 with nothing written. In play: `B13EBABEBABEBABE` (5 calls). Ours: `user::Xuid`; harness `user`: the same for users 0-5, with and without an out. B5 step 2. |
| `sub_822952B8` | `XNotifyCreateListener` | Dropped | `XamNotifyCreateListener(mask, 0)`. Five at boot: the app (3), the storage devices (1), the sign-in helper (3), the network start-up (2 and 1). Dropped (B5 step 6, `src/hooks/notify.cpp`): no listener; returns a fixed nonzero value (`4E4F4C53`, not an SDK handle), which the readers only test for 0, and which leaves the readers still calling `XNotifyGetNext` with nothing. |
| `sub_822952C0` | `XShowSigninUI` | Dropped | `XamShowSigninUI` (panes, flags). From `sub_8214F290`, `sub_821A2FB0`, `sub_821A4228`, `sub_8227C088` and `sub_8227C210`; once in B5's survey (from `sub_8227C210`). The SDK answered 0 and sent sign-in changed and XN_SYS_UI off, which since step 6 reach only voice's listener. Ours (`src/hooks/user.cpp`): returns 0. Harness `user`: the same. |
| `sub_822952C8` | `XShowFriendsUI` | | Never called. |
| `sub_822952D0` | `XShowGamerCardUI` | | `XamShowGamerCardUIForXUID`. Never called. |
| `sub_822952D8` | `XShowPlayerReviewUI` | | Never called. |
| `sub_822952E8` | `XShowMessageBoxUI` | | `XamShowMessageBoxUI`, the overlapped from the caller's stack. Never called. |
| `sub_82295310` | | | `XamShowDirtyDiscErrorUI`, then `sub_822967E0(0, 0)`. Never called. |
| `sub_822954C8` | | | A wait wrapper: `sub_82296F48` (r4 as a byte), returns 192 when that does, else 0. Not read in detail. The boundary pipeline credited `xstart`'s imports to it until 2026-10-20. |
| `sub_822967E0` | `XLaunchNewImage` | | `XamLoaderLaunchTitle`, with flags for the two names at `820228D8`/`820228EC` (probably the dashboard's). Never called. |
| `sub_82296950` | | | The PAL-50 message box (`XamShowMessageBoxUIEx` with an event, waits through `sub_82295C98`). Never called. |
| `sub_82296AB0` | `CheckPal50` | Dropped | The XDK's PAL-50 check, once from `xstart`: privilege 10 (`XexCheckExecutablePrivilege`, probably "PAL-50 incompatible"), then unless the AV pack (`XGetAVPack`) is 3, 4, 6 or 8, the AV region (`ExGetXConfigSetting(2, 2)`, `& 0xFF00 == 0x300`) and video flags (`(3, 10)`), and on a 50 Hz console a message box (`sub_82296950`) in the console's language (`XGetLanguage`, strings by `sub_82296A08`); returns 1 if shown. Returns 0 in play: the executable's system flags are `0x600` (privilege 10 set) and the SDK's AV pack is 6. Dropped (B5 step 1): returns 0 (`src/hooks/start_up.cpp`); harness `start_up` gives the same answer. |
| `xstart` | `xstart` | | The C runtime's entry point (`0x82296C80`). The heap check (`sub_82297CE0`), `sub_82297B38(1)`, the PAL-50 check (`sub_82296AB0`: `XamLoaderTerminateTitle` if 1), the C runtime's set-up, the command line (`+14264`), `main` (`sub_82090108`), the exit (`sub_825AA388`), then `DbgPrint` and `XamLoaderTerminateTitle`. Not read in detail. B5 (and B6's `DbgPrint`). |
| `sub_82296EE0` | `CheckTitleId` | Overridden | 0 when r3 is 0 or its high half is this title's (`XamGetExecutionId`, title id +12: `555307EE`), else 1627. Before the profile read and stats enumerators (10 calls, 0). Ours: `user::CheckTitleId`; harness `user`: 200 title ids the same. B5 step 2. |
| `sub_82297C08` | `CreateHeap` | Overridden | Reads the image's heap size (`RtlImageXexHeaderField(header, 0x20401)`, default 1 MB; returns 1 if it's 0) and creates the heap once (`sub_822997B0(2, 0, size, 4096, 0, attributes)`, the attributes a 48-byte block, its size then zeros; kept at `8297149C`, first written here as `826A549C`); returns 1 when it exists. Once at boot (1). `default.xex` has no `0x20401` field, so the size is 1 MB. Ours (`src/hooks/start_up.cpp`): the same creation with the size fixed, `sub_822997B0` as a guest call in a frame like the original's; no header read. |
| `sub_82297CE0` | `CheckHeap` | Overridden | Called once by `xstart`: `sub_82297C08`; if that fails, calls the debug monitor's routine (`820007F0`, none without a debugger) and `HalReturnToFirmware(1)`. Answers 1 in play (no firmware return). Ours (`src/hooks/start_up.cpp`): makes the heap as `sub_82297C08` does; if it can't, logs an error and exits (the SDK's `HalReturnToFirmware` exited too). B5 step 1. |
| `sub_822997B0` | | | Probably RtlCreateHeap. Makes a heap: r3 flags (2), r4 base (0), r5 size, r6 4096 (probably the commit size), r7 0, r8 an attributes block (copied when its first word is 48), returns the heap or 0. Called by `sub_82297C08` (xstart's heap). With `--native_heap` its heap stays empty: the heap's four functions are routed to the native layer (`heap.md`). Not read in detail. |
| `sub_8229B440` | `ReadTimeZoneSettings` | Overridden | r3 = TIME_ZONE_INFORMATION (out), r4 = "uses daylight saving" (out, or 0); returns 0 or a DOS error. Reads the console's time zone settings (`ExGetXConfigSetting` category 3, settings 1-7, and 12 when r4 isn't 0; `RtlNtStatusToDosError` on failure): writes the bias, each name's first four characters (widened bytes) and a terminator after them (the rest left alone), each date cleared (`sub_825A71D0`) then month, day of week, day and hour, the two biases; "uses daylight" is retail flags bit 1 clear. The SDK answers zeros and retail flags `0x40`: an all-zero zone, "uses daylight" 1. In play (B5's survey, 2026-10-20): 4 calls, from the front end. Ours (B5 step 1): `helpers::kConsoleTimeZone` (`src/helpers/console_settings.h`) written by `time_hooks::ReadTimeZoneSettings` (`src/hooks/time.cpp`), called directly by GetTimeZoneInformation; never fails. It answers the time harness's made-up zones (`time_hooks::SetFakeZone`); the `REX_FUNC` adapter is left only for the recompiled time functions the harness runs (so Overridden, not Native, until the harness goes). Harness: the same bytes as the recompiled one, "uses daylight" given and not. |
| `sub_82459B68` | `WSAStartup` | Dropped | `NetDll_WSAStartup` (through `XexGetProcedureAddress` on systems from `200A3200`; the SDK's is 0). Once at boot from `sub_82575B08` (through `sub_82459C10`): 0. Dropped (B5 step 4): returns 0; the WSADATA it filled is never read. Harness `network`: the same answer. |
| `sub_82459C10` | | | WSAStartup's entry: `sub_82459B68` with r3 1 (the caller). From `sub_82575B08`. |
| `sub_82459C20` | `socket` | | `NetDll_socket`. From `sub_82148958` (Dropped, B5 step 4) and `sub_82148A80` (never called). Never called in play since step 4 (two attract races and two sessions the user played, 2026-10-21). B5. |
| `sub_82459C38` | `closesocket` | | Never called. |
| `sub_82459C48` | `ioctlsocket` | | `NetDll_ioctlsocket` (`FIONBIO`). From `sub_82148958` (Dropped, B5 step 4) and `sub_82148A80` (never called). Never called in play since step 4 (two attract races and two sessions the user played, 2026-10-21). B5. |
| `sub_82459C60` | `bind` | | `NetDll_bind`. From `sub_82148958` (Dropped, B5 step 4) and `sub_82148A80` (never called). Never called in play since step 4 (two attract races and two sessions the user played, 2026-10-21). B5. |
| `sub_82459C78` | `connect` | | Never called. |
| `sub_82459C90` | `listen` | | Never called. |
| `sub_82459CA0` | `accept` | | Never called. |
| `sub_82459CB8` | `select` | | Never called. |
| `sub_82459CD8` | `recvfrom` | | Never called. |
| `sub_82459CF8` | `sendto` | | Never called. |
| `sub_82459D18` | `WSAGetLastError` | | Never called. |
| `sub_82459D20` | `__WSAFDIsSet` | | Never called. |
| `sub_82459D28` | `XNetXnAddrToInAddr` | | Never called. |
| `sub_82459D40` | `XNetXnAddrToMachineId` | Dropped | `NetDll_XNetXnAddrToMachineId` (XNADDR, id out). Once at boot from `sub_8214E190`: 1 (not signed in to Live), nothing written. Dropped (B5 step 4): returns 1. Harness `network`: the same. |
| `sub_82459D50` | `XNetGetTitleXnAddr` | Overridden | `NetDll_XNetGetTitleXnAddr` (XNADDR out, 36 bytes). Once at boot from `sub_8214E190`, looped until nonzero: 4, with 127.0.0.1, no online address or port, `CC` for each Ethernet address byte, no online id. Ours (B5 step 4, `src/hooks/network.cpp`): the same bytes and answer as fixed values. Harness `network`: the same. |
| `sub_82493700` | | Overridden | Returns +92 (the app, for the Live connection reader). Ours (B7 loop, 2026-10-28): plain C++ in `src/save_interface/` behind its adapter in `src/hooks/save_interface.cpp` (`--native_save_interface`, falling back to the original); harness `save_interface` (4 cases); taken with the save interface (`save-interface.md`: the users through `sub_820943C8`); the framework's `sub_820908D8` calls it too. Ran natively in an attract race; reached through a vtable, so the adapter stays. |
| `sub_82575B08` | | | WSAStartup (version 2, the WSADATA in its frame, unread), then on success `sub_82577560`; returns 0, 1627, or WSAStartup's error. Once at boot, from `sub_82148868`. |
| `sub_82575C90` | | | XMsg `FC 58004` (the logon id). Never called. |
| `sub_82575D78` | | | An XLiveBase request's buffers: `XamAlloc` and `XamFree`. Only from `sub_82576900` and `sub_82576BE0` (never called). Never called in play (two attract races and two sessions the user played, 2026-10-21). Not read. |
| `sub_82575E50` | `XSessionCreate` | | `XamSessionCreateHandle`, XMsg `FB B0010`. Never called. |
| `sub_82575FE0` | `XSessionModify` | | XMsg `FB B0018`. Never called. |
| `sub_82576080` | `XSessionDelete` | | XMsg `FB B0011`. Never called. |
| `sub_82576130` | | | XMsg `FB B0012` (probably join local). Never called. |
| `sub_825761D8` | | | XMsg `FB B0012` (probably join remote). Never called. |
| `sub_82576280` | | | XMsg `FB B0013` (probably leave local). Never called. |
| `sub_82576328` | | | XMsg `FB B0013` (probably leave remote). Never called. |
| `sub_825763D0` | | | XMsg `FB B001A`. Never called. |
| `sub_825764A8` | `XSessionStart` | | XMsg `FB B0014`. Never called. |
| `sub_82576540` | `XSessionEnd` | | XMsg `FB B0015`. Never called. |
| `sub_825765F0` | `XSessionSearchEx` | | XMsg `FB B001C`. Never called. |
| `sub_825766E0` | | | XMsg `FB B0025`. Never called. |
| `sub_825767D0` | | | XMsg `FC 58023`. Never called. |
| `sub_82576900` | | | An XLiveBase request with allocated buffers. Never called. |
| `sub_82576BE0` | | | An XLiveBase request with allocated buffers. Never called. |
| `sub_82576E78` | `XUserReadProfileSettings` | Overridden | r3 title, r4 user, r5 count, r6 ids, r7 size (in, out), r8 results, r9 overlapped; `XamUserReadProfileSettings` after `sub_82296EE0`. From `sub_82149178`: 26 settings, 4,192 bytes; the SDK's defaults, the title-specific ones unset (10 calls). Ours (B5 step 7, `src/hooks/profile.cpp`, the data in `src/user/profile.cpp`): the size query answered 4,192 (ERROR_INSUFFICIENT_BUFFER), the 26 settings as hardcoded data at the buffer's address, the overlapped completed as the SDK did; harness `profile`: the same on five cases. |
| `sub_82576EE0` | | | XMsg `FC 5800E`. Never called. |
| `sub_82576F30` | | | XMsg `FC 58035`. Never called. |
| `sub_82576FE8` | | | XMsg `FC 58035`. Never called. |
| `sub_82577560` | | | Copies two words (from `8265D404`, `8265D408`) into `82957680` (`sub_825A7410`); probably XNet start-up parameters. Called by `sub_82575B08`. Not read in detail. |
| `sub_82578BF8` | | | Probably XHVCreateEngine: the three parameters into a block, `Voice::Init` (`sub_8257B7A8`); on failure releases what it stored (table +4). Once at boot. The engine can't be failed: it's used without null checks (B5). |
| `sub_8257A498` | | | The voice engine's +0: adds to the reference count (+4), returns it (probably AddRef). |
| `sub_8257A4C0` | | | The voice engine's +8: tries or takes the critical section (probably Lock). Not read in detail. |
| `sub_8257A548` | | | The voice engine's +12 (`sub_8257DEB8`; probably StartLocalProcessingModes). Not read. |
| `sub_8257A5B8` | | | The voice engine's +16 (`sub_8257DF30`; probably StopLocalProcessingModes). Not read. |
| `sub_8257A628` | | | The voice engine's +28: stores r4 at +1324, returns 0 (probably SetMaxDecodePackets). |
| `sub_8257A650` | | | The voice engine's +32: registers a local talker (`sub_8257E4B0`, `XamVoiceCreate`). Never called in play. Not read in detail. |
| `sub_8257A708` | | | The voice engine's +36: unregisters a local talker (`sub_8257DDA0`, `XamVoiceClose`). Not read in detail. |
| `sub_8257A790` | | | The voice engine's +52: the local talker's +192 (`sub_8257D6D0`), 0 without a talker (probably IsHeadsetPresent). |
| `sub_8257A7E8` | | | The voice engine's +56: the local talker's +4 then +144, 0 without one (probably IsLocalTalking). |
| `sub_8257A818` | | | The voice engine's +72: returns +1268 (`GetDataReadyFlags`; 0 with no talker). Every frame from `sub_8214FA78`. |
| `sub_8257A820` | | | The voice engine's +76: a local talker's chat data (`sub_8257E2B0`), E_INVALIDARG without a talker (`GetLocalChatData`). Not read in detail. |
| `sub_8257AED8` | | | The voice engine's +4: drops a reference (`sub_8257AD68` frees it at 0; probably Release). Also Voice::Init's clean-up. Not read in detail. |
| `sub_8257AF30` | `Voice::Poll` | Dropped | The voice thread (`sub_8257B5D8`), every 10 ms, in the engine's critical section (+1272): reads one notification from its listener (+1312; sign-in changed `0xA`, `0xE`, `0x04000002`, `0x04000003` set +1316, read only by `sub_8257D798`, never called), sets up an audio voice once (`sub_8245A050`, `sub_8245A168`, flag +1092; nothing else uses it), `sub_8257A8F8`, submits the four local talkers' data (`sub_8257DFA8`, none in play), updates the remote talkers (`sub_8257EED0`, none). No longer called: the thread is Dropped. Not read in detail. B5. |
| `sub_8257B0A8` | | | The voice engine's +20 (`sub_8257EF80`; probably StartRemoteProcessingModes). Not read. |
| `sub_8257B118` | | | The voice engine's +24 (`sub_8257EFF8`; probably StopRemoteProcessingModes). Not read. |
| `sub_8257B188` | | | The voice engine's +40: registers a remote talker (`sub_8257F070`). Not read in detail. |
| `sub_8257B290` | | | The voice engine's +44: unregisters a remote talker (`sub_8257F110`). Not read in detail. |
| `sub_8257B2F0` | | | The voice engine's +48: the remote talkers' XUIDs (r5) and count (r4), skipping empty ones (probably GetRemoteTalkers). |
| `sub_8257B3B0` | | | The voice engine's +60 (`sub_8257F210`; probably IsRemoteTalking). Not read. |
| `sub_8257B408` | | | The voice engine's +64 (`sub_8257E9B8`). Not read. |
| `sub_8257B470` | | | The voice engine's +68 (`sub_8257EAC0`). Not read. |
| `sub_8257B4F8` | | | The voice engine's +80 (`sub_8257F1E8`; probably SetPlaybackPriority). Not read. |
| `sub_8257B568` | | | The voice engine's +84 (`sub_8257F198`; probably SubmitIncomingChatData). Not read. |
| `sub_8257B5D8` | | Dropped | The voice thread's entry (made by `Voice::Init` when the parameters' +8 is 0), r3 the engine: a waitable timer (10 ms), sets the "started" event (+1304), then waits on the timer and runs `Voice::Poll` until +1300 is 3 (set by the release `sub_8257AD68`), cancels and closes the timer, exits. Ours (`src/hooks/voice.cpp`): sets +1304 and returns, so voice chat (Xbox-only) does nothing; the release finds the thread ended. |
| `sub_8257B688` | | | Fills the voice engine's table at `829576D0` once (22 entries, `sub_8257A498`..`sub_8257B568`). |
| `sub_8257B7A8` | `Voice::Init` | | Once at boot from `sub_82578BF8`: fills the engine's table (`sub_8257B688`, at `829576D0`), allocates the engine (2,240 bytes: +0 the table, +4 a reference count of 1, +8 a copy of the 48-byte parameters), `sub_82579F88`, a profile read per local talker (`sub_8257E568`, 4, into +1096), `sub_8257F388` per remote talker (none), an event (+1304), a listener (mask 5, +1312), a critical section (+1272), the voice thread (`sub_8257B5D8`, +1264) and waits for it to start; stores the engine through r4 (0 on failure, after `sub_8257AED8`). The thread only when the parameters' +8 is 0, then `sub_825BA338` with the thread when +4 is set (probably its processor). Its listener and `sub_8257E568` stay recompiled until the network manager's code (B7); its thread is Dropped. Not read in detail. B5. |
| `sub_8257D6D0` | | | A local talker's +192 (for the engine's +52). |
| `sub_8257D798` | | | The voice code's user check (headset, sign-in, profile, privileges, friends). Never called. |
| `sub_8257DDA0` | | | Voice close. Never called. |
| `sub_8257DFA8` | | | Voice submit. Never called. |
| `sub_8257E3B8` | | | Voice close. Never called. |
| `sub_8257E4B0` | | | `XamVoiceCreate`. Never called. |
| `sub_8257E568` | | | Voice's local talker set-up (4 calls at boot, from `Voice::Init`, r6 = +1096 + 4n): allocates a 472-byte talker, calls an import, allocates its buffers (`sub_8257C858`, five), the engine's methods, and somewhere its profile read (`XamUserReadProfileSettings`). Not read in detail. B5 (B7 with the network manager). |
| `sub_825816D8` | | | `XamVoiceSubmitPacket`. Never called. |
| `sub_82589360` | | Dropped | XMsg `FA 7001A` (XMP: set the playback controller) through `XMsgStartIORequestEx` with a 12-byte block, then `sub_82295700(0)`; 0, or the error's code. Twice: at the boot movie's start (`sub_825894C8`) and end (`sub_825895A0`); 0. No longer called since those were dropped (B5 step 3; no override). |
| `sub_82589408` | | Dropped | XMsg `FA 7001B` (XMP: get the playback controller) through `XMsgInProcessCall`: r3 client, r4 controller (out), r5 locked (out); the SDK wrote 0 to both. Once, from `sub_825894C8`: 0. No longer called since that was dropped (B5 step 3; no override). |
| `sub_825894C8` | | Dropped | Takes the playback from the player's music: below system version `8A100` (the SDK's is 0), in a critical section (`8265D9F4`), reads the controller (`sub_82589408`), keeps whether the player's music had it (`8295777C`), sets it (`sub_82589360`) and marks it taken (`82957778`); 1627 if already taken. Once, from `Boot::StartMovie`, which ignores the answer. Dropped (B5 step 3, `src/hooks/xmp.cpp`): returns 0 and clears the last error, as the SDK's answers left it; harness `xmp`: the same. |
| `sub_825895A0` | | Dropped | Hands the playback back: the same check and critical section; if taken, sets the controller back (`sub_82589360`, 0 or 4 by `8295777C`) and clears both globals; 1627 if not taken. Once, from `Boot::EndMovie`, which returns its answer. Dropped (B5 step 3, `src/hooks/xmp.cpp`): returns 0 and clears the last error; harness `xmp`: the same. |
| `sub_825DA140` | | | Uses the XMP code's critical section (`8265D9F4`); probably sets it up. Not read. |
