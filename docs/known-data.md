# Known data

The game's globals and object fields worked out so far (the functions
are in [`functions/`](functions/functions.md)).

| Address | What | Notes |
|---|---|---|
| vehicle+0x30 | Speed (float, m/s) | Read by `Vehicle::GetSpeedKmh`. |
| vehicle+0xD0 | Vehicle world matrix | 4x4 row-major: rotation rows, then position row (w = 1). The car's forward axis is -row 0. |
| vehicle+0xCF0 | Copy of the world matrix | Identical every tick; not yet known which one drawing reads. |
| `0x826BAC10` | Camera view matrix | Pass 0's view (+0x10). The camera position is -t·Rᵀ. In the car's frame the views sit at (-5.48, 1.34, 0) for chase, (-0.44, 0.67, 0) for bumper and (-0.26, 0.85, -0.38) for cockpit. Writing it has no effect while driving: Draw rebuilds it from the eye and target. |
| `0x826BAC00` | Camera pass array | 21 objects of 592 bytes: view +0x10, projection +0x50, view-projection +0x90, eye +0x190, look-at target +0x1A0, up +0x1B0. Pass settings at +564 to +588, active flag at +588. Pass 0 is the main 1280x720 view; pass 10 a 1024x1024 reflection and 11 a 512x512 shadow map, both rebuilt in Draw. |
| `[0x82814E34]` | Title screen object | +4 state (5 = start the attract race), +36 idle ticks. See "The title screen" in `title-screen.md`. |
| car + 16 | The car's world matrix, for drawing | Copied in by `sub_8215C780` each tick; the cockpit dials' parent. |
| car + 696, + 700, + 704 | Speed (km/h), revs, boost | Set each tick by `sub_8215C780`; the cockpit dials read them (`sub_820E42E8`, `sub_820E45A8`). `sub_820DAC68` takes 1.0 off + 704 every pass. |
| `0x70180000` | Main thread stack | During Draw, holds temporary world matrices and camera copies passed to the recorders (for example 0x7018E640). Not render state. |
| save owner | Save slots' owner (`r3` of the `SaveSlot` steps) | +16 the content's root name, +44 the slots (1,000 bytes each), +52 the device selector's flags, +60 and +64 the save's size and a count (for `sub_82295C18`), +68 how many saves to enumerate. See `functions/files-and-saves.md`. |
| save slot | One save slot (1,000 bytes) | Overlappeds (28 bytes): +8 the device selector, +48 the enumeration, +100 the open, +136 create, delete and thumbnail, +472 the file read or write (its event at +488, also kept at +492). +36 the device (0 for none), +40 its free bytes (64-bit), +76 the enumerator, +80 its buffer (308-byte XCONTENT_DATA each), +88 its size, +164 the XCONTENT_DATA used (+168 its content type), +496 the file handle, +500 bytes done, +504 the display name (UTF-16), +760 the file name, +802 the file's path, +932 and +936 the data and its size, +940 and +944 the thumbnail and its size, +956 the user, +960 and +964 the state the steps share. See `functions/files-and-saves.md`. |
| movie source | The movie player's file data source (vtable `8204DD50`, one per stream) | +44 the handle (-1 closed), +48 the end flag, +56 the start and +64 the size (64-bit), +72 and +80 the blocks' window (start and bytes), +88 the position, +96 a read count, +104 the block size, +108 the block count, +112 the current block's index, +116 the blocks to keep ahead, +120 flags (1 opened by path, 4 the handle was the caller's, 8 loop), +124 the blocks' memory, +128 the current block, +132 the block list. Ours makes no blocks (+108, +124, +132 stay 0). See `functions/files-and-saves.md`. |
