# Known functions: files and saves

The game's functions that call file and save kernel imports, and the
game code around them met along the way: the Win32-style file calls
(CreateFile, ReadFile, WriteFile, SetFilePointer and the rest), the
game's own file layer and the C runtime's low-level I/O above them, the
save slots (the device selector, content create, enumerate, open,
delete and close, the thumbnail), and the graphics library's debug
capture, which mounts a drive of its own. Worked on as B4 in
`docs/rexglue-removal.md`. Rows are sorted by address. The rest of the
game's functions, and the prose sections, are in
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
| `sub_8209AC78` | | | Called by the game's file layer (`sub_8258C608`, `sub_8258C688`, `sub_8258C768`, `sub_8258C820`) and `sub_8209DD00` when a file call fails; uses `sub_8258DE88` and the data at `82600340`. Probably reports the error. Not read. |
| `sub_8209DD00` | | | Starts an asynchronous read with ReadFileEx (`sub_82295450`); `sub_8209AC78` on failure. From `sub_8209E118`, which has no direct callers. Not read in detail. In play: the archives' reads (1,135 in the user's session). |
| `sub_8209E210` | | | Opens a data file: CreateFile (twice), `sub_82295398` (probably VirtualAlloc: `NtAllocateVirtualMemory`) four times, reads through the file layer (`sub_8258C820`, `sub_820A6AB8`). From `sub_82106548`. Not read in detail. In play: opens `game:\DATA\BUILD\TXR\BUILD.DAT` and `BUILD_CRS.DAT`, once. |
| `sub_820A45C0` | `SaveSlot::CheckDevice` | Overridden | r3 = a save slot: when it has a device (+36), asks for the device's data (XContentGetDeviceData, `sub_82295C10`) and clears +36 if that fails (the device was removed); returns +36. From `SaveDevices::Update` (`sub_820A4610`). Ours: the device is never removed; returns +36. The same as the recompiled one in the harness (`saves`). |
| `sub_820A48B8` | `SaveSlot::WaitDeviceSelector` | Overridden | r4 = slot: 4 while the selector's overlapped (+8) is pending (997, or 996 from XGetOverlappedResult); when done, with a device chosen (+36): +960 = 0, +964 = 1, and the device's free bytes (XContentGetDeviceData, +16 of its data) at +40; returns 3. Ours: the free bytes are the SDK's answer (3 GB, fixed), with no device data call. The same as the recompiled one on 6 cases (chosen, pending, incomplete, cancelled, no device). |
| `sub_820A4960` | `SaveSlot::ShowDeviceSelector` | Overridden | r3 = owner, r4 = slot: clears the overlapped (+8, 28 bytes) and the device (+36), and calls XShowDeviceSelectorUI(user +956, content type 1 (saved game), the owner's flags (+52), 0 bytes requested, device +36, overlapped +8); 1 when pending (997), else -13. Ours: no dialog: device 1 is chosen at once and the overlapped (+8) is done. |
| `sub_820A49D0` | `SaveSlot::Enumerate` | Overridden | r3 = owner, r4 = slot: -2 without a device (+36). XContentCreateEnumerator(user +956, device +36, content type 1, flags 0, the owner's count (+68), buffer size out (+88), handle out (+76)); -4 unless the size is count * 308 (an XCONTENT_DATA each). Then XEnumerate(+76, buffer +80, size, 0, overlapped +48); 1 when pending, else -4. Ours: no enumerator (+76 = 0); the one save ("GAME DATA   " and the file's date in UTC, `SystemData.bin`) goes into the buffer when `saves/TXR_DATA_1` exists, and +48 is done with the count. |
| `sub_820A4A88` | `SaveSlot::WaitEnumerate` | Overridden | r4 = slot: the enumeration's wait. XGetOverlappedResult on +48 (the count to +92); done: +984 = the count; 1627: +984 = 0; then CloseHandle on the enumerator (+76) and 1. 0 while pending, -6 for an error (after the close). From `sub_820A59C0`. Ours: the same, with no enumerator to close. The same as the recompiled one on 5 cases. |
| `sub_820A4B18` | `SaveSlot::OpenItem` | Overridden | r3 = owner, r4 = slot, r5 = index: -2 without a device. XContentCreate(user +956, root name owner +16, the index's XCONTENT_DATA in the enumeration buffer (+80 + 308 * index), flags 3 (open existing), overlapped +100); 1 when pending, else -5. Its wait is `sub_820A4B98`. Ours: the open (+100) is done at once: found (0) when `saves/TXR_DATA_1` exists, else 2. |
| `sub_820A4B98` | `SaveSlot::WaitOpenItem` | Overridden | r3 = owner, r4 = slot: when the open (+100) is done, closes it again (XContentClose(owner +16, +100)) and returns 1; 0 while pending, -7 for an error. Probably checks an enumerated save can be opened. Not read in detail. Ours: the close has nothing to do; +100 stays done. |
| `sub_820A4C58` | `SaveSlot::Create` | Overridden | r3 = owner, r4 = slot, r5 = mode: -2 without a device. Mode 3 first checks the size (`sub_82295C18` of the owner's +60 and +64) against the device's free bytes (-10 if short). Builds an XCONTENT_DATA at +164 (device, content type 1, display name from +504 by `sub_82295D48`, file name from +760) and calls XContentCreate(user, owner +16, it, flags 18 for mode 1 (create always, 2, with `0x10`), else 3 (open existing), overlapped +136); 1 when pending, else -5. The save step passes 1, the load step 0. Ours: the content data at +164 built as the game does; the create (+136) done at once: create always succeeds, open existing finds the save only when its file exists. Mode 3 still checks the size (the recompiled `sub_82295C18`) against the fixed free bytes. |
| `sub_820A4D68` | `SaveSlot::WriteFile` | Overridden | r4 = slot: waits for the create (+136; 0 while pending, -7 for an error). Then an event (`sub_82295E78`, at +492; -99 without), CreateFile(the path at +802, write, no sharing, create always, normal and overlapped) at +496 (-3 if it fails), and WriteFile(+496, data +932, size +936, +500, the overlapped at +472 with the event); 1. From the save step `sub_820A5BE8`. In play: `TXR360:\TXR_DATA_1`, 129,400 bytes, once per save. Ours: once the create is done, the data goes to `saves/<the path's file name>` (a temporary file renamed over the old one); +472 done with the size; no file or event handle (+496 = -1, +492 = 0). Checked by the user: saved fresh and over an old save. |
| `sub_820A4EF8` | `SaveSlot::ReadFile` | Overridden | The same as `sub_820A4D68` for reading: CreateFile (open existing, read sharing) and ReadFile. From the load step `sub_820A5E68`. Not read in detail. In play: `TXR360:\TXR_DATA_1`, 129,400 bytes, once at boot. Ours: reads `saves/<name>` into +932 (at most +936 bytes); -3 if it can't be read; no handles. Checked by the user: the progress loaded. |
| `sub_820A5110` | `SaveSlot::SetThumbnail` | Overridden | r4 = slot: -2 without a device. XContentSetThumbnail(user +956, the XCONTENT_DATA at +164, image +940, size +944, overlapped +136); 1 when pending, else -9. Not called in an attract race or the user's session (2026-10-21). Ours: no thumbnail kept; its step (+136) done. |
| `sub_820A51E0` | `SaveSlot::Delete` | Overridden | r4 = slot: -2 without a device. Builds the XCONTENT_DATA at +164 as `sub_820A4C58` does and calls XContentDelete(user +956, it, overlapped +136); 1 when pending, else -12. Not called in an attract race or the user's session (2026-10-21). Ours: deletes `saves/TXR_DATA_1`; its step (+136) done (2 if it's still there). |
| `sub_820A55A8` | | | r3 = owner, r4 = slot index (1,000-byte slots from +44), r5 = an XCONTENT_DATA: XContentGetCreator(the slot's user +956, r5, creator flag and XUID on the stack, no overlapped); returns its answer only. No direct callers (probably a vtable). Not called in an attract race or the user's session (2026-10-21). |
| `sub_820A5BE8` | `SaveSlot::SaveStep` | | The save's state machine (slot state +960 and others): its steps are `sub_820A4C58` (mode 1), `sub_820A4D68`, `sub_820A4E68` (probably the write's wait), `sub_820A5070`, `sub_820A5110`, `sub_820A5190` and XContentClose. No direct callers (probably a vtable). Not read in detail. |
| `sub_820A5E68` | `SaveSlot::LoadStep` | | The load's state machine: `sub_820A4C58` (mode 0), `sub_820A4EF8`, `sub_820A4FF8` (probably the read's wait), `sub_820A5070` and XContentClose. No direct callers (probably a vtable). Not read in detail. |
| `sub_8228FC50` | | | Front end: the save's size (XContentCalculateSize of `0x1F978`, 129,400 bytes) and a look through the enumerated saves (`sub_820A0428`), probably for `SystemData.bin`. From six UI functions (`sub_821A2C98`, `sub_821A33E8`, `sub_821A3728`, `sub_821A4450`, `sub_821BBD18`, `sub_821C2620`). `sub_82290040` is alike (from `sub_82218478`). Not read in detail. |
| `sub_822952E0` | `XShowDeviceSelectorUI` | | `XamShowDeviceSelectorUI` (user, content type, flags, bytes requested, device out, overlapped). From `SaveSlot::ShowDeviceSelector` (`sub_820A4960`; user 0, content type 1, flags `200`): 997, device 1 through the overlapped. Twice in play, around saves. No longer called: its callers (the SaveSlot steps) are native (B4). |
| `sub_822953E8` | | | ReadFileEx's APC (`sub_82295450`): r3 = a completion routine, r4 = an IO_STATUS_BLOCK: for an error status (`0xC...`) calls `routine(RtlNtStatusToDosError(status), 0, block)`, else `routine(0, block+4, block)`. Reached only through the function table. The last recompiled caller of `RtlNtStatusToDosError`. Not read in detail. In play: called straight after each ReadFileEx with the routine `sub_8209D808`. |
| `sub_82295450` | `ReadFileEx` | | r3 = handle, r4 = buffer, r5 = size, r6 = overlapped (offset from +8 and +12), r7 = completion routine: `NtReadFile` with `sub_822953E8` (low bit set) as the APC and r7 as its context; 0 with the last error set for an error status, else 1. From `sub_8209DD00`. In play: 1,134 calls in the user's session, from the archive streaming, each done at once under the SDK (status 0 on return). |
| `sub_82295508` | `CreateFile` | Wrapped | CreateFileA: r3 = path, r4 = access, r5 = sharing, r7 = disposition (1-5 to NT's 2, 5, 1, 3, 4; 5 needs write access), r8 = flags and attributes. The only caller of `NtCreateFile` (root -3, `ObDosDevices`). Errors: 80 for a name collision, 3 or 5 for `C00000BA` (3 when the path ends in `\`); success sets the last error to 183 when create always or open always found the file, else 0. Returns the handle or -1. `--log_file_opens` logs every path. |
| `sub_82295BC8` | `XContentCreate` | | 87 when the XCONTENT_DATA (r5) is missing or has no device (its +0), else `XamContentCreate` (user, root name, data, flags, disposition, license, overlapped). From `SaveSlot::OpenItem` and `SaveSlot::Create`. No longer called: its callers (the SaveSlot steps) are native (B4). |
| `sub_82295BE8` | `XContentDelete` | | `XamContentDelete`. From `SaveSlot::Delete` (`sub_820A51E0`). Not called in an attract race or the user's session (2026-10-21). No longer called: its callers (the SaveSlot steps) are native (B4). |
| `sub_82295BF0` | `XContentClose` | Dropped | `XamContentClose` (root name, overlapped). From `SaveSlot::WaitOpenItem` and the save and load steps. Ours (B4, `src/hooks/saves.cpp`): nothing to close: the overlapped done and r3 = 997 (the SDK's answer), or 0 without an overlapped. The save and load steps call it inline. |
| `sub_82295BF8` | `XContentSetThumbnail` | | `XamContentSetThumbnail`. From `SaveSlot::SetThumbnail` (`sub_820A5110`). Not called in an attract race or the user's session (2026-10-21). No longer called: its callers (the SaveSlot steps) are native (B4). |
| `sub_82295C00` | `XContentGetCreator` | | `XamContentGetCreator`. From `sub_820A55A8`. Not called in an attract race or the user's session (2026-10-21). |
| `sub_82295C08` | `XContentCreateEnumerator` | | `XamContentCreateEnumerator`. From `SaveSlot::Enumerate` (`sub_820A49D0`). No longer called: its callers (the SaveSlot steps) are native (B4). |
| `sub_82295C10` | `XContentGetDeviceData` | | `XamContentGetDeviceData` (device, data out: free bytes at +16). From `SaveSlot::CheckDevice`, `SaveSlot::WaitDeviceSelector` and `SaveSlot::Create`. No longer called: its callers (the SaveSlot steps) are native (B4). |
| `sub_82295C18` | | | r3 = bytes, r4 = a count: a size in 4 KB blocks worked out from them (with an overhead for every 170 blocks), plus 10 blocks. Probably XContentCalculateSize. From `SaveSlot::Create`'s mode 3, and the front end's `sub_8228FC50` and `sub_82290040` (in play: 129,400 bytes give 188,416). |
| `sub_82295C98` | `XGetOverlappedResult` | | r3 = overlapped, r4 = result (out, from +4), r5 = wait: while +0 is 997, waits on the event (+12) when asked (`sub_822970A0`, which returns 258 on a timeout, then 996); returns +0, or the last error. From the save slot steps. |
| `sub_82295D30` | `XEnumerate` | | `XamEnumerate(handle, 0, buffer, size, items, overlapped)`. From `SaveSlot::Enumerate` (`sub_820A49D0`; 997, 3 calls around saves) and the stats enumerators (`sub_821495E8`, `sub_82292CC0`, `sub_82292DB8`), never called. No longer called from the saves (B4); the stats enumerators, never called, are its last callers. |
| `sub_82295D48` | | | Jumps to `sub_825A7F30`. Copies the save slot's display name (wide string). Probably wcscpy. Not read. |
| `sub_82295D50` | `WriteFile` | | r3 = handle, r4 = buffer, r5 = size, r6 = written (out), r7 = overlapped. With an overlapped: +0 = 997 (pending), the offset from +8 and +12, the event from +16 (its low bit set: no APC context), `NtWriteFile`; 1 when done, else 0 with the last error (997 while pending). Without: `NtWriteFile`, waits on the handle when pending (`NtWaitForSingleObjectEx`); 1, or 0 with the last error. From `SaveSlot::WriteFile`, the file layer (`sub_8258C898`), the C runtime's `_write` (`sub_825B0F48`) and the debug capture (`sub_822B4F10`, `sub_822B5208`). |
| `sub_82295FC8` | `ReadFile` | | The same layout as WriteFile, with `NtReadFile`; end of file (`C0000011`) gives 1 and 0 bytes without an overlapped, the error with one. From `SaveSlot::ReadFile`, the file layer (`sub_8258C820`), the C runtime's `_read` (`sub_825B81A0`), `sub_822E1838` and the `824881D0` group. |
| `sub_822965E0` | `GetFileSize` | | r3 = handle, r4 = high (out): GetFileSizeEx (`sub_822972E0`); -1 with the last error set for a failure. From `sub_822E1838`, `sub_8258C688` and `sub_82593B10`. Not read in detail. |
| `sub_822972E0` | `GetFileSizeEx` | | r3 = handle, r4 = size (out, 64-bit): `NtQueryInformationFile` (class 34, network open information; end of file at +40); 1, or 0 with the last error set. From `GetFileSize` and `sub_82487C48`. |
| `sub_822B4760` | | | The debug capture's free space check: GetDiskFreeSpaceEx (`sub_825B9C20`) among much else. Only from `sub_822B58F0`, which has no callers left. Not read in detail. |
| `sub_822B58F0` | | | Mounting: names built with `sprintf`, `ObCreateSymbolicLink`, `CreateFile`, `sub_822B4760`, `sub_825B9D30`, `sub_825B9DD8`, and DeleteFile (`sub_825B9F20`) among others. Only from `sub_822B5C58` (Dropped): no callers left. Not read in detail. |
| `sub_822B8868` | | | GetFileAttributes (`sub_825B9FD8`) and `OutputDebugStringA` among much else. From `sub_822B8D28`. Graphics library debug code, probably. Not read. In play: 14 calls, each asking for `ShaderDumpxe:\CompareBackEnds` (not there). |
| `sub_822BC588` | | | GetFileAttributes (`sub_825B9FD8`) and `sub_822976F8`. From `sub_822BC8A8`. Not read. |
| `sub_822E1838` | | | Loads a whole file: CreateFile (read, read sharing, open existing), GetFileSize, `sub_822976F8` (probably allocates physical memory), ReadFile. From `sub_822B9300`. Not read in detail. |
| `sub_8235E878` | | | Copies a name (`sub_825A7F50`, 260), CreateFile (open existing), GetFileTime (`sub_825BA040`), `sub_82294E20`, `sub_8235DF98`. From `sub_8235F100` and `sub_8235F1E8`. Not read. Not called in an attract race or the user's session (2026-10-21). |
| `sub_82487C48` | `MoviePlayer::OpenFile` | Wrapped | The movie player's file open (r3 = player, through a vtable): CreateFile (open existing) and GetFileSizeEx. Logged with `--log_file_opens`. |
| `sub_824881D0` | | | Reads with ReadFile (`sub_82295FC8`); probably the movie player's reads (next to `MoviePlayer::OpenFile`). No direct callers; `sub_82488598` and `sub_82488A10` are alike (with `sub_82295F20`, a wait). Not read. |
| `sub_8258C608` | `File::OpenWrite` | | r3 = a file (+4 handle, -1 when closed), r4 = path: CreateFile(write, write sharing, create always, normal) into +4; 1, or `sub_8209AC78` and 0. From `sub_820A6A50` (no direct callers). Not called in an attract race or the user's session (2026-10-21). |
| `sub_8258C688` | `File::OpenRead` | | The same with read access, read sharing and open existing, then GetFileSize. From `sub_820A6AB8`. Not read in detail. |
| `sub_8258C768` | `File::Seek` | | SetFilePointer (`sub_825B9DD8`) on +4, the method 0, 1 or 2 by an argument. From `sub_8209DFA8`. Not read in detail. |
| `sub_8258C820` | `File::Read` | | r3 = file, r4 = buffer, r5 = size (pointer): ReadFile on +4, adds the bytes read to the position (+8) and returns them; `sub_8209AC78` and 0 on failure. From `sub_82090CC0`, `sub_8209DFA8` and `sub_8209E210`. |
| `sub_8258C898` | `File::Write` | | WriteFile on the file, then `sub_825A1318` twice. No direct callers. Not read in detail. Not called in an attract race or the user's session (2026-10-21). |
| `sub_82593B10` | | | CreateFile (open existing) and GetFileSize. From `sub_820A6B20` (no direct callers). Not read. |
| `sub_825B0F48` | `_write` | | Probably the C runtime's `_write` (errno 9 and 22, a lock): SetFilePointer (`sub_825B6500`) and WriteFile. From `sub_825B11A8`. Not read in detail. Not called in an attract race or the user's session (2026-10-21). |
| `sub_825B4828` | `_commit` | | Probably the C runtime's `_commit`: FlushFileBuffers (`sub_825BA510`). From `sub_825ACD10`. Not read in detail. Not called in an attract race or the user's session (2026-10-21). |
| `sub_825B6500` | `_lseek` | | Probably the C runtime's `_lseek`: SetFilePointer (`sub_825B9DD8`). From `_write`, `_read` and `sub_825B65D0`. Not read in detail. Not called in an attract race or the user's session (2026-10-21). |
| `sub_825B81A0` | `_read` | | Probably the C runtime's `_read` (text mode handling, errno 9 and 22): ReadFile. From `sub_825B8778`. Not read in detail. Not called in an attract race or the user's session (2026-10-21). |
| `sub_825B9C20` | `GetDiskFreeSpaceEx` | | r3 = path, r4 = free bytes for the caller, r5 = total bytes, r6 = free bytes (each out, 64-bit, optional): `NtOpenFile`, `NtQueryVolumeInformationFile` (class 3, size information: units times sectors times bytes), `NtClose`; 1, or 0 with the last error (2 becomes 3). Only from `sub_822B4760` (the debug capture). Not called in an attract race or the user's session (2026-10-21). |
| `sub_825B9D30` | `SetEndOfFile` | | r3 = handle: the position (`NtQueryInformationFile`, class 14) set as the end of file (class 20) and the allocation size (class 19); 1, or 0 with the last error. From the debug capture (`sub_822B4F10`, `sub_822B58F0`). Not called in an attract race or the user's session (2026-10-21). |
| `sub_825B9DD8` | `SetFilePointer` | | r3 = handle, r4 = low, r5 = high (in and out, optional), r6 = method (0 begin, 1 current, 2 end): `NtQueryInformationFile` for the current position (class 14) or the size (class 34), `NtSetInformationFile` (class 14). Error 131 for a negative position, 87 for one past 4 GB without r5. Returns the new low word (clearing the last error when it's -1), or -1. From `_lseek`, `File::Seek` and the debug capture. |
| `sub_825B9F20` | `DeleteFile` | | r3 = path: `NtOpenFile` for delete (root -3, sharing 7, not a directory) and `NtSetInformationFile` (class 13, disposition: delete), `NtClose`; 1, or 0 with the last error. Only from `sub_822B58F0` (the debug capture, no callers). Not called in an attract race or the user's session (2026-10-21). |
| `sub_825B9FD8` | `GetFileAttributes` | | r3 = path: `NtQueryFullAttributesFile` (root -3, `ObDosDevices`; attributes `0x40`, case-insensitive); returns the attributes, or -1 with the last error set (`sub_82296E88`). From `sub_822B8868` and `sub_822BC588`. In play: only `ShaderDumpxe:\CompareBackEnds`, which fails (-1), from `sub_822B8868`. |
| `sub_825BA040` | `GetFileTime` | | r3 = handle, r4, r5, r6 = creation, last access, last write (out, 64-bit, optional): `NtQueryInformationFile` (class 34); 1, or 0 with the last error. From `sub_8235E878`. Not called in an attract race or the user's session (2026-10-21). |
| `sub_825BA510` | `FlushFileBuffers` | | r3 = handle: `NtFlushBuffersFile`; 1, or 0 with the last error. From `_commit` (`sub_825B4828`). Not called in an attract race or the user's session (2026-10-21). |
