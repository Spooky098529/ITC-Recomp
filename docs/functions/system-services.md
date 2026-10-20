# Known functions: system services

The game's functions that call system service kernel imports, and the
game code around them met along the way: sign-in and the user, the
profile, the system dialogs, sessions, achievements and stats (XGI),
Xbox Live (XLiveBase), the player's own music (XMP), network and system
link, voice chat, notifications, the loader and start-up, and the
console's settings. Worked on as B5 in `docs/rexglue-removal.md`, which
has the survey (what runs, what the SDK answers). Xbox-only features are
mapped here but not ported ("The approach" there). Rows are sorted by
address. The rest of the game's functions, and the prose sections, are
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
| `sub_82099600` | `App::ReadUsers` | | For users 0-3: the sign-in state (`sub_82295028`) at +8 + 40n, and when signed in the name (`sub_82295020`, 16 bytes) and XUID (`sub_82295268`); a list of signed-in users (+200, count +216) and of Live ones (state 2, +168, count +220). In play: user 0 state 1, "User", `B13EBABEBABEBABE`. |
| `sub_82099828` | `App::PollSignIn` | | Reached through a vtable from `sub_82099A98`, every frame. Reads one notification from the listener at +240: sign-in changed (`0xA`) sets +244 and rebuilds the users (`sub_82099600`); Live connection changed (`0x02000002`, never sent) sets +248 and keeps its parameter at +252. B5. |
| `sub_820998B8` | | | Creates the app's listener (`sub_822952B8`, mask 3: the first with the system bit, so it gets the SDK's start-up notifications). Not read in detail. |
| `sub_820A45C0` | | | A save slot's pending operation (+36): `sub_82295C10` (probably XGetOverlappedResult), cleared when done; returns it. Not read in detail. |
| `sub_820A4610` | `SaveDevices::Update` | | Reached through a vtable from `sub_82099A98`, every frame: each slot's state machine (+44, 1,000 bytes each, +956/+988), and on storage devices changed (`0xB`, which the SDK never sends) re-checks each slot's pending operation (`sub_820A45C0`). Not read in detail. B5 (its `XNotifyGetNext`). |
| `sub_82148958` | | | Makes the system link's UDP socket (socket, bind, non-blocking); both fail on the SDK. Once at boot. Not read in detail. |
| `sub_82149080` | | | From `sub_8214E268`: reads Live connection changed (`0x02000002`, never sent) into `826C72F8` (+`826C72FC` = 1), or asks an object (`826C7300`) for the state; returns 1 once. B5. |
| `sub_82149178` | | | Reads the profile (`sub_82576E78`, 26 settings) twice: the size, then the settings. From `sub_8214E268` on sign-in changes. Not read in detail. |
| `sub_8214E268` | `Network::Update` | | Every frame, from `sub_82107910` (Draw). The sign-in manager (`sub_8214F290`), the users' XUIDs and names, the profile read on sign-in changes (`sub_82149178`), the XNet address (`sub_82459D50`, `sub_82459D40`), Live connection changes (`sub_82149080`), notification `0x11` (never sent), and a state machine (13 states) with system link receives (`sub_8214D7A0`). Not read in detail. B5. |
| `sub_8214F290` | `SignIn::Update` | | Every frame, from `sub_8214E268`. Counts the signed-in users (`826C7954`..`826C795C`), the first one (`82618B84`), the sign-in UI (`sub_822952C0`), a "sign in" message box (`sub_822952E8`, text by `swprintf`, its only caller: the "VS MODE" format at `0x82009218`) whose button 1 launches the dashboard (`sub_822967E0`), and XN_SYS_UI (`9`) from its listener (`826C794C`). The message box never ran. Not read in detail. B5. |
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
| `sub_822952B8` | `XNotifyCreateListener` | | `XamNotifyCreateListener(r3 mask, 0)`. 5 calls at boot: masks 3 (`sub_820998B8`), 1 (`sub_820A6160`), 3 (`sub_8214F1C0`), 2 (`sub_82148868`) and 2 more; returns the handle. B5. |
| `sub_822952C0` | `XShowSigninUI` | | `XamShowSigninUI` (panes, flags). Once in play, from `sub_8227C210` (0; the SDK sends sign-in changed and XN_SYS_UI off). B5. |
| `sub_822952C8` | `XShowFriendsUI` | | Never called. |
| `sub_822952D0` | `XShowGamerCardUI` | | `XamShowGamerCardUIForXUID`. Never called. |
| `sub_822952D8` | `XShowPlayerReviewUI` | | Never called. |
| `sub_822952E0` | `XShowDeviceSelectorUI` | | `XamShowDeviceSelectorUI`, from `sub_820A4960` (user 0, content type 1, flags `200`): 997, device 1 through the overlapped. Twice in play, around saves. Belongs with B4. |
| `sub_822952E8` | `XShowMessageBoxUI` | | `XamShowMessageBoxUI`, the overlapped from the caller's stack. Never called. |
| `sub_82295310` | | | `XamShowDirtyDiscErrorUI`, then `sub_822967E0(0, 0)`. Never called. |
| `sub_822954C8` | | | A wait wrapper: `sub_82296F48` (r4 as a byte), returns 192 when that does, else 0. Not read in detail. The boundary pipeline credited `xstart`'s imports to it until 2026-10-20. |
| `sub_82295D30` | `XEnumerate` | | `XamEnumerate(handle, 0, buffer, size, items, overlapped)`, from `sub_820A49D0` on the content enumerator: 997 (3 calls, around saves). Belongs with B4. |
| `sub_822967E0` | `XLaunchNewImage` | | `XamLoaderLaunchTitle`, with flags for the two names at `820228D8`/`820228EC` (probably the dashboard's). Never called. |
| `sub_82296950` | | | The PAL-50 message box (`XamShowMessageBoxUIEx` with an event, waits through `sub_82295C98`). Never called. |
| `sub_82296AB0` | `CheckPal50` | Dropped | The XDK's PAL-50 check, once from `xstart`: privilege 10 (`XexCheckExecutablePrivilege`, probably "PAL-50 incompatible"), then unless the AV pack (`XGetAVPack`) is 3, 4, 6 or 8, the AV region (`ExGetXConfigSetting(2, 2)`, `& 0xFF00 == 0x300`) and video flags (`(3, 10)`), and on a 50 Hz console a message box (`sub_82296950`) in the console's language (`XGetLanguage`, strings by `sub_82296A08`); returns 1 if shown. Returns 0 in play: the executable's system flags are `0x600` (privilege 10 set) and the SDK's AV pack is 6. Dropped (B5 step 1): returns 0 (`src/hooks/start_up.cpp`); harness `start_up` gives the same answer. |
| `xstart` | `xstart` | | The C runtime's entry point (`0x82296C80`). The heap check (`sub_82297CE0`), `sub_82297B38(1)`, the PAL-50 check (`sub_82296AB0`: `XamLoaderTerminateTitle` if 1), the C runtime's set-up, the command line (`+14264`), `main` (`sub_82090108`), the exit (`sub_825AA388`), then `DbgPrint` and `XamLoaderTerminateTitle`. Not read in detail. B5 (and B6's `DbgPrint`). |
| `sub_82296EE0` | `CheckTitleId` | Overridden | 0 when r3 is 0 or its high half is this title's (`XamGetExecutionId`, title id +12: `555307EE`), else 1627. Before the profile read and stats enumerators (10 calls, 0). Ours: `user::CheckTitleId`; harness `user`: 200 title ids the same. B5 step 2. |
| `sub_82297C08` | `CreateHeap` | Overridden | Reads the image's heap size (`RtlImageXexHeaderField(header, 0x20401)`, default 1 MB; returns 1 if it's 0) and creates the heap once (`sub_822997B0(2, 0, size, 4096, 0, attributes)`, the attributes a 48-byte block, its size then zeros; kept at `8297149C`, first written here as `826A549C`); returns 1 when it exists. Once at boot (1). `default.xex` has no `0x20401` field, so the size is 1 MB. Ours (`src/hooks/start_up.cpp`): the same creation with the size fixed, `sub_822997B0` as a guest call in a frame like the original's; no header read. |
| `sub_82297CE0` | `CheckHeap` | Overridden | Called once by `xstart`: `sub_82297C08`; if that fails, calls the debug monitor's routine (`820007F0`, none without a debugger) and `HalReturnToFirmware(1)`. Answers 1 in play (no firmware return). Ours (`src/hooks/start_up.cpp`): makes the heap as `sub_82297C08` does; if it can't, logs an error and exits (the SDK's `HalReturnToFirmware` exited too). B5 step 1. |
| `sub_822997B0` | | | Makes a heap: r3 flags (2), r4 base (0), r5 size, r6 4096 (probably the commit size), r7 0, r8 an attributes block (copied when its first word is 48), returns the heap or 0. Called by `sub_82297C08` (xstart's heap). Not read in detail. |
| `sub_8229B440` | `ReadTimeZoneSettings` | Overridden | r3 = TIME_ZONE_INFORMATION (out), r4 = "uses daylight saving" (out, or 0); returns 0 or a DOS error. Reads the console's time zone settings (`ExGetXConfigSetting` category 3, settings 1-7, and 12 when r4 isn't 0; `RtlNtStatusToDosError` on failure): writes the bias, each name's first four characters (widened bytes) and a terminator after them (the rest left alone), each date cleared (`sub_825A71D0`) then month, day of week, day and hour, the two biases; "uses daylight" is retail flags bit 1 clear. The SDK answers zeros and retail flags `0x40`: an all-zero zone, "uses daylight" 1. In play (B5's survey, 2026-10-20): 4 calls, from the front end. Ours (B5 step 1): `helpers::kConsoleTimeZone` (`src/helpers/console_settings.h`) written by `time_hooks::ReadTimeZoneSettings` (`src/hooks/time.cpp`), called directly by GetTimeZoneInformation; never fails. It answers the time harness's made-up zones (`time_hooks::SetFakeZone`); the `REX_FUNC` adapter is left only for the recompiled time functions the harness runs (so Overridden, not Native, until the harness goes). Harness: the same bytes as the recompiled one, "uses daylight" given and not. |
| `sub_82459B68` | `WSAStartup` | | `NetDll_WSAStartup` (through `XexGetProcedureAddress` on older systems). Once at boot from `sub_82575B08`: 0. B5. |
| `sub_82459C20` | `socket` | | `NetDll_socket`. Once at boot from `sub_82148958` (UDP): a handle. B5. |
| `sub_82459C38` | `closesocket` | | Never called. |
| `sub_82459C48` | `ioctlsocket` | | `NetDll_ioctlsocket` (`FIONBIO`). Once at boot from `sub_82148958`: -1. B5. |
| `sub_82459C60` | `bind` | | `NetDll_bind`. Once at boot from `sub_82148958`: -1 (no system link). B5. |
| `sub_82459C78` | `connect` | | Never called. |
| `sub_82459C90` | `listen` | | Never called. |
| `sub_82459CA0` | `accept` | | Never called. |
| `sub_82459CB8` | `select` | | Never called. |
| `sub_82459CD8` | `recvfrom` | | Never called. |
| `sub_82459CF8` | `sendto` | | Never called. |
| `sub_82459D18` | `WSAGetLastError` | | Never called. |
| `sub_82459D20` | `__WSAFDIsSet` | | Never called. |
| `sub_82459D28` | `XNetXnAddrToInAddr` | | Never called. |
| `sub_82459D40` | `XNetXnAddrToMachineId` | | Once at boot from `sub_8214E190`: 1. B5. |
| `sub_82459D50` | `XNetGetTitleXnAddr` | | Once at boot from `sub_8214E190`: 4. B5. |
| `sub_82575C90` | | | XMsg `FC 58004` (the logon id). Never called. |
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
| `sub_82576E78` | `XUserReadProfileSettings` | | r3 title, r4 user, r5 count, r6 ids, r7 size (in, out), r8 results, r9 overlapped; `XamUserReadProfileSettings` after `sub_82296EE0`. From `sub_82149178`: 26 settings, 4,192 bytes; the SDK's defaults, the title-specific ones unset (10 calls). B5. |
| `sub_82576EE0` | | | XMsg `FC 5800E`. Never called. |
| `sub_82576F30` | | | XMsg `FC 58035`. Never called. |
| `sub_82576FE8` | | | XMsg `FC 58035`. Never called. |
| `sub_8257AF30` | `Voice::Poll` | | The voice thread (`sub_8257B5D8`), every 10 ms: reads its notifications, then the voice update (`sub_8257DFA8` and others). Not read in detail. B5. |
| `sub_8257B7A8` | `Voice::Init` | | Once at boot from `sub_82578BF8`: the voice code's state, its profile reads (`sub_8257E568`, 4 times), its listener and critical section. Not read in detail. B5. |
| `sub_8257D798` | | | The voice code's user check (headset, sign-in, profile, privileges, friends). Never called. |
| `sub_8257DDA0` | | | Voice close. Never called. |
| `sub_8257DFA8` | | | Voice submit. Never called. |
| `sub_8257E3B8` | | | Voice close. Never called. |
| `sub_8257E4B0` | | | `XamVoiceCreate`. Never called. |
| `sub_8257E568` | | | The voice code's profile read (4 calls at boot). Not read in detail. B5. |
| `sub_825816D8` | | | `XamVoiceSubmitPacket`. Never called. |
| `sub_82589360` | | Dropped | XMsg `FA 7001A` (XMP: set the playback controller) through `XMsgStartIORequestEx` with a 12-byte block, then `sub_82295700(0)`; 0, or the error's code. Twice: at the boot movie's start (`sub_825894C8`) and end (`sub_825895A0`); 0. No longer called since those were dropped (B5 step 3; no override). |
| `sub_82589408` | | Dropped | XMsg `FA 7001B` (XMP: get the playback controller) through `XMsgInProcessCall`: r3 client, r4 controller (out), r5 locked (out); the SDK wrote 0 to both. Once, from `sub_825894C8`: 0. No longer called since that was dropped (B5 step 3; no override). |
| `sub_825894C8` | | Dropped | Takes the playback from the player's music: below system version `8A100` (the SDK's is 0), in a critical section (`8265D9F4`), reads the controller (`sub_82589408`), keeps whether the player's music had it (`8295777C`), sets it (`sub_82589360`) and marks it taken (`82957778`); 1627 if already taken. Once, from `Boot::StartMovie`, which ignores the answer. Dropped (B5 step 3, `src/hooks/xmp.cpp`): returns 0 and clears the last error, as the SDK's answers left it; harness `xmp`: the same. |
| `sub_825895A0` | | Dropped | Hands the playback back: the same check and critical section; if taken, sets the controller back (`sub_82589360`, 0 or 4 by `8295777C`) and clears both globals; 1627 if not taken. Once, from `Boot::EndMovie`, which returns its answer. Dropped (B5 step 3, `src/hooks/xmp.cpp`): returns 0 and clears the last error; harness `xmp`: the same. |
| `sub_825DA140` | | | Uses the XMP code's critical section (`8265D9F4`); probably sets it up. Not read. |
