# B7's static map

Made by `itc_recomp/tools/static_map.py` on every run; never edit it by hand (run the tool again). What it reads, how edges are found and its gaps: the tool's docstring and "The static map" in `docs/coverage.md`.

Made 2026-11-15 at commit `2111194e` (`lift-sweep`).

## Totals

- Recompiled `sub_` functions: 16552; 144 jump-table cases folded into their parents, leaving 16408.
- Reachable: 9118 (7398 from `main`, 1720 more from 190 extra roots: functions seen in play that `main` doesn't reach, 2 of them not under another).
- Ours: 9092. Excluded (replaced libraries): 9.
- **Unported: 17** (8 under `main`): seen in play 9, never seen 8; 5998 generated lines.
- Of the unported, 14 hang under unported code (or a root); 3 are reached only through ours (1 seen in play), 3 of them called by our C++ itself (a guest call by name or address, a fallback for a branch, a table or vtable it stores) and the rest under those; the TSV's `behind_ours` column lists them.
- Ours functions' edges come from our C++ (`src_edges.py`): 36892 definitions in 1192 files of `src/` (harnesses left out), 9673 functions with C++ (an override or `ITC_NATIVE_RUNNING`), 9042 of them reachable. Ours functions without C++ (Merged, or whose print names no address) keep the original's edges where something still reaches them.
- Ours functions whose original body is still reachable: 72 (70 from our C++: an `__imp__` fallback when the native C++ declines, or a guest call to a function with neither override nor C++); listed below.
- Host code no native function's C++ reaches but that names guest functions (constants defined but not resolved to a user, a few hooks' helpers): 3030 functions named, taken as roots; they add 1478 reachable functions.
- Before 2026-11-14's fix the graph followed the original's edges through ours too: 15901 reachable. **Originals no longer called: 1659** (980 not ours, 679 ours whose work our C++ took in; 139159 lines, 264 seen in play): reachable through the original's graph (outside the replaced libraries), not through ours; below, by the native function that left the call out.
- Frontier (ready now): 14 functions.
- Reachable functions calling imports: 295 (2 unported).
- Image read: 13492 code-built data addresses scanned; 181 function pointers in data that no code-built address reaches (136 functions; not edges).

A `*` after a function: never seen in play. Sizes are generated lines.

## Blockers, by what they unblock

Unported functions ranked by how many unported functions above them (through unported callers) wait on them; "frees" is how many would join the frontier if this one alone became ours.

| Function | Unblocks | Frees | Lines | Seen | Ready | File |
|---|---|---|---|---|---|---|
| `sub_822AED18` | 3 | 3 | 490 | yes | yes | functions.md |
| `sub_82121760` | 0 | 0 | 647 | no | yes | unmapped |
| `sub_82130978` | 0 | 0 | 1038 | yes | yes | screen-objects.md |
| `sub_821743A8` | 0 | 0 | 657 | no | yes | race-world.md |
| `xstart` | 0 | 0 | 148 | yes | yes | unmapped |
| `sub_822997B0` | 0 | 0 | 416 | yes | yes | system-services.md |
| `sub_8229D6F0` | 0 | 0 | 15 | no | yes | command-buffer.md |
| `sub_8229D720` | 0 | 0 | 62 | no | yes | command-buffer.md |
| `sub_822AFA58` | 0 | 0 | 1105 | yes | yes | functions.md |
| `sub_825A6BD0` | 0 | 0 | 51 | yes | yes | unmapped |
| `sub_825A6C40` | 0 | 0 | 53 | no | yes | unmapped |
| `sub_825BD8F0` | 0 | 0 | 410 | yes | no | functions.md |
| `sub_825BDDA0` | 0 | 0 | 11 | yes | yes | functions.md |
| `sub_825BDDC0` | 0 | 0 | 460 | no | no | functions.md |
| `sub_825BE300` | 0 | 0 | 345 | no | no | functions.md |
| `sub_825BFEA0` | 0 | 0 | 65 | yes | yes | xact.md |
| `sub_825D1C50` | 0 | 0 | 25 | no | yes | xact-waves.md |

## Frontier clusters, by what they unblock

12 clusters (frontier functions sharing an unported caller within 0x4000 bytes, neighbors in address order, or a call cycle). Each: what it unblocks, its functions with their sizes, its function files.

1. **sub_822AED18**-822AED18: unblocks 3; 1 functions, 490 lines, 1 seen; functions.md. `sub_822AED18` 490
2. **sub_8229D6F0**-8229D720: unblocks 0; 2 functions, 77 lines, 0 seen; command-buffer.md. `sub_8229D6F0`* 15, `sub_8229D720`* 62
3. **sub_825A6BD0**-825A6C40: unblocks 0; 2 functions, 104 lines, 1 seen; unmapped. `sub_825A6BD0` 51, `sub_825A6C40`* 53
4. **sub_82121760**-82121760: unblocks 0; 1 functions, 647 lines, 0 seen; unmapped. `sub_82121760`* 647
5. **sub_82130978**-82130978: unblocks 0; 1 functions, 1038 lines, 1 seen; screen-objects.md. `sub_82130978` 1038
6. **sub_821743A8**-821743A8: unblocks 0; 1 functions, 657 lines, 0 seen; race-world.md. `sub_821743A8`* 657
7. **xstart**-rt: unblocks 0; 1 functions, 148 lines, 1 seen; unmapped. `xstart` 148
8. **sub_822997B0**-822997B0: unblocks 0; 1 functions, 416 lines, 1 seen; system-services.md. `sub_822997B0` 416
9. **sub_822AFA58**-822AFA58: unblocks 0; 1 functions, 1105 lines, 1 seen; functions.md. `sub_822AFA58` 1105
10. **sub_825BDDA0**-825BDDA0: unblocks 0; 1 functions, 11 lines, 1 seen; functions.md. `sub_825BDDA0` 11
11. **sub_825BFEA0**-825BFEA0: unblocks 0; 1 functions, 65 lines, 1 seen; xact.md. `sub_825BFEA0` 65
12. **sub_825D1C50**-825D1C50: unblocks 0; 1 functions, 25 lines, 0 seen; xact-waves.md. `sub_825D1C50`* 25

## Unmapped areas

Clusters of unported reachable functions with no rows in any function file: 3 (4 functions, 899 lines). Largest first; the callers named are outside the cluster.

- **sub_825A6BD0**-825A6C40: 2 functions, 104 lines, 1 seen, unblocks 0; called from `sub_825DABC8` (lifted.md, ours), `sub_825DAC10` (lifted.md, ours), `sub_825DB428` (lifted.md, ours), `sub_825DB488` (lifted.md, ours).
- **sub_82121760**-82121760: 1 functions, 647 lines, 0 seen, unblocks 0; called from `sub_82123E48` (race-overlays.md, ours).
- **xstart**-rt: 1 functions, 148 lines, 1 seen, unblocks 0; called from nothing outside it.

## Extra roots

Seen in play but not reachable from `main` (static edges miss them, or they run outside `main`: start-up, threads, callbacks), not under another extra root:

`xstart` (unported), `sub_822ADB50` (ours)

## Originals no longer called

Reachable through the original's graph but not through ours: the native function's C++ left the call out (plugged-in data, a dropped call, work merged into the C++). Grouped by that native function: its original's callees no longer reached, then how many functions hang only under them (a function under several groups counts in each; "not ours" leaves out those ours already, mostly Merged). Candidates for Dropped or "Not called" rows, for the user's review; the TSV's `lost` column lists them all.

| Native caller | File | Status | Lost callees | Under them | Not ours | Lines | Seen |
|---|---|---|---|---|---|---|---|
| `sub_821077A8` | game-modes.md | Overridden | `sub_82106F88`, `sub_82107670` | 813 | 476 | 77425 | 110 |
| `sub_822ACB90` | device-state.md | Native | `sub_822A1280`, `sub_822A12B0`, `sub_822A12F0`, `sub_822A1380`, `sub_822A1410`, `sub_822A14A0`, `sub_822A1530`, `sub_822A15A0`, ... 90 more | 98 | 98 | 739 | 0 |
| `sub_8214E268` | system-services.md | Overridden | `sub_82149080`, `sub_8214AC68`, `sub_8214ADC0`, `sub_8214AEA8`, `sub_8214D7A0`, `sub_8214F160`, `sub_8214F290`, `sub_8214F810`, ... 5 more | 85 | 77 | 7197 | 0 |
| `sub_8209EEC8` | mode-tick.md | Overridden | `sub_8209E4F0`, `sub_8209E540`, `sub_8209E610`, `sub_8209E698`, `sub_8209E730`, `sub_8209E790`, `sub_8209E860`, `sub_8209E9A8`, ... 3 more | 83 | 81 | 7036 | 0 |
| `sub_825D0F00` | xact-tracks.md | Native | `sub_825D0F4C`, `sub_825D1A80`, `sub_825D5F40`, `sub_825D5FE8`, `sub_825D6308`, `sub_825D67E8`, `sub_825D6CF0`, `sub_825D70C8`, ... 3 more | 69 | 68 | 3492 | 1 |
| `sub_8235AAC0` | texture-compression.md | Native | `sub_82365D88`, `sub_82366458`, `sub_82368170`, `sub_8236D380` | 58 | 54 | 5529 | 3 |
| `sub_8216DD88` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_8221F1E8` | 56 | 16 | 4274 | 5 |
| `sub_821A1380` | car-makers.md | Lifted | `sub_8219C528`, `sub_8219C6D0`, `sub_8219C850`, `sub_8219C8F8`, `sub_8219CA08`, `sub_8219CB18`, `sub_8219CBD8`, `sub_8219FC60`, ... 6 more | 28 | 0 | 2045 | 28 |
| `sub_8221BD20` | race-screens.md | Native | `sub_8221A4A8`, `sub_8221A610`, `sub_8221A680`, `sub_8221A6C8`, `sub_8221A720`, `sub_8221B298`, `sub_8221B860`, `sub_8221B8A8` | 28 | 15 | 1869 | 10 |
| `sub_8220F638` | race-screens.md | Native | `sub_8220C578`, `sub_8220C5D0`, `sub_8220D898`, `sub_8220DB98`, `sub_8220E988`, `sub_8220EBD8` | 26 | 10 | 2289 | 15 |
| `sub_825CB470` | xact.md | Overridden | `sub_825C07C8`, `sub_825C95F8`, `sub_825C9A80`, `sub_825CABC8` | 25 | 25 | 1467 | 0 |
| `sub_8222ED98` | location-screen.md | Lifted | `sub_8222A470`, `sub_8222A718`, `sub_8222AD50`, `sub_8222AE00`, `sub_8222B8A8`, `sub_8222BE60`, `sub_8222CFF0`, `sub_8222D108`, ... 10 more | 24 | 0 | 3258 | 23 |
| `sub_825D06B8` | xact-tracks.md | Overridden | `sub_825C07C8`, `sub_825C95F8`, `sub_825C9788`, `sub_825CB3D8` | 24 | 24 | 1421 | 0 |
| `sub_8216DB70` | race-hud-machine.md | Overridden | `sub_82211458` | 22 | 10 | 1721 | 5 |
| `sub_825C2910` | xact.md | Overridden | `sub_825C95F8`, `sub_825C9878` | 22 | 22 | 1352 | 0 |
| `sub_8216C6D8` | race-hud-machine.md | Overridden | `sub_8221EF60` | 20 | 0 | 1703 | 0 |
| `sub_822979E0` | c-runtime-core.md | Lifted | `sub_825A6E48`, `sub_825B6958`, `sub_825D9590` | 20 | 0 | 2679 | 0 |
| `sub_821159B0` | career.md | Overridden | `sub_82110CA8`, `sub_82111140`, `sub_821115C8`, `sub_82112260`, `sub_82115550`, `sub_82115708` | 19 | 19 | 2616 | 0 |
| `sub_822900E0` | save-interface.md | Native | `sub_825A9EC0`, `sub_825AA058` | 19 | 15 | 3084 | 0 |
| `sub_8216CB98` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_82216A40` | 14 | 14 | 1029 | 0 |
| `sub_8216D918` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_82216A40` | 14 | 14 | 1029 | 0 |
| `sub_825A8A50` | c-runtime.md | Overridden | `sub_825AFEC0`, `sub_825AFEF8`, `sub_825B0860`, `sub_825B0968`, `sub_825B0B80` | 14 | 11 | 974 | 0 |
| `sub_825AAD20` | c-runtime.md | Overridden | `sub_825AB1C8`, `sub_825AFEF8`, `sub_825B08E8`, `sub_825B0A40`, `sub_825B0B80` | 14 | 10 | 976 | 0 |
| `sub_8216E5B0` | race-hud-machine.md | Overridden | `sub_8216D328`, `sub_8216D7B8` | 12 | 5 | 1090 | 0 |
| `sub_8259C3A8` | models.md | Native | `sub_8259B748`, `sub_8259BE40` | 11 | 11 | 1838 | 0 |
| `sub_825B13D0` | c-runtime.md | Native | `sub_825AC108`, `sub_825AD4C0`, `sub_825B1368`, `sub_825B6898`, `sub_825B7EB8`, `sub_825BA4A0` | 11 | 6 | 729 | 0 |
| `sub_8216DFD0` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_8221F930`, `sub_8221FFF0` | 10 | 6 | 614 | 0 |
| `sub_825C3050` | lifted.md | Lifted | `sub_825C31D8` | 10 | 2 | 809 | 9 |
| `sub_825C3110` | xact.md | Lifted | `sub_825C31D8` | 10 | 2 | 809 | 9 |
| `sub_82092358` | frame-driver.md | Lifted | `sub_82091A60`, `sub_82091EF8`, `sub_820920D8` | 9 | 5 | 394 | 5 |
| `sub_825AF130` | c-runtime.md | Native | `sub_825AB010`, `sub_825AC108`, `sub_825AD4C0`, `sub_825AF000`, `sub_825AF098`, `sub_825B6890`, `sub_825B6898` | 9 | 3 | 322 | 2 |
| `sub_8216D6D0` | race-hud-machine.md | Overridden | `sub_8221F7D0`, `sub_822200E0` | 8 | 0 | 777 | 0 |
| `sub_820990F0` | input.md | Overridden | `sub_82591918`, `sub_82591C18` | 7 | 2 | 617 | 0 |
| `sub_822A08A0` | functions.md | Overridden | `sub_822A05A8`, `sub_822B1F68`, `sub_822B20D8`, `sub_822B2388`, `sub_822B3FE0`, `sub_822B5C58` | 7 | 2 | 758 | 0 |
| `sub_8211E500` | ui-movies-game.md | Overridden | `sub_8211A6C0`, `sub_8259D5C8`, `sub_825A0CD0`, `sub_825A0CF8`, `sub_825A0EB0`, `sub_825A0EF0` | 6 | 0 | 245 | 0 |
| `sub_8216C210` | race-hud-machine.md | Overridden | `sub_82216850` | 6 | 6 | 366 | 0 |
| `sub_8216C530` | race-hud-machine.md | Overridden | `sub_82216850` | 6 | 6 | 366 | 0 |
| `sub_822968B8` | functions.md | Overridden | `sub_82297678` | 6 | 0 | 613 | 0 |
| `sub_820CDC18` | race-setup.md | Native | `sub_820CC450`, `sub_820CC458`, `sub_820CC460`, `sub_820CC468`, `sub_820CC478` | 5 | 1 | 37 | 4 |
| `sub_8209D130` | mode-tick.md | Overridden | `sub_8217C348`, `__savevmx_124`, `__restvmx_124` | 4 | 1 | 315 | 3 |
| `sub_8221A090` | race-screens.md | Native | `sub_822191D0`, `sub_82219A78`, `sub_82219B60`, `sub_82219C30` | 4 | 2 | 249 | 2 |
| `sub_82299D60` | heap.md | Overridden | `sub_82299208`, `sub_8229A5F4` | 4 | 4 | 704 | 4 |
| `sub_8229A930` | heap.md | Overridden | `sub_82298A88`, `sub_8229B16C` | 4 | 4 | 938 | 4 |
| `sub_82579DA0` | lifted.md | Lifted | `sub_82579D18`, `sub_82579E18`, `sub_8257A498` | 4 | 4 | 436 | 0 |
| `sub_82593088` | std-string.md | Overridden | `sub_82592E18` | 4 | 3 | 622 | 0 |
| `sub_825A6880` | heap.md | Overridden | `sub_820911E8`, `sub_825A67A8`, `sub_825AD150`, `sub_825DB3F8` | 4 | 4 | 99 | 0 |
| `sub_825A8ED0` | c-runtime.md | Overridden | `__savefpr_25`, `__restfpr_25`, `sub_825A8D50`, `sub_825AB1C8` | 4 | 1 | 168 | 2 |
| `sub_820921A8` | frame-driver.md | Lifted | `sub_82091EF8` | 3 | 0 | 114 | 2 |
| `sub_82093BE0` | app-states.md | Overridden | `sub_82091F58` | 3 | 3 | 121 | 0 |
| `sub_8209A458` | framework.md | Overridden | `sub_82091F58` | 3 | 3 | 121 | 0 |
| `sub_820A6F60` | std-string.md | Overridden | `sub_820A6E60`, `sub_825A6A20`, `sub_825A7270` | 3 | 2 | 192 | 0 |
| `sub_820EF168` | car-models.md | Native | `sub_820EF0B8` | 3 | 3 | 174 | 0 |
| `sub_8211C680` | build-screen.md | Overridden | `sub_8211B720`, `sub_8211B770` | 3 | 0 | 85 | 0 |
| `sub_8211CD90` | ui-movies-game.md | Overridden | `sub_8211B720`, `sub_8211B770` | 3 | 0 | 85 | 0 |
| `sub_8212BB20` | sprites.md | Lifted | `__savefpr_21`, `__restfpr_21`, `sub_825A9934` | 3 | 0 | 51 | 2 |
| `sub_8214E190` | system-services.md | Overridden | `sub_82148958`, `sub_82459D40`, `sub_82459D50` | 3 | 0 | 55 | 0 |
| `sub_8216D450` | race-hud-machine.md | Overridden | `sub_822157E8`, `sub_82215870`, `sub_822158E8` | 3 | 3 | 145 | 0 |
| `sub_8216D600` | race-hud-machine.md | Overridden | `sub_8216C798`, `sub_82211390` | 3 | 1 | 142 | 0 |
| `sub_8216DA40` | race-hud-machine.md | Overridden | `sub_8216C798`, `sub_82211390` | 3 | 1 | 142 | 0 |
| `sub_82229238` | location-screen.md | Overridden | `sub_8211AB28`, `sub_8211BE98` | 3 | 0 | 69 | 0 |
| `sub_8229A648` | heap.md | Overridden | `sub_82298548`, `sub_82299448`, `sub_8229A8DC` | 3 | 3 | 581 | 3 |
| `sub_822A6210` | graphics-resources.md | Overridden | `sub_822AD9D8` | 3 | 3 | 86 | 0 |
| `sub_822B0AB0` | functions.md | Overridden | `sub_822B0760`, `sub_822B07B0`, `sub_822B0860` | 3 | 0 | 166 | 0 |
| `sub_825963F0` | models.md | Native | `sub_825ABFE8` | 3 | 2 | 107 | 0 |
| `sub_8259F348` | ui-movies.md | Overridden | `sub_8211A948`, `sub_8259D618`, `sub_8259D628` | 3 | 0 | 83 | 0 |
| `sub_825A1A38` | model-animation.md | Overridden | `sub_825A1938`, `sub_825A6608` | 3 | 3 | 123 | 0 |
| `sub_825A6F60` | c-runtime.md | Native | `sub_825A7044`, `sub_825AA1A8` | 3 | 0 | 27 | 3 |
| `sub_82091720` | std-string.md | Overridden | `sub_82091398`, `sub_825A7270` | 2 | 1 | 142 | 0 |
| `sub_820988C8` | frame-driver.md | Overridden | `sub_8258C998`, `sub_8258C9C0` | 2 | 0 | 20 | 0 |
| `sub_8209E118` | files-and-saves.md | Overridden | `sub_8209DD00`, `sub_822954C8` | 2 | 1 | 63 | 0 |
| `sub_820A4610` | system-services.md | Overridden | `sub_820A45C0` | 2 | 0 | 33 | 0 |
| `sub_820A4C58` | files-and-saves.md | Overridden | `sub_82295BC8`, `sub_82295C10` | 2 | 0 | 18 | 0 |
| `sub_820AB6A0` | camera-modes.md | Native | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_820AF018` | car-physics.md | Native | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_820B1FB8` | car-physics.md | Native | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_820B2808` | car-physics.md | Native | `sub_820B6938`, `sub_820B6940` | 2 | 2 | 11 | 0 |
| `sub_820B3F80` | car-physics.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820B4778` | car-physics.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_820B4AA8` | car-physics.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820BD508` | collision.md | Native | `sub_820BACB8`, `sub_820BCF28` | 2 | 1 | 162 | 0 |
| `sub_820BD6E0` | collision.md | Native | `sub_820BACB8`, `sub_820BCF28` | 2 | 1 | 162 | 0 |
| `sub_820BDA48` | lifted.md | Lifted | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820BE4F0` | lifted.md | Lifted | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_820BE8A8` | lifted.md | Lifted | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_820BF320` | collision.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820C0D28` | car-physics.md | Native | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_820C1E00` | vehicle.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820C5DE8` | car-scene-draw.md | Overridden | `__savefpr_18`, `__restfpr_18` | 2 | 0 | 38 | 2 |
| `sub_820C9170` | car-draw.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820D2330` | material-passes.md | Overridden | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_820D3E98` | scene-loaders.md | Native | `__savevmx_123`, `__restvmx_123` | 2 | 0 | 40 | 2 |
| `sub_820D9F50` | second-set.md | Native | `__savevmx_123`, `__restvmx_123` | 2 | 0 | 40 | 2 |
| `sub_820EE0C8` | car-models.md | Native | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_820F3098` | course-scenery.md | Native | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_820F9768` | track-route.md | Overridden | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_820FC900` | collision-world.md | Native | `__savefpr_15`, `__restfpr_15` | 2 | 0 | 44 | 2 |
| `sub_82106158` | game-modes.md | Overridden | `sub_82105FE0`, `sub_82106100` | 2 | 0 | 57 | 2 |
| `sub_82109EF0` | strings-and-font.md | Overridden | `sub_82109D80` | 2 | 2 | 330 | 0 |
| `sub_8210A680` | strings-and-font.md | Native | `sub_825A7EF8`, `sub_825A7F30` | 2 | 0 | 32 | 0 |
| `sub_8210D478` | car-sounds.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_8210E938` | car-sounds.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_8211B800` | ui-movies-game.md | Native | `sub_8211B310`, `sub_8211B720` | 2 | 0 | 59 | 0 |
| `sub_8211BEE8` | ui-movies-game.md | Overridden | `sub_8211A948`, `sub_8259D628` | 2 | 0 | 76 | 0 |
| `sub_82120008` | lifted.md | Lifted | `__savefpr_15`, `__restfpr_15` | 2 | 0 | 44 | 2 |
| `sub_82120868` | lifted.md | Lifted | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82120BB8` | lifted.md | Lifted | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_82121D78` | lifted.md | Lifted | `__savefpr_15`, `__restfpr_15` | 2 | 0 | 44 | 2 |
| `sub_82123E48` | race-overlays.md | Lifted | `__savefpr_19`, `__restfpr_19` | 2 | 0 | 36 | 2 |
| `sub_82125B60` | race-overlays.md | Overridden | `__savefpr_21`, `__restfpr_21` | 2 | 0 | 32 | 2 |
| `sub_82128750` | sprites.md | Overridden | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_8212B4A8` | race-overlays.md | Native | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_8212D818` | race-overlays.md | Overridden | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_8212F148` | screen-objects.md | Lifted | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_82130618` | lifted.md | Lifted | `__savefpr_18`, `__restfpr_18` | 2 | 0 | 38 | 2 |
| `sub_82131DD8` | screen-objects.md | Native | `sub_8212F030`, `sub_821318B8` | 2 | 2 | 358 | 0 |
| `sub_821329B8` | screen-objects.md | Overridden | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_82132BD8` | screen-objects.md | Native | `sub_8212EFA8`, `sub_82132318` | 2 | 2 | 361 | 0 |
| `sub_82134060` | post-effects.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82136FA8` | post-effects.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82147D88` | scenery-lists.md | Native | `sub_82146BD0` | 2 | 1 | 1052 | 1 |
| `sub_82148058` | scenery-lists.md | Overridden | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_82148658` | sprites.md | Native | `sub_8209F5A0`, `sub_8209F5C8` | 2 | 0 | 21 | 0 |
| `sub_82155E58` | camera-modes.md | Overridden | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_82158F70` | camera-modes.md | Overridden | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82173E28` | race-world.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_82176188` | race-sounds.md | Native | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_8217BA20` | front-end-car-draw.md | Native | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_82185600` | car-dealer.md | Overridden | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_821B4BA0` | car-settings.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_821D8978` | tuning-screen.md | Native | `sub_8211A630`, `sub_8211BA00` | 2 | 0 | 107 | 0 |
| `sub_821E3AD0` | tuning-screen.md | Native | `sub_8211A630`, `sub_8211BA00` | 2 | 0 | 107 | 0 |
| `sub_82211AB0` | camera-modes.md | Native | `sub_8211AB90`, `sub_8211BE98` | 2 | 0 | 35 | 0 |
| `sub_822122B0` | race-hud-object.md | Native | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82212F00` | race-hud-object.md | Native | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82228588` | location-screen.md | Native | `__savefpr_20`, `__restfpr_20` | 2 | 0 | 34 | 2 |
| `sub_82293998` | matrix-math.md | Native | `sub_822936D8` | 2 | 0 | 118 | 0 |
| `sub_82296270` | functions.md | Overridden | `sub_822972D0` | 2 | 2 | 12 | 0 |
| `sub_8229E308` | command-buffer.md | Overridden | `sub_822AD690`, `sub_822AD8F8` | 2 | 2 | 95 | 0 |
| `sub_8229EC00` | command-buffer.md | Native | `sub_822AD690`, `sub_822AD8F8` | 2 | 2 | 95 | 0 |
| `sub_822AD298` | functions.md | Overridden | `sub_822AD190`, `sub_822B2388` | 2 | 0 | 128 | 0 |
| `sub_822B0910` | functions.md | Overridden | `sub_822B07B0`, `sub_822B0860` | 2 | 0 | 142 | 0 |
| `sub_8236A4F0` | texture-compression.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_8236AEA8` | texture-compression.md | Lifted | `__savefpr_20`, `__restfpr_20` | 2 | 0 | 34 | 2 |
| `sub_8236B418` | texture-compression.md | Lifted | `__savefpr_19`, `__restfpr_19` | 2 | 0 | 36 | 2 |
| `sub_8236BF40` | texture-compression.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_823F6C60` | texture-compression.md | Native | `sub_823EAB58`, `sub_823F55B0` | 2 | 2 | 197 | 0 |
| `sub_82420AB8` | texture-compression.md | Native | `__savefpr_15`, `__restfpr_15` | 2 | 0 | 44 | 2 |
| `sub_82420F50` | texture-compression.md | Native | `__savefpr_19`, `__restfpr_19` | 2 | 0 | 36 | 2 |
| `sub_82422178` | texture-compression.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82463B90` | xaudio-objects.md | Native | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_82467A18` | xaudio-objects.md | Native | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_82468848` | xaudio-objects.md | Native | `__savefpr_17`, `__restfpr_17` | 2 | 0 | 40 | 2 |
| `sub_82474578` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82474AE8` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82475200` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82477348` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82481390` | car-sounds.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82481880` | car-sounds.md | Native | `sub_82480780`, `sub_82481708` | 2 | 2 | 196 | 0 |
| `sub_82575B08` | system-services.md | Native | `sub_82459C10` | 2 | 0 | 67 | 1 |
| `sub_82583910` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82587238` | lifted.md | Lifted | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_82590EF0` | models.md | Overridden | `sub_82590D18`, `sub_825981C8` | 2 | 2 | 91 | 0 |
| `sub_825960D0` | scenery-lists.md | Overridden | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82598768` | car-scene-draw.md | Overridden | `__savefpr_18`, `__restfpr_18` | 2 | 0 | 38 | 2 |
| `sub_8259BB98` | lifted.md | Lifted | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_8259ED40` | ui-movies.md | Overridden | `sub_8211A948`, `sub_8259D648` | 2 | 0 | 73 | 0 |
| `sub_825A2CD8` | lifted.md | Lifted | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_825A31F0` | lifted.md | Lifted | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_825A3408` | lifted.md | Lifted | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_825A3980` | lifted.md | Lifted | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_825A3B80` | lifted.md | Lifted | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_825A3E18` | lifted.md | Lifted | `__savefpr_21`, `__restfpr_21` | 2 | 0 | 32 | 2 |
| `sub_825A4680` | lifted.md | Lifted | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_825ACF70` | c-runtime-core.md | Overridden | `sub_825AD02C`, `sub_825AD4C0` | 2 | 0 | 27 | 0 |
| `sub_825B3430` | c-runtime.md | Overridden | `sub_825B32F0` | 2 | 2 | 107 | 0 |
| `sub_825BF7A8` | lifted.md | Lifted | `sub_825BF8D8`, `sub_825BFA68` | 2 | 2 | 253 | 0 |
| `sub_825C27C0` | xact.md | Overridden | `sub_825C35C0` | 2 | 2 | 213 | 0 |
| `sub_825CA980` | xact.md | Overridden | `sub_825D0540` | 2 | 2 | 19 | 0 |
| `sub_825D3F28` | xact-waves.md | Lifted | `sub_825D3FD0`, `sub_825D41F0` | 2 | 2 | 177 | 0 |
| `sub_825D4020` | xact-waves.md | Lifted | `sub_825D3FD0`, `sub_825D41F0` | 2 | 2 | 177 | 0 |
| `sub_82090A08` | framework.md | Lifted | `sub_820902C8` | 1 | 0 | 46 | 1 |
| `sub_82090CC0` | files-and-saves.md | Overridden | `sub_8258BE48` | 1 | 0 | 6 | 1 |
| `sub_82091328` | std-string.md | Overridden | `sub_825A7270` | 1 | 0 | 77 | 0 |
| `sub_82091458` | std-string.md | Native | `sub_825A7270` | 1 | 0 | 77 | 0 |
| `sub_82091540` | std-string.md | Native | `sub_825A6A20` | 1 | 1 | 28 | 0 |
| `sub_82091808` | std-string.md | Overridden | `sub_825A7270` | 1 | 0 | 77 | 0 |
| `sub_82094F30` | lifted.md | Lifted | `sub_82095AF0` | 1 | 1 | 297 | 1 |
| `sub_82095DF8` | lifted.md | Lifted | `sub_82095AF0` | 1 | 1 | 297 | 1 |
| `sub_82098A98` | frame-driver.md | Overridden | `sub_8258C960` | 1 | 0 | 16 | 0 |
| `sub_820998B8` | system-services.md | Lifted | `sub_82099828` | 1 | 0 | 45 | 0 |
| `sub_8209A0E8` | framework.md | Native | `sub_82091CD0` | 1 | 1 | 25 | 0 |
| `sub_8209CC38` | sprites.md | Overridden | `sub_8209C6E0` | 1 | 0 | 475 | 0 |
| `sub_8209F340` | racer-controllers.md | Native | `sub_82591C08` | 1 | 0 | 6 | 0 |
| `sub_820A48B8` | files-and-saves.md | Overridden | `sub_82295C10` | 1 | 0 | 5 | 0 |
| `sub_820A4960` | files-and-saves.md | Overridden | `sub_822952E0` | 1 | 0 | 5 | 0 |
| `sub_820A49D0` | files-and-saves.md | Overridden | `sub_82295C08` | 1 | 0 | 5 | 0 |
| `sub_820A4B18` | files-and-saves.md | Overridden | `sub_82295BC8` | 1 | 0 | 13 | 0 |
| `sub_820A4EF8` | files-and-saves.md | Overridden | `sub_82295FC8` | 1 | 0 | 113 | 0 |
| `sub_820A5110` | files-and-saves.md | Overridden | `sub_82295BF8` | 1 | 0 | 5 | 0 |
| `sub_820A51E0` | files-and-saves.md | Overridden | `sub_82295BE8` | 1 | 0 | 5 | 0 |
| `sub_820A70F8` | lifted.md | Lifted | `sub_820A7218` | 1 | 1 | 358 | 0 |
| `sub_820A7180` | lifted.md | Lifted | `sub_820A7218` | 1 | 1 | 358 | 0 |
| `sub_820A7688` | lifted.md | Lifted | `sub_820A7720` | 1 | 1 | 357 | 0 |
| `sub_820A7AF8` | lifted.md | Lifted | `sub_820A7C50` | 1 | 1 | 379 | 0 |
| `sub_820A7B78` | lifted.md | Lifted | `sub_820A7C50` | 1 | 1 | 379 | 0 |
| `sub_820AEC10` | movies.md | Overridden | `sub_820AE7F8` | 1 | 1 | 8 | 1 |
| `sub_820BCDD0` | collision.md | Native | `sub_820BACB8` | 1 | 0 | 9 | 0 |
| `sub_820BCE80` | collision.md | Native | `sub_820BACB8` | 1 | 0 | 9 | 0 |
| `sub_820BE150` | collision.md | Native | `sub_820BC8A8` | 1 | 1 | 39 | 0 |
| `sub_820CE800` | scene-loaders.md | Overridden | `sub_825A8600` | 1 | 0 | 8 | 0 |
| `sub_820F2D28` | minimap.md | Native | `sub_820F2CA8` | 1 | 1 | 42 | 0 |
| `sub_820F5740` | minimap.md | Overridden | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_820F77A0` | collision-world.md | Lifted | `sub_820F69B0` | 1 | 0 | 27 | 0 |
| `sub_820F9E28` | collision-world.md | Lifted | `sub_820F9050` | 1 | 0 | 166 | 0 |
| `sub_82107B90` | boot-mode.md | Overridden | `sub_825894C8` | 1 | 0 | 75 | 0 |
| `sub_82107BF8` | boot-mode.md | Overridden | `sub_825895A0` | 1 | 0 | 61 | 0 |
| `sub_82109560` | strings-and-font.md | Native | `sub_82108F68` | 1 | 1 | 53 | 0 |
| `sub_82109AA8` | strings-and-font.md | Native | `sub_821097F0` | 1 | 1 | 206 | 0 |
| `sub_8210A490` | strings-and-font.md | Overridden | `sub_825A9A80` | 1 | 0 | 7 | 0 |
| `sub_8210A5E8` | strings-and-font.md | Overridden | `sub_825A7EF8` | 1 | 0 | 20 | 0 |
| `sub_8210A990` | strings-and-font.md | Overridden | `sub_825A7EF8` | 1 | 0 | 20 | 0 |
| `sub_8210ABC0` | strings-and-font.md | Overridden | `sub_825A7EF8` | 1 | 0 | 20 | 0 |
| `sub_8210ADD0` | strings-and-font.md | Overridden | `sub_82109A00` | 1 | 0 | 23 | 1 |
| `sub_82114128` | save-data.md | Lifted | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_82114268` | career.md | Overridden | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_82119B80` | ui-movies.md | Overridden | `sub_8259D628` | 1 | 0 | 10 | 0 |
| `sub_8211AAA8` | ui-movies-game.md | Overridden | `sub_8259D648` | 1 | 0 | 7 | 0 |
| `sub_8211AD00` | ui-movies-game.md | Overridden | `sub_8259D798` | 1 | 0 | 8 | 0 |
| `sub_8211B660` | ui-movies-game.md | Overridden | `sub_8211B310` | 1 | 0 | 30 | 0 |
| `sub_8211B7B8` | ui-movies-game.md | Native | `sub_8211B310` | 1 | 0 | 30 | 0 |
| `sub_8211B940` | ui-movies-game.md | Native | `sub_8211B310` | 1 | 0 | 30 | 0 |
| `sub_8211B9B8` | ui-movies-game.md | Native | `sub_8211B310` | 1 | 0 | 30 | 0 |
| `sub_8211D880` | ui-movies-game.md | Overridden | `sub_8211C930` | 1 | 0 | 49 | 1 |
| `sub_8211D8D8` | ui-movies-game.md | Overridden | `sub_8211C9C0` | 1 | 0 | 31 | 1 |
| `sub_8211D978` | ui-movies-game.md | Overridden | `sub_8259D670` | 1 | 0 | 9 | 0 |
| `sub_8211DBE0` | ui-movies-game.md | Overridden | `sub_8211B298` | 1 | 0 | 40 | 0 |
| `sub_8211DCE8` | ui-movies-game.md | Native | `sub_8211B298` | 1 | 0 | 40 | 0 |
| `sub_8211E1C0` | ui-movies-game.md | Overridden | `sub_8259D930` | 1 | 0 | 81 | 0 |
| `sub_8211E378` | ui-movies-game.md | Overridden | `sub_8259D670` | 1 | 0 | 9 | 0 |
| `sub_8211E808` | ui-movies-game.md | Overridden | `sub_8211A9E8` | 1 | 0 | 52 | 0 |
| `sub_8211F300` | race-overlays.md | Overridden | `sub_8211F3B4` | 1 | 0 | 5 | 0 |
| `sub_82129A08` | sprites.md | Lifted | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_8212ACF0` | sprites.md | Lifted | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_8212C6D0` | sprites.md | Lifted | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_8212DFB8` | sprites.md | Lifted | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_82134410` | lifted.md | Lifted | `sub_82134698` | 1 | 1 | 729 | 0 |
| `sub_82134618` | lifted.md | Lifted | `sub_82134698` | 1 | 1 | 729 | 0 |
| `sub_82148650` | scenery-lists.md | Native | `sub_8209BD40` | 1 | 0 | 6 | 1 |
| `sub_82167610` | race-states.md | Overridden | `sub_821653E8` | 1 | 1 | 85 | 0 |
| `sub_8216D198` | race-hud-machine.md | Overridden | `sub_8221C168` | 1 | 0 | 44 | 0 |
| `sub_821800B0` | type6-type7-modes.md | Native | `sub_8217D760` | 1 | 0 | 37 | 1 |
| `sub_82180F20` | car-dealer.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821880D0` | type2-dialog.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821890D8` | race-menu.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8218C678` | race-menu.md | Overridden | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_8218E420` | type2-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82191D70` | garage-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82193B10` | garage-screen.md | Native | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_82193BA8` | garage-screen.md | Native | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_821955E0` | garage-screen.md | Native | `sub_8211A630` | 1 | 0 | 15 | 0 |
| `sub_821982F0` | type2-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82199688` | type2-screens.md | Native | `sub_8211A630` | 1 | 0 | 15 | 0 |
| `sub_8219BE98` | type2-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821A26F0` | save-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821AA490` | car-settings.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821B8900` | type2-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821BBBA0` | options-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821CE9E8` | tuning-machine.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821DF350` | power-graph.md | Overridden | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_821E6C50` | tuning-screen.md | Overridden | `sub_821E6F90` | 1 | 0 | 78 | 1 |
| `sub_821E9488` | tuning-pages.md | Native | `sub_8259D628` | 1 | 0 | 10 | 0 |
| `sub_821EA130` | tuning-pages.md | Native | `sub_8259D628` | 1 | 0 | 10 | 0 |
| `sub_821EAE68` | tuning-pages.md | Native | `sub_8259D628` | 1 | 0 | 10 | 0 |
| `sub_82206A50` | tuning-pages.md | Native | `sub_82200DC0` | 1 | 0 | 418 | 1 |
| `sub_82206F48` | tuning-pages.md | Native | `sub_822012E8` | 1 | 0 | 268 | 0 |
| `sub_82207320` | tuning-pages.md | Native | `sub_82201620` | 1 | 0 | 320 | 0 |
| `sub_8220B070` | user-info.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82211980` | camera-modes.md | Native | `sub_8211AB90` | 1 | 0 | 8 | 0 |
| `sub_822129A8` | race-hud-object.md | Native | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_822135B0` | race-hud-object.md | Native | `sub_8211AB90` | 1 | 0 | 8 | 0 |
| `sub_822141A0` | race-hud-object.md | Native | `sub_8211AB90` | 1 | 0 | 8 | 0 |
| `sub_822141E8` | race-hud.md | Overridden | `sub_82212908` | 1 | 0 | 27 | 1 |
| `sub_82218C10` | race-screens.md | Overridden | `sub_82218838` | 1 | 0 | 72 | 1 |
| `sub_82220C18` | title-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82221FC8` | title-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82226978` | type5-mode.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82230790` | type67-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82232910` | type6-type7-modes.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82233B18` | type6-type7-modes.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82236FD8` | type67-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82242388` | type6-type7-modes.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82242F70` | type67-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82244080` | type6-type7-modes.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82247AA0` | data-load.md | Native | `sub_8211A9E8` | 1 | 0 | 52 | 0 |
| `sub_8224FB20` | car-list.md | Native | `sub_8211A630` | 1 | 0 | 15 | 0 |
| `sub_82250D70` | power-graph-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8225A930` | todays-result.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8225C160` | todays-result.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82289570` | type10-mode.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8228F408` | scenery-lists.md | Overridden | `sub_8228F484` | 1 | 0 | 157 | 1 |
| `sub_822912A0` | data-load.md | Native | `sub_8211A9E8` | 1 | 0 | 52 | 0 |
| `sub_82291328` | data-load.md | Overridden | `sub_8259D798` | 1 | 0 | 8 | 0 |
| `sub_82293988` | matrix-math.md | Overridden | `sub_82293598` | 1 | 0 | 5 | 0 |
| `sub_82295890` | c-runtime.md | Overridden | `sub_82296FE0` | 1 | 0 | 74 | 1 |
| `sub_82295D48` | files-and-saves.md | Native | `sub_825A7F30` | 1 | 0 | 12 | 0 |
| `sub_822977B0` | frame-driver.md | Native | `sub_82296E68` | 1 | 1 | 5 | 0 |
| `sub_82297888` | frame-driver.md | Native | `sub_82296E68` | 1 | 1 | 5 | 0 |
| `sub_82297CE0` | system-services.md | Overridden | `sub_82297C08` | 1 | 0 | 66 | 0 |
| `sub_8229E100` | command-buffer.md | Native | `sub_8229DC28` | 1 | 0 | 55 | 1 |
| `sub_8229E848` | command-buffer.md | Overridden | `sub_8229DFF8` | 1 | 1 | 48 | 0 |
| `sub_8229EDA8` | command-buffer.md | Native | `sub_822AD850` | 1 | 1 | 58 | 0 |
| `sub_8229EF48` | command-buffer.md | Overridden | `sub_8229E5D0` | 1 | 0 | 119 | 1 |
| `sub_822A0718` | functions.md | Lifted | `sub_822A0688` | 1 | 1 | 43 | 0 |
| `sub_822A3A20` | device-state.md | Native | `sub_822B0C38` | 1 | 0 | 92 | 0 |
| `sub_822A63F8` | graphics-resources.md | Native | `sub_8229DDD8` | 1 | 0 | 55 | 1 |
| `sub_822AC918` | functions.md | Lifted | `sub_822AC5A0` | 1 | 1 | 7 | 0 |
| `sub_822B6BF8` | predicated-tiling.md | Overridden | `sub_8229E5D0` | 1 | 0 | 119 | 1 |
| `sub_8235A990` | models.md | Native | `sub_822A5B68` | 1 | 0 | 7 | 1 |
| `sub_82459D60` | xaudio-engine.md | Overridden | `sub_8245C638` | 1 | 0 | 9 | 0 |
| `sub_8245C650` | audio.md | Overridden | `sub_82295728` | 1 | 1 | 127 | 0 |
| `sub_8245C958` | audio.md | Overridden | `sub_8245D118` | 1 | 1 | 10 | 0 |
| `sub_82465258` | xaudio-objects.md | Native | `sub_82467A00` | 1 | 0 | 12 | 0 |
| `sub_824696F0` | xaudio-objects.md | Native | `sub_82469728` | 1 | 1 | 48 | 0 |
| `sub_8246AEE0` | audio.md | Lifted | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_8246F140` | xaudio-objects.md | Overridden | `sub_8246F270` | 1 | 1 | 39 | 0 |
| `sub_8258BD28` | second-set.md | Native | `sub_8258B7E0` | 1 | 1 | 147 | 0 |
| `sub_8258C820` | files-and-saves.md | Overridden | `sub_82295FC8` | 1 | 0 | 113 | 0 |
| `sub_8258DF18` | frame-driver.md | Overridden | `sub_825BA158` | 1 | 0 | 7 | 1 |
| `sub_82594570` | models.md | Native | `sub_825981C8` | 1 | 1 | 62 | 0 |
| `sub_82595550` | models.md | Overridden | `sub_825A1148` | 1 | 1 | 54 | 0 |
| `sub_82598288` | car-scene-draw.md | Native | `sub_825981C8` | 1 | 1 | 62 | 0 |
| `sub_8259F700` | ui-movies.md | Native | `sub_8259EC58` | 1 | 1 | 84 | 0 |
| `sub_825A16B0` | model-animation.md | Lifted | `sub_825A1708` | 1 | 1 | 37 | 0 |
| `sub_825A5020` | lifted.md | Lifted | `sub_825A5610` | 1 | 1 | 131 | 0 |
| `sub_825A6948` | c-runtime-core.md | Native | `sub_825AD4C0` | 1 | 0 | 6 | 0 |
| `sub_825A71B0` | c-runtime-core.md | Overridden | `sub_825A70F0` | 1 | 0 | 65 | 0 |
| `sub_825A82C8` | c-runtime.md | Native | `sub_825AFD98` | 1 | 1 | 16 | 0 |
| `sub_825A96A0` | c-runtime-core.md | Native | `sub_825AD4C0` | 1 | 0 | 6 | 0 |
| `sub_825A9A90` | heap.md | Overridden | `sub_825AD150` | 1 | 1 | 26 | 0 |
| `sub_825AC248` | heap.md | Native | `sub_825AD150` | 1 | 1 | 26 | 0 |
| `sub_825ACD90` | c-runtime-core.md | Native | `sub_825ACEB4` | 1 | 0 | 16 | 0 |
| `sub_825AD280` | c-runtime-core.md | Native | `sub_825AD488` | 1 | 0 | 21 | 0 |
| `sub_825AEBC8` | heap.md | Overridden | `sub_825AD150` | 1 | 1 | 26 | 0 |
| `sub_825AEE28` | c-runtime-core.md | Overridden | `sub_825AD4C0` | 1 | 0 | 6 | 0 |
| `sub_825B11A8` | c-runtime-core.md | Overridden | `sub_825B1304` | 1 | 0 | 21 | 0 |
| `sub_825B40B8` | c-runtime-core.md | Overridden | `sub_825AD4C0` | 1 | 0 | 6 | 0 |
| `sub_825B4828` | files-and-saves.md | Overridden | `sub_825B4978` | 1 | 0 | 21 | 0 |
| `sub_825B65D0` | c-runtime-core.md | Overridden | `sub_825B6730` | 1 | 0 | 21 | 0 |
| `sub_825BFCC8` | lifted.md | Lifted | `sub_825BFCFC` | 1 | 1 | 44 | 0 |
| `sub_825C09A8` | sound-cues.md | Lifted | `sub_82295FC8` | 1 | 0 | 113 | 0 |
| `sub_825C4D90` | xact.md | Overridden | `sub_825C4DC8` | 1 | 1 | 18 | 0 |
| `sub_825C7F98` | xact.md | Overridden | `sub_825C7FE8` | 1 | 1 | 53 | 0 |
| `sub_825C8230` | xact.md | Overridden | `sub_825C7B10` | 1 | 1 | 32 | 0 |
| `sub_825C8C98` | lifted.md | Lifted | `sub_825C8CD4` | 1 | 1 | 65 | 0 |
| `sub_825CC1D0` | xact.md | Overridden | `sub_825CC148` | 1 | 1 | 56 | 0 |
| `sub_825CC9C8` | xact.md | Overridden | `sub_825CC858` | 1 | 1 | 48 | 0 |
| `sub_825CD6A8` | lifted.md | Lifted | `sub_825CD6EC` | 1 | 1 | 22 | 0 |
| `sub_825CDEA8` | lifted.md | Lifted | `sub_825CE05C` | 1 | 1 | 176 | 0 |
| `sub_825CE450` | lifted.md | Lifted | `sub_825CE49C` | 1 | 1 | 25 | 0 |
| `sub_825D3178` | xact-waves.md | Lifted | `sub_825D31D8` | 1 | 1 | 102 | 0 |
| `sub_825D3428` | xact-waves.md | Lifted | `sub_825D3470` | 1 | 1 | 32 | 0 |

## Ours functions whose original body is still reachable

Ours by their rows, but something reaches their recompiled body: an `__imp__` fallback in our C++ (taken when the native C++ declines: a block not in our heap, a flag of the callee's off), a guest call to a function with no override and no C++, or an unported caller of one without an override. Each with its callers in that variant (native ones marked `n`):

`sub_82099B18` (Overridden; from sub_82090108 n, sub_82091C48 n, sub_820922B8 n ...), `sub_8209A080` (Overridden; from sub_82090108 n), `sub_8209BD50` (Overridden; from sub_820E42E8 n, sub_820E45A8 n), `sub_8209F738` (Native; from sub_8209F9B8 n), `sub_820A2D30` (Overridden; from sub_820E42E8 n, sub_820E45A8 n), `sub_820A8F80` (Merged; from sub_820A9198), `sub_820A9198` (Merged; from sub_820A9200 n), `sub_820AC7F0` (Overridden; from sub_820AC8A0 n), `sub_820AC8A0` (Overridden; from sub_820AC7F0 n), `sub_820B18B8` (Native; from sub_820B6498, sub_820B6498 n), `sub_820B2100` (Native; from sub_820B6498, sub_820B6498 n), `sub_820B5670` (Overridden; from sub_820C3150 n, sub_820C33B8 n), `sub_820B5B08` (Overridden; from sub_820C3150 n, sub_820C33B8 n), `sub_820B5FD8` (Overridden; from sub_820BFD30, sub_820BFD30 n, sub_820C3020 n), `sub_820B6498` (Overridden; from sub_820C3020 n, sub_821706E8 n, sub_821724F8 n), `sub_820B7998` (Overridden; from sub_820C3608, sub_820C3608 n, sub_821046F8 n ...), `sub_820B81C8` (Native; from sub_820B8548 n), `sub_820BB758` (Overridden; from sub_820B7998, sub_820B7998 n, sub_820B92C8 n), `sub_820BBE30` (Overridden; from sub_820B87B0 n), `sub_820BFD30` (Native; from sub_820C3020 n), `sub_820C0118` (Overridden; from sub_82171560 n), `sub_820C1D08` (Overridden; from sub_820C3150 n, sub_820C33B8 n, sub_821046F8 n ...), `sub_820C20E0` (Overridden; from sub_82170668 n, sub_82171978 n, sub_82171E50 n ...), `sub_820C23E8` (Overridden; from sub_82165E20 n, sub_821662E0 n, sub_82166E50 n ...), `sub_820C27E8` (Overridden; from sub_820C3150 n, sub_820C33B8 n, sub_820CD5C8 n ...), `sub_820C3608` (Overridden; from sub_821046F8 n, sub_82104898 n, sub_82104D40 n ...), `sub_820C3B78` (Overridden; from sub_82171250 n, sub_82171308 n), `sub_820C3BD0` (Overridden; from sub_82173A90 n), `sub_820C5C88` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820DC448 n ...), `sub_820CC470` (Merged; from sub_8224B660 n, sub_8224B708 n), `sub_820D09A0` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820C9608 n ...), `sub_820D3318` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_820D3688` (Native; from sub_820E45A8 n), `sub_820D3740` (Native; from sub_820E42E8 n), `sub_820D47A8` (Overridden; from sub_820D4F80 n), `sub_820D4F80` (Overridden; from sub_820D47A8 n), `sub_820D5890` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D6108` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_820D63A0` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D6A70` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D7428` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D78B8` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D7D38` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_820D8908` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D9570` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820F0070` (Native; from sub_820F0820 n), `sub_820F4128` (Native; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D06F0 n ...), `sub_820FEFB0` (Overridden; from sub_82171250 n), `sub_820FFD88` (Overridden; from sub_820C5DE8 n, sub_820F5740 n, sub_82104F68 n ...), `sub_8211AED0` (Overridden; from sub_8211CD90 n, sub_8211E258 n, sub_8211E370 n ...), `sub_8211BD10` (Overridden; from sub_8211E500 n), `sub_82128E70` (Overridden; from sub_82129280 n), `sub_8213B1E0` (Overridden; from sub_82173A90 n), `sub_8213B2B8` (Overridden; from sub_82171250 n, sub_82171308 n), `sub_82150350` (Native; from sub_821561F8 n), `sub_821715B8` (Native; from sub_8213B1E0 n, sub_82173A90 n), `sub_8217C368` (Merged; from sub_82096418 n), `sub_82294330` (Merged; from sub_820B81C8), `sub_82295330` (Overridden; from sub_8209DD98 n, sub_8209E118 n), `sub_822A5B78` (Overridden; from sub_8211E500 n), `sub_822A6008` (Overridden; from sub_82098940 n, sub_822A8A60 n, sub_822AD298 n), `sub_822A8740` (Native; from sub_820C52A0 n, sub_820D6528 n, sub_820D7578 n ...), `sub_82594D10` (Native; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_82596920` (Overridden; from sub_820F0820 n), `sub_825986B8` (Native; from sub_8209CC38 n, sub_820C5DE8 n, sub_820F5740 n ...), `sub_825998A0` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_8259F770` (Native; from sub_8259FA00 n), `sub_8259FB30` (Native; from sub_825A0548 n), `sub_8259FC18` (Native; from sub_825A0548 n), `sub_825A7068` (Overridden; from sub_820AB358 n, sub_82152338 n, sub_82155288 n ...), `sub_825A70E8` (Overridden; from sub_82090148 n, sub_82090190 n, sub_820901D8 n ...), `sub_825A7410` (Overridden; from sub_8209CC38 n, sub_820AC7F0 n, sub_820AC8A0 n ...)

## Reached past the movie library's end (82575B08-8257FFFF)

`b7-loop.md` ends the movie library's first range at 82575B07; `movies.md` at 8257FFFF. Reachable there: 144 functions (144 ours, 0 unported, 0 reached only through the replaced libraries). Entered from 20 functions outside it:

- `sub_82148868` (ours, system-services.md): `sub_82575B08` (ours).
- `sub_82148D00` (ours, lifted.md): `sub_825765F0` (ours).
- `sub_82149178` (ours, system-services.md): `sub_82576E78` (ours).
- `sub_8214E678` (ours, lifted.md): `sub_82576080` (ours).
- `sub_8214E6C8` (ours, lifted.md): `sub_82575E50` (ours).
- `sub_8214EC80` (ours, lifted.md): `sub_825763D0` (ours).
- `sub_8214ED30` (ours, lifted.md): `sub_82576080` (ours), `sub_825764A8` (ours), `sub_82576540` (ours).
- `sub_822917B8` (ours, lifted.md): `sub_82575E50` (ours).
- `sub_82291858` (ours, lifted.md): `sub_825761D8` (ours).
- `sub_82291920` (ours, lifted.md): `sub_825764A8` (ours).
- `sub_822919A0` (ours, lifted.md): `sub_82576540` (ours).
- `sub_82291A40` (ours, lifted.md): `sub_82576328` (ours).
- `sub_82291AD0` (ours, lifted.md): `sub_82576080` (ours).
- `sub_82292B28` (ours, lifted.md): `sub_825766E0` (ours).
- `sub_82292E98` (ours, lifted.md): `sub_82576080` (ours).
- `sub_82293028` (ours, lifted.md): `sub_82576BE0` (ours), `sub_82576F30` (ours).
- `sub_82293190` (ours, lifted.md): `sub_82576080` (ours).
- `sub_8245A410` (ours, sound-cues.md): `sub_82578AF0` (ours).
- `sub_82580020` (ours, lifted.md): `sub_8257FF68` (ours).
- `sub_82580028` (ours, lifted.md): `sub_8257FF68` (ours).

Each function there, with its callers: `sub_82575B08` (ours) from `sub_82148868`; `sub_82575B68` (ours) from `sub_82576BE0`; `sub_82575BF0` (ours) from `sub_82576BE0`; `sub_82575C90` (ours) from `sub_82576BE0`; `sub_82575D78` (ours) from `sub_82576BE0`; `sub_82575E50` (ours) from `sub_8214E6C8`, `sub_822917B8`; `sub_82576080` (ours) from `sub_8214E678`, `sub_8214ED30`, `sub_82291AD0` ...; `sub_825761D8` (ours) from `sub_82291858`; `sub_82576328` (ours) from `sub_82291A40`; `sub_825763D0` (ours) from `sub_8214EC80`; `sub_825764A8` (ours) from `sub_8214ED30`, `sub_82291920`; `sub_82576540` (ours) from `sub_8214ED30`, `sub_822919A0`; `sub_825765F0` (ours) from `sub_82148D00`; `sub_825766E0` (ours) from `sub_82292B28`; `sub_825767A0` (ours) from `sub_82576BE0`; `sub_82576BE0` (ours) from `sub_82293028`; `sub_82576E78` (ours) from `sub_82149178`; `sub_82576F30` (ours) from `sub_82293028`; `sub_82577088` (ours) from `sub_82577850`, `sub_825778D8`; `sub_825770B8` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82577110` (ours) from `sub_825786B8`; `sub_82577140` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82577190` (ours) from `sub_825778D8`; `sub_825774D0` (ours) from `sub_82575BF0`, `sub_82577630`; `sub_82577528` (ours) from `sub_825778D8`, `sub_825786B8`; `sub_82577560` (ours) from `sub_82575B08`; `sub_82577630` (ours) from `sub_82577788`; `sub_825776A8` (ours) from `sub_82575C90`; `sub_82577788` (ours) from `sub_82575D78`; `sub_82577850` (ours) from `sub_825778D8`; `sub_825778D8` (ours) from `sub_825780A0`; `sub_825780A0` (ours) from `sub_82575D78`; `sub_825780D0` (ours) from `sub_82577630`, `sub_825776A8`, `sub_82577788` ...; `sub_825780F0` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82578118` (ours) from `sub_82578168`, `sub_825784A8`, `sub_82578530` ...; `sub_82578168` (ours) from `sub_82578500`; `sub_82578248` (ours) from `sub_825778D8`, `sub_825787D0`; `sub_825782C0` (ours) from `sub_825778D8`, `sub_825787D0`; `sub_82578340` (ours) from `sub_825778D8`, `sub_825787D0`; `sub_825783C8` (ours) from `sub_825778D8`, `sub_825787D0`; `sub_82578470` (ours) from `sub_825778D8`; `sub_825784A8` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82578500` (ours) from `sub_825770B8`, `sub_82577140`, `sub_82577190` ...; `sub_82578530` (ours) from `sub_82577088`, `sub_825770B8`, `sub_82577110` ...; `sub_82578580` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82578588` (ours) from `sub_825778D8`; `sub_82578608` (ours) from `sub_825778D8`, `sub_825786B8`; `sub_82578658` (ours) from `sub_825778D8`; `sub_825786B8` (ours) from `sub_825778D8`; `sub_825787D0` (ours) from `sub_825778D8`; `sub_825788C0` (ours) from `sub_82578AF0`; `sub_82578AF0` (ours) from `sub_8245A410`; `sub_82578C88` (ours) from ; `sub_82578CB8` (ours) from ; `sub_82578CD0` (ours) from `sub_82578D70`, `sub_82578F00`, `sub_825791B0`; `sub_82578D70` (ours) from `sub_825790F0`; `sub_82578F00` (ours) from `sub_825790F0`; `sub_825790F0` (ours) from ; `sub_82579108` (ours) from `sub_82579630`; `sub_825791B0` (ours) from ; `sub_82579280` (ours) from `sub_825796C0`; `sub_82579318` (ours) from ; `sub_82579448` (ours) from ; `sub_82579590` (ours) from `sub_8257F198`; `sub_82579630` (ours) from ; `sub_825796C0` (ours) from `sub_82579798`; `sub_82579798` (ours) from ; `sub_825797B0` (ours) from ; `sub_825797C0` (ours) from `sub_82579968`; `sub_82579830` (ours) from ; `sub_82579880` (ours) from ; `sub_82579898` (ours) from ; `sub_82579968` (ours) from ; `sub_82579A88` (ours) from ; `sub_82579BE8` (ours) from `sub_82579C48`; `sub_82579C48` (ours) from `sub_82579CD0`; `sub_82579CD0` (ours) from ; `sub_82579CF0` (ours) from ; `sub_82579D00` (ours) from ; `sub_82579D40` (ours) from ; `sub_82579D60` (ours) from ; `sub_82579D88` (ours) from ; `sub_82579DA0` (ours) from ; `sub_8257A3D0` (ours) from `sub_82579A88`; `sub_8257A3F0` (ours) from `sub_82579BE8`; `sub_8257B9A8` (ours) from `sub_82579630`, `sub_82579898`; `sub_8257B9D0` (ours) from `sub_82579108`, `sub_82579830`; `sub_8257B9F8` (ours) from ; `sub_8257BA88` (ours) from ; `sub_8257BAA0` (ours) from ; `sub_8257BAD0` (ours) from ; `sub_8257BB00` (ours) from ; `sub_8257BB28` (ours) from ; `sub_8257BB50` (ours) from ; `sub_8257BB60` (ours) from `sub_8257BBC8`, `sub_8257BE48`; `sub_8257BBC8` (ours) from ; `sub_8257BC48` (ours) from ; `sub_8257BE48` (ours) from ; `sub_8257BEC0` (ours) from ; `sub_8257C160` (ours) from `sub_82578CD0`, `sub_825797C0`, `sub_82579A88` ...; `sub_8257C2B0` (ours) from `sub_8257C3E0`; `sub_8257C3E0` (ours) from `sub_8257EDE8`; `sub_8257C570` (ours) from `sub_82578F00`, `sub_8257B9F8`, `sub_8257CCD8`; `sub_8257C590` (ours) from `sub_8257BC48`, `sub_8257BEC0`, `sub_8257C2B0`; `sub_8257C610` (ours) from `sub_8257C160`; `sub_8257C6B0` (ours) from `sub_8257C3E0`; `sub_8257C710` (ours) from `sub_82579280`, `sub_82579BE8`, `sub_8257CFA8`; `sub_8257C858` (ours) from `sub_825791B0`, `sub_82579A88`, `sub_8257D518`; `sub_8257C948` (ours) from `sub_8257C9A0`, `sub_8257D298`; `sub_8257C9A0` (ours) from `sub_8257D020`; `sub_8257CA10` (ours) from `sub_8257D298`; `sub_8257CB38` (ours) from `sub_8257D298`; `sub_8257CC08` (ours) from `sub_8257D298`; `sub_8257CCD8` (ours) from `sub_8257D298`; `sub_8257CF10` (ours) from `sub_8257D020`; `sub_8257CFA8` (ours) from `sub_82579280`, `sub_8257D518`; `sub_8257D020` (ours) from `sub_82578F00`; `sub_8257D298` (ours) from `sub_82579590`; `sub_8257D460` (ours) from `sub_82579630`; `sub_8257D518` (ours) from `sub_825791B0`; `sub_8257D6D8` (ours) from `sub_82579630`; `sub_8257D6F8` (ours) from `sub_82579108`; `sub_8257E8E8` (ours) from `sub_8257F070`, `sub_8257F1E8`; `sub_8257E9B8` (ours) from `sub_8257F070`; `sub_8257EAF0` (ours) from `sub_8257EF80`, `sub_8257EFF8`, `sub_8257F198`; `sub_8257EB68` (ours) from `sub_8257F070`; `sub_8257EDE8` (ours) from `sub_82579BE8`; `sub_8257EF80` (ours) from `sub_82579898`; `sub_8257EFF8` (ours) from `sub_82579830`; `sub_8257F070` (ours) from `sub_82579898`, `sub_82579A88`; `sub_8257F198` (ours) from `sub_82579968`; `sub_8257F1E8` (ours) from `sub_82579898`, `sub_82579A88`; `sub_8257F388` (ours) from `sub_82579A88`; `sub_8257F5E8` (ours) from `sub_8257BC48`, `sub_8257BEC0`; `sub_8257F970` (ours) from `sub_8257FA90`; `sub_8257FA90` (ours) from `sub_8257BC48`; `sub_8257FB38` (ours) from `sub_8257FBF8`, `sub_8257FC00`; `sub_8257FBF8` (ours) from `sub_8257BB00`, `sub_8257BBC8`; `sub_8257FC00` (ours) from `sub_8257FD18`; `sub_8257FD18` (ours) from `sub_8257BBC8`; `sub_8257FD48` (ours) from `sub_8257FEB8`; `sub_8257FEB8` (ours) from `sub_8257BEC0`; `sub_8257FF50` (ours) from `sub_8257BEC0`; `sub_8257FF68` (ours) from `sub_82580020`, `sub_82580028`.

## Our C++ naming replaced-library functions

Tables and vtables our C++ stores whose words land in the replaced libraries, and calls into `sub_822B8F10` (the HLSL compiler's entry, overridden by the stored shaders); not counted as the game's:

`sub_8209BE88` -> `sub_822B8F10`, `sub_8210ADD0` -> `sub_822B8F10`, `sub_8210AE60` -> `sub_824F65C0`, `sub_824600F0` -> `sub_824877D0`, `sub_824658C8` -> `sub_824877D0`, `sub_82465938` -> `sub_824877D0`, `sub_8259C6D0` -> `sub_824F65C0`, `sub_825A1438` -> `sub_822B8F10`, `sub_825D82E8` -> `sub_824892F0`, `sub_825D8660` -> `sub_824892F0`

## Replaced-library functions with direct callers in the game's code

In the excluded ranges but called directly by unported game code ("checked against its callers first"), so counted as the game's (ours or unported), with what they reach:

`sub_82481B88`, `sub_82481B98`, `sub_82482068`, `sub_82482730`, `sub_82482780`, `sub_82482968`, `sub_82482B18`, `sub_82483130`, `sub_82483208`, `sub_82483238`, `sub_82483388`, `sub_82483460`, `sub_82483700`, `sub_82483770`, `sub_82483DC0`, `sub_82483DD0`, `sub_82483F30`, `sub_824847D0`, `sub_82485350`, `sub_82485460`, `sub_82486710`, `sub_82486810`, `sub_82486F90`, `sub_824875A8`, `sub_824875B8`, `sub_82487660`, `sub_824877E0`, `sub_824878B0`, `sub_824878D0`, `sub_82487B28`, `sub_82487BE8`, `sub_82488F50`, `sub_82489048`, `sub_824892A8`, `sub_824892D0`, `sub_824892F8`, `sub_82489300`, `sub_824893B8`, `sub_8248CA80`, `sub_8248CB58`, `sub_8248CBD0`, `sub_8248EC58`, `sub_8248ED78`, `sub_8248F030`, `sub_8248F218`, `sub_8248F550`, `sub_82490960`, `sub_82490B08`, `sub_824936E8`, `sub_82493700`, `sub_824960B8`, `sub_824960E0`, `sub_82496148`
