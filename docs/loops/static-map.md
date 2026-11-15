# B7's static map

Made by `itc_recomp/tools/static_map.py` on every run; never edit it by hand (run the tool again). What it reads, how edges are found and its gaps: the tool's docstring and "The static map" in `docs/coverage.md`.

Made 2026-11-15 at commit `69c08d38` (`lift-sweep`).

## Totals

- Recompiled `sub_` functions: 16552; 144 jump-table cases folded into their parents, leaving 16408.
- Reachable: 9081 (7362 from `main`, 1719 more from 190 extra roots: functions seen in play that `main` doesn't reach, 2 of them not under another).
- Ours: 9068. Excluded (replaced libraries): 11.
- **Unported: 2** (1 under `main`): seen in play 1, never seen 1; 805 generated lines.
- Of the unported, 1 hang under unported code (or a root); 1 are reached only through ours (0 seen in play), 1 of them called by our C++ itself (a guest call by name or address, a fallback for a branch, a table or vtable it stores) and the rest under those; the TSV's `behind_ours` column lists them.
- Ours functions' edges come from our C++ (`src_edges.py`): 37222 definitions in 1201 files of `src/` (harnesses left out), 9689 functions with C++ (an override or `ITC_NATIVE_RUNNING`), 9060 of them reachable. Ours functions without C++ (Merged, or whose print names no address) keep the original's edges where something still reaches them.
- Ours functions whose original body is still reachable: 0 (0 from our C++: an `__imp__` fallback when the native C++ declines, or a guest call to a function with neither override nor C++); listed below.
- Host code no native function's C++ reaches but that names guest functions (constants defined but not resolved to a user, a few hooks' helpers): 3045 functions named, taken as roots; they add 1478 reachable functions.
- Before 2026-11-14's fix the graph followed the original's edges through ours too: 15901 reachable. **Originals no longer called: 1696** (980 not ours, 716 ours whose work our C++ took in; 140298 lines, 300 seen in play): reachable through the original's graph (outside the replaced libraries), not through ours; below, by the native function that left the call out.
- Frontier (ready now): 2 functions.
- Reachable functions calling imports: 295 (1 unported).
- Image read: 13492 code-built data addresses scanned; 181 function pointers in data that no code-built address reaches (136 functions; not edges).

A `*` after a function: never seen in play. Sizes are generated lines.

## Blockers, by what they unblock

Unported functions ranked by how many unported functions above them (through unported callers) wait on them; "frees" is how many would join the frontier if this one alone became ours.

| Function | Unblocks | Frees | Lines | Seen | Ready | File |
|---|---|---|---|---|---|---|
| `sub_821743A8` | 0 | 0 | 657 | no | yes | race-world.md |
| `xstart` | 0 | 0 | 148 | yes | yes | unmapped |

## Frontier clusters, by what they unblock

2 clusters (frontier functions sharing an unported caller within 0x4000 bytes, neighbors in address order, or a call cycle). Each: what it unblocks, its functions with their sizes, its function files.

1. **sub_821743A8**-821743A8: unblocks 0; 1 functions, 657 lines, 0 seen; race-world.md. `sub_821743A8`* 657
2. **xstart**-rt: unblocks 0; 1 functions, 148 lines, 1 seen; unmapped. `xstart` 148

## Unmapped areas

Clusters of unported reachable functions with no rows in any function file: 1 (1 functions, 148 lines). Largest first; the callers named are outside the cluster.

- **xstart**-rt: 1 functions, 148 lines, 1 seen, unblocks 0; called from nothing outside it.

## Extra roots

Seen in play but not reachable from `main` (static edges miss them, or they run outside `main`: start-up, threads, callbacks), not under another extra root:

`xstart` (unported), `sub_822ADB50` (ours)

## Originals no longer called

Reachable through the original's graph but not through ours: the native function's C++ left the call out (plugged-in data, a dropped call, work merged into the C++). Grouped by that native function: its original's callees no longer reached, then how many functions hang only under them (a function under several groups counts in each; "not ours" leaves out those ours already, mostly Merged). Candidates for Dropped or "Not called" rows, for the user's review; the TSV's `lost` column lists them all.

| Native caller | File | Status | Lost callees | Under them | Not ours | Lines | Seen |
|---|---|---|---|---|---|---|---|
| `sub_821077A8` | game-modes.md | Overridden | `sub_82106F88`, `sub_82107670` | 834 | 476 | 77737 | 131 |
| `sub_8214E268` | system-services.md | Overridden | `sub_82149080`, `sub_8214AC68`, `sub_8214ADC0`, `sub_8214AEA8`, `sub_8214D7A0`, `sub_8214F160`, `sub_8214F290`, `sub_8214F810`, ... 7 more | 100 | 77 | 7400 | 15 |
| `sub_822ACB90` | device-state.md | Native | `sub_822A1280`, `sub_822A12B0`, `sub_822A12F0`, `sub_822A1380`, `sub_822A1410`, `sub_822A14A0`, `sub_822A1530`, `sub_822A15A0`, ... 92 more | 100 | 98 | 766 | 2 |
| `sub_8209EEC8` | mode-tick.md | Overridden | `sub_8209E4F0`, `sub_8209E540`, `sub_8209E610`, `sub_8209E698`, `sub_8209E730`, `sub_8209E790`, `sub_8209E860`, `sub_8209E9A8`, ... 5 more | 98 | 81 | 7239 | 15 |
| `sub_825D0F00` | xact-tracks.md | Native | `__savegprlr_26`, `__restgprlr_26`, `sub_825D0F4C`, `sub_825D1A80`, `sub_825D5F40`, `sub_825D5FE8`, `sub_825D6308`, `sub_825D67E8`, ... 5 more | 76 | 68 | 3571 | 8 |
| `sub_8235AAC0` | texture-compression.md | Native | `sub_82365D88`, `sub_82366458`, `sub_82368170`, `sub_8236D380` | 73 | 54 | 5748 | 18 |
| `sub_8216DD88` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_8221F1E8`, `__restgprlr_28` | 67 | 16 | 4435 | 16 |
| `sub_8222ED98` | location-screen.md | Lifted | `sub_8222A470`, `sub_8222A718`, `sub_8222AD50`, `sub_8222AE00`, `sub_8222B8A8`, `sub_8222BE60`, `sub_8222CFF0`, `sub_8222D108`, ... 10 more | 37 | 0 | 3432 | 36 |
| `sub_821A1380` | car-makers.md | Lifted | `sub_8219C528`, `sub_8219C6D0`, `sub_8219C850`, `sub_8219C8F8`, `sub_8219CA08`, `sub_8219CB18`, `sub_8219CBD8`, `sub_8219FC60`, ... 8 more | 36 | 0 | 2173 | 36 |
| `sub_8221BD20` | race-screens.md | Native | `sub_8221A4A8`, `sub_8221A610`, `sub_8221A680`, `sub_8221A6C8`, `sub_8221A720`, `sub_8221B298`, `sub_8221B860`, `sub_8221B8A8` | 31 | 15 | 1900 | 13 |
| `sub_8220F638` | race-screens.md | Native | `sub_8220C578`, `sub_8220C5D0`, `sub_8220D898`, `sub_8220DB98`, `sub_8220E988`, `sub_8220EBD8` | 29 | 10 | 2320 | 18 |
| `sub_822900E0` | save-interface.md | Native | `__savegprlr_27`, `__restgprlr_27`, `sub_825A9EC0`, `sub_825AA058` | 28 | 15 | 3214 | 9 |
| `sub_825CB470` | xact.md | Overridden | `__savegprlr_26`, `__restgprlr_26`, `sub_825C07C8`, `sub_825C95F8`, `sub_825C9A80`, `sub_825CABC8` | 28 | 25 | 1500 | 3 |
| `sub_821159B0` | career.md | Overridden | `sub_82110CA8`, `sub_82111140`, `sub_821115C8`, `sub_82112260`, `sub_82115550`, `sub_82115708`, `__savegprlr_27`, `__restgprlr_27` | 27 | 19 | 2712 | 8 |
| `sub_825D06B8` | xact-tracks.md | Overridden | `__savegprlr_26`, `__restgprlr_26`, `sub_825C07C8`, `sub_825C95F8`, `sub_825C9788`, `sub_825CB3D8` | 27 | 24 | 1454 | 3 |
| `sub_822979E0` | c-runtime-core.md | Lifted | `sub_825A6E48`, `sub_825B6958`, `sub_825D9590` | 26 | 0 | 2778 | 6 |
| `sub_8216C6D8` | race-hud-machine.md | Overridden | `sub_8221EF60` | 25 | 0 | 1763 | 5 |
| `sub_8216DB70` | race-hud-machine.md | Overridden | `sub_82211458`, `__savegprlr_27`, `__restgprlr_27` | 25 | 10 | 1752 | 8 |
| `sub_825C2910` | xact.md | Overridden | `__savegprlr_24`, `__restgprlr_24`, `sub_825C95F8`, `sub_825C9878` | 25 | 22 | 1389 | 3 |
| `sub_8216CB98` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_82216A40` | 22 | 14 | 1133 | 8 |
| `sub_8216D918` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_82216A40` | 22 | 14 | 1133 | 8 |
| `sub_8259C3A8` | models.md | Native | `sub_8259B748`, `sub_8259BE40`, `__savegprlr_26`, `__restgprlr_26` | 20 | 11 | 1941 | 9 |
| `sub_825A8A50` | c-runtime.md | Overridden | `sub_825AFEC0`, `sub_825AFEF8`, `sub_825B0860`, `sub_825B0968`, `sub_825B0B80` | 18 | 11 | 1020 | 4 |
| `sub_825AAD20` | c-runtime.md | Overridden | `sub_825AB1C8`, `sub_825AFEF8`, `sub_825B08E8`, `sub_825B0A40`, `sub_825B0B80` | 18 | 10 | 1022 | 4 |
| `sub_8216E5B0` | race-hud-machine.md | Overridden | `sub_8216D328`, `sub_8216D7B8` | 17 | 5 | 1152 | 5 |
| `sub_825C3050` | lifted.md | Lifted | `sub_825C31D8` | 17 | 2 | 888 | 16 |
| `sub_825C3110` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27`, `sub_825C31D8` | 17 | 2 | 888 | 16 |
| `sub_8216DFD0` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_8221F930`, `sub_8221FFF0`, `__savegprlr_26`, `__restgprlr_26` | 14 | 6 | 664 | 4 |
| `sub_825B13D0` | c-runtime.md | Native | `sub_825AC108`, `sub_825AD4C0`, `sub_825B1368`, `sub_825B6898`, `sub_825B7EB8`, `sub_825BA4A0` | 13 | 6 | 750 | 2 |
| `sub_82092358` | frame-driver.md | Lifted | `sub_82091A60`, `sub_82091EF8`, `sub_820920D8`, `__savegprlr_24`, `__restgprlr_24` | 11 | 5 | 421 | 7 |
| `sub_820990F0` | input.md | Overridden | `sub_82591918`, `sub_82591C18`, `__savegprlr_27`, `__restgprlr_27` | 10 | 2 | 648 | 3 |
| `sub_8216D6D0` | race-hud-machine.md | Overridden | `sub_8221F7D0`, `sub_822200E0` | 10 | 0 | 804 | 2 |
| `sub_82299D60` | heap.md | Overridden | `sub_82299208`, `sub_8229A5F4`, `__savegprlr_22`, `__restgprlr_22` | 10 | 4 | 781 | 10 |
| `sub_8229A930` | heap.md | Overridden | `sub_82298A88`, `sub_8229B16C`, `__savegprlr_19`, `__restgprlr_19` | 10 | 4 | 1031 | 10 |
| `sub_822A08A0` | functions.md | Overridden | `sub_822A05A8`, `sub_822B1F68`, `sub_822B20D8`, `sub_822B2388`, `sub_822B3FE0`, `sub_822B5C58`, `__savegprlr_25`, `__restgprlr_25` | 10 | 2 | 793 | 3 |
| `sub_82593088` | std-string.md | Overridden | `sub_82592E18`, `__savegprlr_26`, `__restgprlr_26` | 10 | 3 | 701 | 6 |
| `sub_825AF130` | c-runtime.md | Native | `sub_825AB010`, `sub_825AC108`, `sub_825AD4C0`, `sub_825AF000`, `sub_825AF098`, `sub_825B6890`, `sub_825B6898` | 10 | 3 | 332 | 3 |
| `sub_8209D130` | mode-tick.md | Overridden | `sub_8217C348`, `__savegprlr_21`, `__restgprlr_21`, `__savevmx_124`, `__restvmx_124` | 8 | 1 | 389 | 7 |
| `sub_8211E500` | ui-movies-game.md | Overridden | `sub_8211A6C0`, `sub_8259D5C8`, `sub_825A0CD0`, `sub_825A0CF8`, `sub_825A0EB0`, `sub_825A0EF0`, `__savegprlr_25`, `__restgprlr_25` | 8 | 0 | 270 | 2 |
| `sub_8216C210` | race-hud-machine.md | Overridden | `sub_82216850` | 8 | 6 | 393 | 2 |
| `sub_8216C530` | race-hud-machine.md | Overridden | `sub_82216850` | 8 | 6 | 393 | 2 |
| `sub_822968B8` | functions.md | Overridden | `sub_82297678` | 8 | 0 | 648 | 2 |
| `sub_82579DA0` | lifted.md | Lifted | `sub_82579D18`, `sub_82579E18`, `sub_8257A498` | 8 | 4 | 498 | 4 |
| `sub_8209CC38` | sprites.md | Overridden | `sub_8209C6E0`, `__savegprlr_25`, `__restgprlr_25` | 7 | 0 | 551 | 6 |
| `sub_8211CD90` | ui-movies-game.md | Overridden | `sub_8211B720`, `sub_8211B770`, `__savegprlr_20`, `__restgprlr_20`, `__savefpr_26`, `__restfpr_26` | 7 | 0 | 142 | 4 |
| `sub_8216D450` | race-hud-machine.md | Overridden | `sub_822157E8`, `sub_82215870`, `sub_822158E8`, `__savegprlr_24`, `__restgprlr_24` | 7 | 3 | 195 | 4 |
| `sub_8229A648` | heap.md | Overridden | `sub_82298548`, `sub_82299448`, `sub_8229A8DC`, `__savegprlr_25`, `__restgprlr_25` | 7 | 3 | 643 | 7 |
| `sub_822B0AB0` | functions.md | Overridden | `sub_822B0760`, `sub_822B07B0`, `sub_822B0860`, `__savegprlr_27`, `__restgprlr_27`, `__savefpr_28`, `__restfpr_28` | 7 | 0 | 205 | 4 |
| `sub_8259F348` | ui-movies.md | Overridden | `sub_8211A948`, `sub_8259D618`, `sub_8259D628`, `__savegprlr_24`, `__restgprlr_24`, `__savefpr_28`, `__restfpr_28` | 7 | 0 | 128 | 4 |
| `sub_82109EF0` | strings-and-font.md | Overridden | `sub_82109D80`, `__savegprlr_19`, `__restgprlr_19` | 6 | 2 | 388 | 4 |
| `sub_8211BEE8` | ui-movies-game.md | Overridden | `sub_8211A948`, `sub_8259D628`, `__savegprlr_27`, `__restgprlr_27`, `__savefpr_27`, `__restfpr_27` | 6 | 0 | 117 | 4 |
| `sub_822B0910` | functions.md | Overridden | `sub_822B07B0`, `sub_822B0860`, `__savegprlr_27`, `__restgprlr_27`, `__savefpr_28`, `__restfpr_28` | 6 | 0 | 181 | 4 |
| `sub_82091720` | std-string.md | Overridden | `sub_82091398`, `__savegprlr_27`, `__restgprlr_27`, `sub_825A7270` | 5 | 1 | 173 | 3 |
| `sub_82094F30` | lifted.md | Lifted | `sub_82095AF0`, `__savegprlr_25`, `__restgprlr_25` | 5 | 1 | 345 | 5 |
| `sub_82095DF8` | lifted.md | Lifted | `sub_82095AF0`, `__savegprlr_24`, `__restgprlr_24` | 5 | 1 | 347 | 5 |
| `sub_820A6F60` | std-string.md | Overridden | `sub_820A6E60`, `sub_825A6A20`, `__savegprlr_27`, `__restgprlr_27`, `sub_825A7270` | 5 | 2 | 213 | 2 |
| `sub_820CDC18` | race-setup.md | Native | `sub_820CC450`, `sub_820CC458`, `sub_820CC460`, `sub_820CC468`, `sub_820CC478` | 5 | 1 | 37 | 4 |
| `sub_8212ACF0` | sprites.md | Lifted | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_27`, `__restfpr_27`, `sub_825A9934` | 5 | 0 | 60 | 4 |
| `sub_8212BB20` | sprites.md | Lifted | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_21`, `__restfpr_21`, `sub_825A9934` | 5 | 0 | 72 | 4 |
| `sub_8212C6D0` | sprites.md | Lifted | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_28`, `__restfpr_28`, `sub_825A9934` | 5 | 0 | 60 | 4 |
| `sub_8212DFB8` | sprites.md | Lifted | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_22`, `__restfpr_22`, `sub_825A9934` | 5 | 0 | 76 | 4 |
| `sub_82131DD8` | screen-objects.md | Native | `sub_8212F030`, `sub_821318B8`, `__restgprlr_28` | 5 | 2 | 393 | 3 |
| `sub_82132BD8` | screen-objects.md | Native | `sub_8212EFA8`, `sub_82132318`, `__restgprlr_28` | 5 | 2 | 396 | 3 |
| `sub_82229238` | location-screen.md | Overridden | `sub_8211AB28`, `sub_8211BE98`, `__savegprlr_21`, `__restgprlr_21` | 5 | 0 | 102 | 2 |
| `sub_825963F0` | models.md | Native | `__savegprlr_26`, `__restgprlr_26`, `sub_825ABFE8` | 5 | 2 | 130 | 2 |
| `sub_825A6F60` | c-runtime.md | Native | `__savegprlr_23`, `__restgprlr_23`, `sub_825A7044`, `sub_825AA1A8` | 5 | 0 | 56 | 5 |
| `sub_825BD8F0` | functions.md | Overridden | `sub_822ADEF0`, `sub_822AE070`, `sub_822AE1D0`, `__savegprlr_22`, `__restgprlr_22` | 5 | 0 | 417 | 5 |
| `sub_825BE300` | functions.md | Overridden | `sub_822ADEF0`, `sub_822AE070`, `sub_822AE1D0`, `__savegprlr_18`, `__restgprlr_18` | 5 | 0 | 425 | 5 |
| `sub_820921A8` | frame-driver.md | Lifted | `sub_82091EF8`, `__restgprlr_28` | 4 | 0 | 124 | 3 |
| `sub_820A4610` | system-services.md | Overridden | `sub_820A45C0`, `__savegprlr_27`, `__restgprlr_27` | 4 | 0 | 54 | 2 |
| `sub_820A4C58` | files-and-saves.md | Overridden | `sub_82295BC8`, `sub_82295C10`, `__savegprlr_27`, `__restgprlr_27` | 4 | 0 | 39 | 2 |
| `sub_820AB6A0` | camera-modes.md | Native | `__savegprlr_19`, `__restgprlr_19`, `__savefpr_14`, `__restfpr_14` | 4 | 0 | 83 | 4 |
| `sub_820AC248` | camera-modes.md | Native | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 47 | 4 |
| `sub_820AF1C0` | car-physics.md | Native | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 49 | 4 |
| `sub_820B1B00` | car-physics.md | Native | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 45 | 4 |
| `sub_820B1FB8` | car-physics.md | Native | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 49 | 4 |
| `sub_820B2CC0` | car-physics.md | Native | `__savegprlr_21`, `__restgprlr_21`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 53 | 4 |
| `sub_820B3F80` | car-physics.md | Native | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_24`, `__restfpr_24` | 4 | 0 | 47 | 4 |
| `sub_820B81C8` | car-physics.md | Native | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_22`, `__restfpr_22` | 4 | 0 | 55 | 4 |
| `sub_820B9FF8` | lifted.md | Lifted | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 49 | 4 |
| `sub_820BB868` | collision.md | Native | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 45 | 4 |
| `sub_820BD508` | collision.md | Native | `sub_820BACB8`, `sub_820BCF28` | 4 | 1 | 183 | 2 |
| `sub_820BD6E0` | collision.md | Native | `sub_820BACB8`, `sub_820BCF28` | 4 | 1 | 183 | 2 |
| `sub_820BDA48` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_24`, `__restfpr_24` | 4 | 0 | 49 | 4 |
| `sub_820BDE40` | lifted.md | Lifted | `__savegprlr_21`, `__restgprlr_21`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 55 | 4 |
| `sub_820BE4F0` | lifted.md | Lifted | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 55 | 4 |
| `sub_820BE8A8` | lifted.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 53 | 4 |
| `sub_820BF320` | collision.md | Native | `__savegprlr_20`, `__restgprlr_20`, `__savefpr_24`, `__restfpr_24` | 4 | 0 | 61 | 4 |
| `sub_820BF690` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 45 | 4 |
| `sub_820C0D28` | car-physics.md | Native | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_14`, `__restfpr_14` | 4 | 0 | 75 | 4 |
| `sub_820C1E00` | vehicle.md | Native | `__savegprlr_20`, `__restgprlr_20`, `__savefpr_24`, `__restfpr_24` | 4 | 0 | 61 | 4 |
| `sub_820C3608` | vehicle.md | Overridden | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 45 | 4 |
| `sub_820D1EB8` | material-passes.md | Overridden | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 43 | 4 |
| `sub_820D9F50` | second-set.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savevmx_123`, `__restvmx_123` | 4 | 0 | 63 | 4 |
| `sub_820DF8B0` | car-draw.md | Native | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 49 | 4 |
| `sub_820E5B08` | car-draw.md | Native | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 49 | 4 |
| `sub_820E8AE8` | car-draw.md | Native | `__savegprlr_21`, `__restgprlr_21`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 51 | 4 |
| `sub_820EA950` | car-draw.md | Native | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 47 | 4 |
| `sub_820EDE00` | car-models.md | Native | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 51 | 4 |
| `sub_820EE0C8` | car-models.md | Native | `__savegprlr_19`, `__restgprlr_19`, `__savefpr_14`, `__restfpr_14` | 4 | 0 | 83 | 4 |
| `sub_820EE638` | car-models.md | Overridden | `__savegprlr_21`, `__restgprlr_21`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 51 | 4 |
| `sub_820F3098` | course-scenery.md | Native | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 55 | 4 |
| `sub_820F32D0` | course-scenery.md | Native | `__savegprlr_17`, `__restgprlr_17`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 61 | 4 |
| `sub_820F6068` | course-scenery.md | Overridden | `__savegprlr_15`, `__restgprlr_15`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 63 | 4 |
| `sub_820FC3C8` | collision-world.md | Native | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 49 | 4 |
| `sub_820FC900` | collision-world.md | Native | `__savegprlr_15`, `__restgprlr_15`, `__savefpr_15`, `__restfpr_15` | 4 | 0 | 89 | 4 |
| `sub_8210D478` | car-sounds.md | Native | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_23`, `__restfpr_23` | 4 | 0 | 59 | 4 |
| `sub_8210F118` | car-sounds.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 41 | 4 |
| `sub_82120008` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_15`, `__restfpr_15` | 4 | 0 | 69 | 4 |
| `sub_82120BB8` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_14`, `__restfpr_14` | 4 | 0 | 69 | 4 |
| `sub_82121D78` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_15`, `__restfpr_15` | 4 | 0 | 69 | 4 |
| `sub_82123A80` | race-overlays.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 43 | 4 |
| `sub_82123E48` | race-overlays.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_19`, `__restfpr_19` | 4 | 0 | 61 | 4 |
| `sub_82124728` | race-overlays.md | Lifted | `__savegprlr_20`, `__restgprlr_20`, `__savefpr_22`, `__restfpr_22` | 4 | 0 | 65 | 4 |
| `sub_82124EE0` | race-overlays.md | Overridden | `__savegprlr_20`, `__restgprlr_20`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 53 | 4 |
| `sub_82125B60` | race-overlays.md | Overridden | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_21`, `__restfpr_21` | 4 | 0 | 61 | 4 |
| `sub_8212A350` | race-overlays.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_22`, `__restfpr_22` | 4 | 0 | 53 | 4 |
| `sub_8212B4A8` | race-overlays.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 47 | 4 |
| `sub_8212D818` | race-overlays.md | Overridden | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 45 | 4 |
| `sub_8212F148` | screen-objects.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_14`, `__restfpr_14` | 4 | 0 | 71 | 4 |
| `sub_82130618` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_18`, `__restfpr_18` | 4 | 0 | 65 | 4 |
| `sub_82130978` | screen-objects.md | Overridden | `__savegprlr_15`, `__restgprlr_15`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 63 | 4 |
| `sub_821329B8` | screen-objects.md | Overridden | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 45 | 4 |
| `sub_82134060` | post-effects.md | Native | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_23`, `__restfpr_23` | 4 | 0 | 55 | 4 |
| `sub_821351F0` | post-effects.md | Overridden | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_82136FA8` | post-effects.md | Native | `__savegprlr_17`, `__restgprlr_17`, `__savefpr_23`, `__restfpr_23` | 4 | 0 | 69 | 4 |
| `sub_82145CA8` | ai-drivers.md | Native | `__savegprlr_20`, `__restgprlr_20`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 53 | 4 |
| `sub_82147D88` | scenery-lists.md | Native | `sub_82146BD0`, `__savegprlr_18`, `__restgprlr_18` | 4 | 1 | 1091 | 3 |
| `sub_82148058` | scenery-lists.md | Overridden | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 53 | 4 |
| `sub_82148658` | sprites.md | Native | `sub_8209F5A0`, `sub_8209F5C8`, `__savegprlr_24`, `__restgprlr_24` | 4 | 0 | 48 | 2 |
| `sub_82153290` | camera-modes.md | Native | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 39 | 4 |
| `sub_82154108` | camera-modes.md | Native | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 41 | 4 |
| `sub_82154548` | camera-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 43 | 4 |
| `sub_8215C780` | race-update.md | Native | `__savegprlr_15`, `__restgprlr_15`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 65 | 4 |
| `sub_82166658` | race-states.md | Lifted | `__savegprlr_17`, `__restgprlr_17`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 61 | 4 |
| `sub_8216D600` | race-hud-machine.md | Overridden | `sub_8216C798`, `sub_82211390` | 4 | 1 | 152 | 1 |
| `sub_8216DA40` | race-hud-machine.md | Overridden | `sub_8216C798`, `sub_82211390` | 4 | 1 | 152 | 1 |
| `sub_82170920` | race-world.md | Native | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 45 | 4 |
| `sub_82172118` | race-world.md | Native | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 49 | 4 |
| `sub_82173E28` | race-world.md | Native | `__savegprlr_21`, `__restgprlr_21`, `__savefpr_24`, `__restfpr_24` | 4 | 0 | 59 | 4 |
| `sub_8217BA20` | front-end-car-draw.md | Native | `__savegprlr_18`, `__restgprlr_18`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 63 | 4 |
| `sub_8217CAB8` | front-end-car-draw.md | Native | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 47 | 4 |
| `sub_8217D3C8` | type6-type7-modes.md | Native | `__savegprlr_19`, `__restgprlr_19`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 59 | 4 |
| `sub_821AD840` | car-settings.md | Lifted | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 45 | 4 |
| `sub_821D8978` | tuning-screen.md | Native | `sub_8211A630`, `sub_8211BA00`, `__savegprlr_25`, `__restgprlr_25` | 4 | 0 | 132 | 2 |
| `sub_821E3AD0` | tuning-screen.md | Native | `sub_8211A630`, `sub_8211BA00`, `__savegprlr_22`, `__restgprlr_22` | 4 | 0 | 138 | 2 |
| `sub_821F7B90` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 39 | 4 |
| `sub_821F86C0` | tuning-pages.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 43 | 4 |
| `sub_82211AB0` | camera-modes.md | Native | `sub_8211AB90`, `sub_8211BE98`, `__savegprlr_27`, `__restgprlr_27` | 4 | 0 | 56 | 2 |
| `sub_8221A090` | race-screens.md | Native | `sub_822191D0`, `sub_82219A78`, `sub_82219B60`, `sub_82219C30` | 4 | 2 | 249 | 2 |
| `sub_82228588` | location-screen.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_20`, `__restfpr_20` | 4 | 0 | 57 | 4 |
| `sub_82293998` | matrix-math.md | Native | `sub_822936D8` | 4 | 0 | 139 | 2 |
| `sub_8229E308` | command-buffer.md | Overridden | `sub_822AD690`, `sub_822AD8F8` | 4 | 2 | 116 | 2 |
| `sub_8229E848` | command-buffer.md | Overridden | `sub_8229DFF8`, `__savegprlr_23`, `__restgprlr_23` | 4 | 1 | 87 | 3 |
| `sub_8229EC00` | command-buffer.md | Native | `sub_822AD690`, `sub_822AD8F8` | 4 | 2 | 116 | 2 |
| `sub_8236AB00` | texture-compression.md | Lifted | `__savegprlr_19`, `__restgprlr_19`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 59 | 4 |
| `sub_8236B418` | texture-compression.md | Lifted | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_19`, `__restfpr_19` | 4 | 0 | 67 | 4 |
| `sub_823EB4A0` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_823EC460` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_823EC8C8` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_823ED328` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 51 | 4 |
| `sub_823ED7A8` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_823EE2D0` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 51 | 4 |
| `sub_823EE750` | texture-compression.md | Lifted | `__savegprlr_22`, `__restgprlr_22`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 49 | 4 |
| `sub_823EFC18` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_823F0370` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 51 | 4 |
| `sub_823F07E8` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_82420AB8` | texture-compression.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_15`, `__restfpr_15` | 4 | 0 | 67 | 4 |
| `sub_82422178` | texture-compression.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_23`, `__restfpr_23` | 4 | 0 | 51 | 4 |
| `sub_82466440` | xaudio-objects.md | Overridden | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 45 | 4 |
| `sub_82468848` | xaudio-objects.md | Native | `__savegprlr_20`, `__restgprlr_20`, `__savefpr_17`, `__restfpr_17` | 4 | 0 | 75 | 4 |
| `sub_82481390` | car-sounds.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_23`, `__restfpr_23` | 4 | 0 | 51 | 4 |
| `sub_82481880` | car-sounds.md | Native | `sub_82480780`, `sub_82481708`, `__savegprlr_27`, `__restgprlr_27` | 4 | 2 | 217 | 2 |
| `sub_82582498` | lifted.md | Lifted | `__savegprlr_21`, `__restgprlr_21`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 51 | 4 |
| `sub_82582AD8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 43 | 4 |
| `sub_82583180` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 43 | 4 |
| `sub_82583DC8` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 49 | 4 |
| `sub_825855B0` | lifted.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_82585948` | lifted.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_82586C10` | lifted.md | Lifted | `__savegprlr_23`, `__restgprlr_23`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 47 | 4 |
| `sub_82587238` | lifted.md | Lifted | `__savegprlr_19`, `__restgprlr_19`, `__savefpr_25`, `__restfpr_25` | 4 | 0 | 61 | 4 |
| `sub_82587960` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 41 | 4 |
| `sub_82587BC8` | lifted.md | Lifted | `__savegprlr_15`, `__restgprlr_15`, `__savefpr_26`, `__restfpr_26` | 4 | 0 | 67 | 4 |
| `sub_825884F8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 41 | 4 |
| `sub_82588E30` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 39 | 4 |
| `sub_82590EF0` | models.md | Overridden | `sub_82590D18`, `sub_825981C8`, `__savegprlr_25`, `__restgprlr_25` | 4 | 2 | 116 | 2 |
| `sub_82593568` | cameras.md | Overridden | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 39 | 4 |
| `sub_82597878` | car-scene-draw.md | Native | `__savegprlr_26`, `__restgprlr_26`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 43 | 4 |
| `sub_82598768` | car-scene-draw.md | Overridden | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_18`, `__restfpr_18` | 4 | 0 | 59 | 4 |
| `sub_82599C58` | models.md | Overridden | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 45 | 4 |
| `sub_8259ACE0` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 43 | 4 |
| `sub_8259BB98` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_23`, `__restfpr_23` | 4 | 0 | 53 | 4 |
| `sub_8259ED40` | ui-movies.md | Overridden | `sub_8211A948`, `sub_8259D648`, `__savegprlr_26`, `__restgprlr_26` | 4 | 0 | 96 | 2 |
| `sub_825A08E0` | ui-movies.md | Overridden | `__savegprlr_19`, `__restgprlr_19`, `__savefpr_28`, `__restfpr_28` | 4 | 0 | 55 | 4 |
| `sub_825A1A38` | model-animation.md | Overridden | `sub_825A1938`, `sub_825A6608`, `__restgprlr_28` | 4 | 3 | 133 | 1 |
| `sub_825A3E18` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `__savefpr_21`, `__restfpr_21` | 4 | 0 | 57 | 4 |
| `sub_825A4680` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24`, `__savefpr_14`, `__restfpr_14` | 4 | 0 | 73 | 4 |
| `sub_825A5BF0` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27`, `__savefpr_27`, `__restfpr_27` | 4 | 0 | 41 | 4 |
| `sub_825A6880` | heap.md | Overridden | `sub_820911E8`, `sub_825A67A8`, `sub_825AD150`, `sub_825DB3F8` | 4 | 4 | 99 | 0 |
| `sub_825A8ED0` | c-runtime.md | Overridden | `__savefpr_25`, `__restfpr_25`, `sub_825A8D50`, `sub_825AB1C8` | 4 | 1 | 168 | 2 |
| `sub_825C27C0` | xact.md | Overridden | `__savegprlr_27`, `__restgprlr_27`, `sub_825C35C0` | 4 | 2 | 234 | 2 |
| `sub_82090A08` | framework.md | Lifted | `sub_820902C8`, `__savegprlr_25`, `__restgprlr_25` | 3 | 0 | 71 | 3 |
| `sub_82091458` | std-string.md | Native | `__savegprlr_27`, `__restgprlr_27`, `sub_825A7270` | 3 | 0 | 98 | 2 |
| `sub_82093BE0` | app-states.md | Overridden | `sub_82091F58` | 3 | 3 | 121 | 0 |
| `sub_82098A98` | frame-driver.md | Overridden | `sub_8258C960`, `__savegprlr_27`, `__restgprlr_27` | 3 | 0 | 37 | 2 |
| `sub_8209A0E8` | framework.md | Native | `sub_82091CD0`, `__savegprlr_23`, `__restgprlr_23` | 3 | 1 | 54 | 2 |
| `sub_8209A458` | framework.md | Overridden | `sub_82091F58` | 3 | 3 | 121 | 0 |
| `sub_8209E118` | files-and-saves.md | Overridden | `sub_8209DD00`, `sub_822954C8`, `__savegprlr_27` | 3 | 1 | 73 | 1 |
| `sub_820A70F8` | lifted.md | Lifted | `sub_820A7218` | 3 | 1 | 376 | 2 |
| `sub_820A7180` | lifted.md | Lifted | `sub_820A7218` | 3 | 1 | 376 | 2 |
| `sub_820A7688` | lifted.md | Lifted | `sub_820A7720` | 3 | 1 | 380 | 2 |
| `sub_820A7AF8` | lifted.md | Lifted | `sub_820A7C50` | 3 | 1 | 397 | 2 |
| `sub_820A7B78` | lifted.md | Lifted | `sub_820A7C50` | 3 | 1 | 397 | 2 |
| `sub_820B2808` | car-physics.md | Native | `sub_820B6938`, `sub_820B6940`, `__restgprlr_28` | 3 | 2 | 21 | 1 |
| `sub_820B7658` | car-physics.md | Overridden | `__restgprlr_28`, `__savefpr_28`, `__restfpr_28` | 3 | 0 | 28 | 3 |
| `sub_820BE150` | collision.md | Native | `sub_820BC8A8`, `__savegprlr_23`, `__restgprlr_23` | 3 | 1 | 68 | 2 |
| `sub_820C8528` | scene.md | Native | `__restgprlr_28`, `__savefpr_26`, `__restfpr_26` | 3 | 0 | 32 | 3 |
| `sub_820CE800` | scene-loaders.md | Overridden | `__savegprlr_27`, `__restgprlr_27`, `sub_825A8600` | 3 | 0 | 29 | 2 |
| `sub_820D3E98` | scene-loaders.md | Native | `__restgprlr_28`, `__savevmx_123`, `__restvmx_123` | 3 | 0 | 50 | 3 |
| `sub_820EF168` | car-models.md | Native | `sub_820EF0B8` | 3 | 3 | 174 | 0 |
| `sub_820F2D28` | minimap.md | Native | `sub_820F2CA8`, `__savegprlr_27`, `__restgprlr_27` | 3 | 1 | 63 | 2 |
| `sub_820F5740` | minimap.md | Overridden | `__savefpr_28`, `__restfpr_28`, `sub_825A9934` | 3 | 0 | 37 | 2 |
| `sub_82109560` | strings-and-font.md | Native | `sub_82108F68`, `__savegprlr_27`, `__restgprlr_27` | 3 | 1 | 74 | 2 |
| `sub_82109AA8` | strings-and-font.md | Native | `sub_821097F0`, `__savegprlr_23`, `__restgprlr_23` | 3 | 1 | 235 | 2 |
| `sub_8210A990` | strings-and-font.md | Overridden | `__savegprlr_23`, `__restgprlr_23`, `sub_825A7EF8` | 3 | 0 | 49 | 2 |
| `sub_8210ABC0` | strings-and-font.md | Overridden | `__savegprlr_24`, `__restgprlr_24`, `sub_825A7EF8` | 3 | 0 | 47 | 2 |
| `sub_8210E470` | car-sounds.md | Native | `__restgprlr_28`, `__savefpr_28`, `__restfpr_28` | 3 | 0 | 28 | 3 |
| `sub_82114128` | save-data.md | Lifted | `__savegprlr_22`, `__restgprlr_22`, `sub_825A9934` | 3 | 0 | 50 | 2 |
| `sub_82114268` | career.md | Overridden | `__savegprlr_19`, `__restgprlr_19`, `sub_825A9934` | 3 | 0 | 56 | 2 |
| `sub_82119B80` | ui-movies.md | Overridden | `sub_8259D628`, `__savegprlr_18`, `__restgprlr_18` | 3 | 0 | 49 | 2 |
| `sub_8211C680` | build-screen.md | Overridden | `sub_8211B720`, `sub_8211B770` | 3 | 0 | 85 | 0 |
| `sub_8211D978` | ui-movies-game.md | Overridden | `sub_8259D670`, `__savegprlr_24`, `__restgprlr_24` | 3 | 0 | 36 | 2 |
| `sub_8211DBE0` | ui-movies-game.md | Overridden | `sub_8211B298`, `__savegprlr_26`, `__restgprlr_26` | 3 | 0 | 63 | 2 |
| `sub_8211DCE8` | ui-movies-game.md | Native | `sub_8211B298`, `__savegprlr_27`, `__restgprlr_27` | 3 | 0 | 61 | 2 |
| `sub_8211E1C0` | ui-movies-game.md | Overridden | `sub_8259D930`, `__savegprlr_27`, `__restgprlr_27` | 3 | 0 | 102 | 2 |
| `sub_8211E378` | ui-movies-game.md | Overridden | `sub_8259D670`, `__savegprlr_25`, `__restgprlr_25` | 3 | 0 | 34 | 2 |
| `sub_8211EC18` | sprites.md | Lifted | `__restgprlr_28`, `__savefpr_27`, `__restfpr_27` | 3 | 0 | 30 | 3 |
| `sub_8211F530` | lifted.md | Lifted | `__restgprlr_28`, `__savefpr_26`, `__restfpr_26` | 3 | 0 | 32 | 3 |
| `sub_8211F6F8` | lifted.md | Lifted | `__restgprlr_28`, `__savefpr_26`, `__restfpr_26` | 3 | 0 | 32 | 3 |
| `sub_8212CE40` | sprites.md | Lifted | `__restgprlr_28`, `__savefpr_26`, `__restfpr_26` | 3 | 0 | 32 | 3 |
| `sub_8214E190` | system-services.md | Overridden | `sub_82148958`, `sub_82459D40`, `sub_82459D50` | 3 | 0 | 55 | 0 |
| `sub_82153640` | camera-modes.md | Native | `__restgprlr_28`, `__savefpr_27`, `__restfpr_27` | 3 | 0 | 30 | 3 |
| `sub_82167610` | race-states.md | Overridden | `sub_821653E8`, `__savegprlr_24`, `__restgprlr_24` | 3 | 1 | 112 | 2 |
| `sub_8218C678` | race-menu.md | Overridden | `sub_8211BA00`, `__savegprlr_23`, `__restgprlr_23` | 3 | 0 | 121 | 2 |
| `sub_821E6C50` | tuning-screen.md | Overridden | `sub_821E6F90`, `__savegprlr_26`, `__restgprlr_26` | 3 | 0 | 101 | 3 |
| `sub_821E9488` | tuning-pages.md | Native | `sub_8259D628`, `__savegprlr_26`, `__restgprlr_26` | 3 | 0 | 33 | 2 |
| `sub_821EA130` | tuning-pages.md | Native | `sub_8259D628`, `__savegprlr_26`, `__restgprlr_26` | 3 | 0 | 33 | 2 |
| `sub_821EAE68` | tuning-pages.md | Native | `sub_8259D628`, `__savegprlr_27`, `__restgprlr_27` | 3 | 0 | 31 | 2 |
| `sub_82218C10` | race-screens.md | Overridden | `sub_82218838`, `__savegprlr_27`, `__restgprlr_27` | 3 | 0 | 93 | 3 |
| `sub_82295890` | c-runtime.md | Overridden | `sub_82296FE0`, `__savegprlr_27`, `__restgprlr_27` | 3 | 0 | 95 | 3 |
| `sub_8229F680` | device-state.md | Native | `sub_822A0F80`, `__savegprlr_19`, `__restgprlr_19` | 3 | 0 | 304 | 2 |
| `sub_822A6210` | graphics-resources.md | Overridden | `sub_822AD9D8` | 3 | 3 | 86 | 0 |
| `sub_822A63F8` | graphics-resources.md | Native | `sub_8229DDD8`, `__savegprlr_22`, `__restgprlr_22` | 3 | 0 | 86 | 3 |
| `sub_822AC918` | functions.md | Lifted | `sub_822AC5A0`, `__savegprlr_25`, `__restgprlr_25` | 3 | 1 | 32 | 2 |
| `sub_822B6BF8` | predicated-tiling.md | Overridden | `sub_8229E5D0`, `__savegprlr_26`, `__restgprlr_26` | 3 | 0 | 142 | 3 |
| `sub_823F56A0` | texture-compression.md | Lifted | `__restgprlr_28`, `__savefpr_28`, `__restfpr_28` | 3 | 0 | 28 | 3 |
| `sub_8245C650` | audio.md | Overridden | `sub_82295728`, `__savegprlr_23`, `__restgprlr_23` | 3 | 1 | 156 | 2 |
| `sub_82465258` | xaudio-objects.md | Native | `sub_82467A00`, `__savegprlr_26`, `__restgprlr_26` | 3 | 0 | 35 | 2 |
| `sub_8246AEE0` | audio.md | Lifted | `__savegprlr_24`, `__restgprlr_24`, `sub_825A9934` | 3 | 0 | 46 | 2 |
| `sub_825850E0` | lifted.md | Lifted | `__restgprlr_28`, `__savefpr_28`, `__restfpr_28` | 3 | 0 | 28 | 3 |
| `sub_8258BD28` | second-set.md | Native | `sub_8258B7E0` | 3 | 1 | 180 | 2 |
| `sub_82598288` | car-scene-draw.md | Native | `sub_825981C8`, `__savegprlr_27`, `__restgprlr_27` | 3 | 1 | 83 | 2 |
| `sub_8259F700` | ui-movies.md | Native | `sub_8259EC58` | 3 | 1 | 105 | 2 |
| `sub_8259FC18` | ui-movies.md | Native | `__restgprlr_28`, `__savefpr_28`, `__restfpr_28` | 3 | 0 | 28 | 3 |
| `sub_8259FD40` | lifted.md | Lifted | `__restgprlr_28`, `__savefpr_28`, `__restfpr_28` | 3 | 0 | 28 | 3 |
| `sub_825A82C8` | c-runtime.md | Native | `__savegprlr_23`, `__restgprlr_23`, `sub_825AFD98` | 3 | 1 | 45 | 2 |
| `sub_825A96A0` | c-runtime-core.md | Native | `__savegprlr_24`, `__restgprlr_24`, `sub_825AD4C0` | 3 | 0 | 33 | 2 |
| `sub_825ACD90` | c-runtime-core.md | Native | `__savegprlr_27`, `__restgprlr_27`, `sub_825ACEB4` | 3 | 0 | 37 | 2 |
| `sub_825AD280` | c-runtime-core.md | Native | `__savegprlr_27`, `__restgprlr_27`, `sub_825AD488` | 3 | 0 | 42 | 2 |
| `sub_825AEBC8` | heap.md | Overridden | `__savegprlr_27`, `__restgprlr_27`, `sub_825AD150` | 3 | 1 | 47 | 2 |
| `sub_825B11A8` | c-runtime-core.md | Overridden | `__savegprlr_25`, `__restgprlr_25`, `sub_825B1304` | 3 | 0 | 46 | 2 |
| `sub_825B3430` | c-runtime.md | Overridden | `sub_825B32F0` | 3 | 2 | 117 | 1 |
| `sub_825B4828` | files-and-saves.md | Overridden | `__savegprlr_27`, `__restgprlr_27`, `sub_825B4978` | 3 | 0 | 42 | 2 |
| `sub_825B65D0` | c-runtime-core.md | Overridden | `__savegprlr_24`, `__restgprlr_24`, `sub_825B6730` | 3 | 0 | 48 | 2 |
| `sub_825BDDC0` | functions.md | Overridden | `sub_822ADEF0`, `sub_822AE070`, `sub_822AE1D0` | 3 | 0 | 386 | 3 |
| `sub_825BE708` | functions.md | Overridden | `sub_822ADEF0`, `sub_822AE070`, `sub_822AE1D0` | 3 | 0 | 386 | 3 |
| `sub_825C09A8` | sound-cues.md | Lifted | `sub_82295FC8`, `__savegprlr_26`, `__restgprlr_26` | 3 | 0 | 136 | 2 |
| `sub_825CDEA8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27`, `sub_825CE05C` | 3 | 1 | 197 | 2 |
| `sub_825D3178` | xact-waves.md | Lifted | `__savegprlr_25`, `__restgprlr_25`, `sub_825D31D8` | 3 | 1 | 127 | 2 |
| `sub_82090420` | framework.md | Native | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_820905E0` | lifted.md | Lifted | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_82091540` | std-string.md | Native | `sub_825A6A20`, `__restgprlr_28` | 2 | 1 | 38 | 1 |
| `sub_82091808` | std-string.md | Overridden | `__restgprlr_28`, `sub_825A7270` | 2 | 0 | 87 | 1 |
| `sub_82093000` | frame-driver.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82093400` | framework.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82093740` | framework.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820938C0` | app-states.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82094590` | app-start-up.md | Overridden | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82094BE8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82095370` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820956F0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82095958` | input.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82096030` | frame-passes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820988C8` | frame-driver.md | Overridden | `sub_8258C998`, `sub_8258C9C0` | 2 | 0 | 20 | 0 |
| `sub_820991E0` | input.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82099600` | system-services.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82099E20` | framework.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8209A760` | framework.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8209B070` | app-start-up.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8209C1A0` | sprites.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8209CE68` | mode-tick.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8209D038` | mode-tick.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8209D0C8` | mode-tick.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8209D950` | files-and-saves.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8209DC18` | files-and-saves.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8209DE48` | files-and-saves.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8209E210` | files-and-saves.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8209F228` | action-mappers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8209FA38` | cameras.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820A0490` | save-interface.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820A0510` | save-interface.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820A07F0` | models.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820A0978` | cameras.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820A0CC0` | cameras.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820A1C30` | cameras.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820A49D0` | files-and-saves.md | Overridden | `sub_82295C08`, `__restgprlr_28` | 2 | 0 | 15 | 1 |
| `sub_820A4B18` | files-and-saves.md | Overridden | `sub_82295BC8`, `__restgprlr_28` | 2 | 0 | 23 | 1 |
| `sub_820A52E0` | save-interface.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820A53E8` | save-interface.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820A6160` | save-interface.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820A6D28` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820A8B88` | cameras.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820A93E8` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820AA088` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820AAFF0` | cameras.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820ACAA8` | morph-jobs.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820ACC08` | morph-jobs.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820AEC10` | movies.md | Overridden | `sub_820AE7F8`, `__restgprlr_28` | 2 | 1 | 18 | 2 |
| `sub_820AF018` | car-physics.md | Native | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_820AFBD8` | car-physics.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820B0908` | car-physics.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820B0EE8` | car-physics.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820B13C8` | car-physics.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820B18B8` | car-physics.md | Native | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_820B3D10` | car-physics.md | Overridden | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_820B4420` | car-physics.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820B4778` | car-physics.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_820B4AA8` | car-physics.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820B5588` | car-physics.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820B5670` | car-physics.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820B5B08` | car-physics.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820B5FD8` | car-physics.md | Overridden | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_820B6170` | lifted.md | Lifted | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_820B7ED0` | car-physics.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_820B87B0` | car-physics.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820B8AF8` | car-physics.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820B8D30` | car-physics.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820B9570` | car-physics.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820B9C40` | collision.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820BA408` | collision.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820BA688` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820BA888` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820BAE78` | collision.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820BB300` | collision.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820BB580` | collision.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820BBB20` | collision.md | Overridden | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_820BC560` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820BC978` | collision.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820BD280` | collision.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820BD578` | collision.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820BD740` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820BECA0` | collision.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820BF080` | collision.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820BFD30` | vehicle.md | Native | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_820C0310` | vehicle.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820C05C8` | car-physics.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820C1D08` | vehicle.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820C2248` | vehicle.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820C23E8` | vehicle.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820C27E8` | car-physics.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820C3020` | vehicle.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820C3150` | vehicle.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820C33B8` | vehicle.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820C3DA0` | scene.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820C3FD8` | scene-loaders.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820C4A70` | scene.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820C5798` | scene.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820C5D70` | vehicle-accessors.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820C5DE8` | car-scene-draw.md | Overridden | `__savefpr_18`, `__restfpr_18` | 2 | 0 | 38 | 2 |
| `sub_820C8480` | front-end-car-draw.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820C8DE8` | car-models.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820C9170` | car-draw.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820C9E00` | car-specs.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820CD8A8` | race-setup.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820CDCE0` | race-setup.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820CEB30` | second-set.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820D02D0` | scene-loaders.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820D04E8` | car-draw.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_820D0860` | car-models.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820D0A98` | scene-models.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820D0EA8` | material-passes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820D1378` | material-passes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820D17D8` | material-passes.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820D1950` | material-passes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820D1B38` | material-passes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820D1C00` | material-passes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820D2270` | scene-loaders.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820D2330` | material-passes.md | Overridden | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_820D2EA8` | material-passes.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820D3318` | material-passes.md | Overridden | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_820D4078` | car-models.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820D47A8` | car-models.md | Overridden | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_820D4F80` | car-models.md | Overridden | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_820D5750` | car-models.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820D5B50` | car-models.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820D5D20` | car-models.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820D6108` | scene-loaders.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820D6328` | scene-loaders.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820D72D8` | load-poll.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820D7388` | scene-loaders.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820D7428` | scene-loaders.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820D7840` | scene-loaders.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820D78B8` | scene-loaders.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820D7D38` | scene-loaders.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820D7E18` | load-poll.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820D8430` | scene-loaders.md | Overridden | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820D8AA8` | scene-loaders.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820D91E0` | load-poll.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_820D92A0` | load-poll.md | Overridden | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_820D9638` | load-poll.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820DA758` | scene-models.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820DAC68` | car-draw.md | Overridden | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_820DB458` | scene-models.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820DBC88` | second-set.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820DBD38` | car-draw.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820DC138` | load-poll.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820DCC70` | car-draw.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820DCF98` | car-draw.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820DD110` | car-draw.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820DD438` | car-draw.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820DD620` | car-draw.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820DD948` | car-draw.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820DE1C8` | car-draw.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820DE7E8` | second-set.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820DEDD8` | car-draw.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820DF740` | second-set.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820E0258` | second-set.md | Overridden | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820E1690` | car-draw.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820E1D88` | car-draw.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820E2448` | car-draw.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_820E2760` | car-draw.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820E2C10` | second-set.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820E3200` | second-set.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820E3660` | second-set.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820E39F8` | car-draw.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820E4140` | load-poll.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820E5220` | car-draw.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820E55A0` | car-draw.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820E61D8` | car-draw.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820E6820` | car-draw.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820E6A40` | car-draw.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820E70C0` | car-models.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820E7598` | second-set.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820E79C8` | second-set.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820E7CA0` | second-set.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820E8028` | second-set.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820E8A00` | load-poll.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820E9318` | car-draw.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820E9A28` | car-draw.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820EA148` | car-draw.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820EB388` | car-draw.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820EBEB8` | car-draw.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820ECF08` | car-models.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820ED090` | car-models.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820ED2A0` | car-models.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820ED630` | car-models.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820EF468` | accessories.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820EF4E8` | accessories.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820EF568` | accessories.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820EF790` | accessories.md | Lifted | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_820EF9B8` | accessories.md | Lifted | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_820EFD70` | accessories.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820F0120` | accessories.md | Lifted | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_820F09E0` | accessories.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820F0C90` | accessories.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820F1258` | course-scenery.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820F1598` | course-scenery.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820F16C0` | course-scenery.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820F1E00` | course-scenery.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820F2030` | course-scenery.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_820F2520` | course-scenery.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820F2720` | course-scenery.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_820F2B80` | course-scenery.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820F34C8` | course-scenery.md | Native | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_820F3B70` | course-scenery.md | Native | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_820F4128` | course-scenery.md | Native | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_820F4558` | course-scenery.md | Overridden | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_820F4888` | minimap.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820F4BA8` | minimap.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820F52A8` | course-scenery.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820F6C88` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820F6E18` | collision-world.md | Native | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_820F71B0` | collision-world.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820F7818` | collision-world.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_820F7AA0` | collision-world.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_820F7E30` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820F7EE8` | lifted.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_820F8720` | track-route.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_820F93A0` | track-route.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820F9448` | track-route.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820F9600` | track-route.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820F9768` | track-route.md | Overridden | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_820F9E28` | collision-world.md | Lifted | `sub_820F9050` | 2 | 0 | 176 | 1 |
| `sub_820FA728` | track-route.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820FA9D0` | track-route.md | Overridden | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_820FB100` | track-route.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820FB2B0` | lifted.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820FB680` | track-route.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820FBBE8` | course-scenery.md | Native | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_820FBDC8` | course-scenery.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_820FC188` | collision-world.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820FDC18` | collision-world.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_820FDF98` | track-zones.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820FE120` | track-zones.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_820FE398` | track-zones.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_820FE598` | camera-modes.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_820FE6F8` | track-zones.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_820FEBD0` | collision-world.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820FF050` | collision-world.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82101D50` | race-setup.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82101F68` | race-setup.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82102AA8` | replay-snapshots.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82102C60` | replay-snapshots.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82103300` | replay-snapshots.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821034B8` | replay-snapshots.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82103CB8` | replay-snapshots.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821041C0` | replay-snapshots.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821046F8` | replay-snapshots.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82104898` | replay-snapshots.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82104AB8` | replay-snapshots.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82104D40` | replay-snapshots.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82104F68` | replay-snapshots.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82105228` | type10-mode.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82106158` | game-modes.md | Overridden | `sub_82105FE0`, `sub_82106100` | 2 | 0 | 57 | 2 |
| `sub_821063B0` | mode-tick.md | Overridden | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_821073B8` | game-modes.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82107910` | game-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82108018` | boot-mode.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821088B8` | strings-and-font.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82109498` | strings-and-font.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8210A538` | strings-and-font.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8210A680` | strings-and-font.md | Native | `sub_825A7EF8`, `sub_825A7F30` | 2 | 0 | 32 | 0 |
| `sub_8210A8D0` | files-and-saves.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8210B7D0` | action-mappers.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8210BBA0` | action-mappers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8210C0E0` | action-mappers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8210C180` | action-mappers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8210CE38` | car-sounds.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8210D718` | car-sounds.md | Native | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_8210D890` | car-sounds.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8210E778` | car-sounds.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8210E938` | car-sounds.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_8210EEE0` | car-sounds.md | Native | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_8210F820` | car-sounds.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8210FE00` | car-sounds.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821109B8` | save-interface.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82112950` | career.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82112DB0` | race-setup.md | Overridden | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82115190` | career.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82115D98` | race-setup.md | Overridden | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_821162B0` | career.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82116588` | career.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82116DB0` | data-tables.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82117B88` | sound-cues.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82117D08` | sound-cues.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821180B8` | sound-cues.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82118A90` | sound-cues.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82118B58` | sound-cues.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82119278` | menu-sounds.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82119500` | sound-cues.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821197E0` | sound-cues.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8211B800` | ui-movies-game.md | Native | `sub_8211B310`, `sub_8211B720` | 2 | 0 | 59 | 0 |
| `sub_8211BD10` | ui-movies-game.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8211CBD8` | ui-movies-game.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8211D2F8` | ui-movies.md | Overridden | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8211DFF8` | ui-movies-game.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82120868` | lifted.md | Lifted | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82121280` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82122DB8` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82123450` | race-overlays.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821251C0` | race-overlays.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821252D0` | race-overlays.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821253F0` | race-overlays.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82125798` | race-overlays.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82125970` | race-overlays.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82126220` | race-overlays.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821281A8` | sprites.md | Overridden | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_82128750` | sprites.md | Overridden | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82128E70` | sprites.md | Overridden | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8212B1B0` | race-overlays.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8212C2F0` | race-overlays.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8212EA98` | post-effects.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8212EC90` | post-effects.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8212F538` | lifted.md | Lifted | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_8212F778` | lifted.md | Lifted | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_8212FDB0` | screen-objects.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82130330` | lifted.md | Lifted | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_82130490` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82131680` | screen-objects.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82131CB8` | screen-objects.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82131FB0` | screen-objects.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821321F8` | screen-objects.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82132778` | screen-objects.md | Native | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_82132CE0` | screen-objects.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82133000` | screen-objects.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82133190` | post-effects.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82133340` | post-effects.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82133790` | post-effects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82133960` | post-effects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82133F58` | post-effects.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821356B0` | lifted.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82135B50` | post-effects.md | Overridden | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_82138610` | post-effects.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82138A18` | post-effects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82139628` | ai-drivers.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8213A000` | ai-drivers.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8213A140` | ai-drivers.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8213A620` | ai-drivers.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_8213A8B0` | ai-drivers.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8213ADF8` | ai-drivers.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8213B058` | ai-drivers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8213B1E0` | ai-drivers.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8213B650` | ai-drivers.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8213CB30` | ai-drivers.md | Native | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_8213D020` | ai-drivers.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8213D1F0` | ai-drivers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8213D550` | ai-drivers.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_8213DE30` | ai-drivers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8213E018` | ai-drivers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8213E1D0` | ai-drivers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8213E360` | ai-drivers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8213E690` | ai-drivers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8213E7B8` | ai-drivers.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8213E970` | ai-drivers.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8213EB48` | ai-drivers.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8213ECF8` | ai-drivers.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8213F140` | ai-drivers.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8213F2E0` | ai-drivers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8213FCF8` | ai-drivers.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821400C8` | race-event.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82140340` | race-event.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82140868` | race-event.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82140AC8` | race-event.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821430A0` | race-setup.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82143210` | race-event.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82143740` | race-event.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82143920` | race-world.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82143A40` | race-world.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_82143B78` | race-world.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82143DC8` | race-event.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82143F90` | race-event.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821444D0` | race-event.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82144798` | race-world.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82144A50` | race-event.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82145A48` | ai-drivers.md | Native | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_82145B08` | ai-drivers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82146608` | ai-drivers.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821467F0` | ai-drivers.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82148428` | scenery-lists.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82148528` | scenery-lists.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82148D00` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82149178` | system-services.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8214BF20` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82150100` | camera-modes.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82150350` | camera-modes.md | Native | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_82150590` | camera-modes.md | Native | `__savefpr_26`, `__restfpr_26` | 2 | 0 | 22 | 2 |
| `sub_82150878` | camera-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82150960` | camera-modes.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82150E00` | race-end.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821511B8` | race-loading.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821517D8` | race-loading.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82151D20` | game-modes.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82152078` | camera-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82154A48` | camera-modes.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82154D38` | camera-modes.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82155288` | camera-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82155E58` | camera-modes.md | Overridden | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_821562F8` | camera-modes.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82156980` | camera-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82156B68` | camera-modes.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82158F70` | camera-modes.md | Overridden | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_821591F0` | camera-modes.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821594B8` | camera-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821596B0` | camera-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821597F0` | camera-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82159950` | camera-modes.md | Overridden | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_8215B878` | car-makers.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8215C0D8` | race-update.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8215C248` | race-scene.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8215D808` | race-scene.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8215E2E0` | race-overlays.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8215E6C8` | game-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8215FA68` | racer-controllers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8215FCA8` | racer-controllers.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_821604C8` | racer-controllers.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_821606D0` | racer-controllers.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821607A0` | racer-controllers.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82160880` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82160AE0` | race-flow.md | Overridden | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82162330` | race-states.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82163000` | race-states.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82163218` | race-states.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821634D8` | race-states.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82163C50` | race-states.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82164090` | race-states.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82164228` | race-states.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821644D8` | race-states.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82164A90` | race-states.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82164B50` | race-states.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82164E18` | race-states.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821650A0` | race-states.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82165AB0` | race-states.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82165C78` | race-states.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_821662E0` | race-world.md | Overridden | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_82166B40` | race-states.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82166F98` | race-states.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821672F0` | race-states.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82167BE0` | race-states.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82167D88` | race-states.md | Lifted | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82168018` | race-states.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82168910` | race-states.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82168B80` | race-states.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82168C80` | race-states.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82169030` | race-states.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821692C8` | race-states.md | Overridden | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8216E150` | race-hud-machine.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8216E6E8` | race-hud-machine.md | Overridden | `__savegprlr_15`, `__restgprlr_15` | 2 | 0 | 45 | 2 |
| `sub_821706E8` | race-world.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82170CB8` | race-world.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82171070` | race-world.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821714D8` | race-world.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82171718` | race-world.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82171978` | race-world.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82171E50` | race-world.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821724F8` | race-world.md | Native | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_82172E60` | race-world.md | Native | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_82172F90` | race-world.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821737F8` | race-world.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82173A90` | race-world.md | Overridden | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82174200` | race-world.md | Native | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_82174D28` | race-world.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82176188` | race-sounds.md | Native | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82176C10` | race-sounds.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821772D8` | race-sounds.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82177378` | race-sounds.md | Overridden | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_821797A8` | stand-models.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82179818` | stand-models.md | Native | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_821798D8` | stand-models.md | Native | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_82179AE8` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8217A370` | mode-tick.md | Native | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_8217AA98` | scene-target.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8217B508` | power-graph.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8217B7C8` | build-screen.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_8217C370` | minimap.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8217C4C0` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8217C708` | power-graph.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8217D120` | front-end-car-draw.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8217DBC0` | stand-models.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8217F880` | car-makers.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8217F9E0` | front-end-car-draw.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8217FBE8` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82180B00` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821813B0` | car-dealer.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821822F8` | front-end-screen-helpers.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821826C0` | car-dealer.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821827E0` | front-end-screen-helpers.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_82182990` | car-dealer.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82182B30` | car-dealer.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82182E68` | car-dealer.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82183600` | car-dealer.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82183BD8` | car-dealer.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82183DE0` | car-dealer.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82183F68` | car-dealer.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82184018` | car-dealer.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82184338` | car-dealer.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82184488` | car-dealer.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821847A0` | car-dealer.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82185600` | car-dealer.md | Overridden | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82185920` | car-dealer.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82186568` | menu-helpers.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82187160` | type2-dialog.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82187EC8` | type2-dialog.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82188178` | type2-dialog.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82188A98` | race-menu.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_82188E20` | race-menu.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821891C8` | race-menu.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82189C28` | race-menu.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8218B2C8` | race-menu.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8218B9F0` | race-menu.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8218BC18` | race-menu.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8218CE10` | race-menu.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8218DCC0` | type2-screens.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8218DF38` | type2-screens.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8218EE48` | type2-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8218F530` | type2-screens.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821907F8` | type2-screens.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82190B18` | type2-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82192378` | garage-screen.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_821933D0` | garage-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821934D0` | garage-screen.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_821939A0` | garage-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821941A8` | garage-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82194680` | garage-screen.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821948B0` | garage-screen.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82194B20` | garage-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82194CD8` | garage-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82194F30` | garage-screen.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821951A8` | garage-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821956C0` | garage-screen.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821958D0` | garage-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82196230` | garage-screen.md | Overridden | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82196520` | garage-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82197278` | type2-mode.md | Overridden | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82197D50` | type2-mode.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82198FB8` | type2-screens.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_821990D0` | type2-screens.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82199220` | type2-screens.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82199488` | type2-screens.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_821995D8` | type2-screens.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821998F0` | type2-screens.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82199990` | type2-screens.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8219A450` | type2-screens.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8219A6B8` | type2-screens.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8219AAB0` | type2-screens.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8219AD68` | type2-screens.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8219B108` | type2-screens.md | Overridden | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_8219B2E8` | type2-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8219CE08` | build-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8219D248` | build-screen.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8219D550` | build-screen.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8219D820` | build-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8219DA58` | build-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8219DBC8` | type2-screens.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8219DDB8` | type2-screens.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8219DFA8` | type2-screens.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8219E348` | type2-screens.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8219E600` | type2-screens.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8219E9A0` | type2-screens.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8219F3A0` | build-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8219F520` | build-screen.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8219FB08` | build-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A0128` | build-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A0228` | type2-screens.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_821A0B38` | build-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A0D38` | build-screen.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_821A0E50` | build-screen.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_821A16C0` | type2-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A24B0` | save-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A2FB0` | save-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A3A08` | save-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A3ED8` | save-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A4AC0` | save-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A51C8` | front-end-screen-helpers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A5360` | front-end-screen-helpers.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A5668` | front-end-screen-helpers.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A5980` | front-end-screen-helpers.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821A5D00` | front-end-screen-helpers.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A6030` | front-end-screen-helpers.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A6E18` | save-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A7470` | save-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A7AC8` | save-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821A8210` | type6-type7-modes.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821AB898` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821ACFB0` | car-settings.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821AD200` | car-settings.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821AEC18` | lifted.md | Lifted | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_821AED18` | car-settings.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_821AF0A8` | car-settings.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_821B1678` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821B1960` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821B1B40` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821B1DA0` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821B2100` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821B2728` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B2B30` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B2DD8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B34A0` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B4068` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821B4298` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821B4848` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B49F8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B4BA0` | car-settings.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_821B4FD8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B6FC0` | car-settings.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B7878` | car-settings.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B8688` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B8728` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B90D8` | type2-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821B9E70` | options-screen.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821BA0F0` | options-screen.md | Lifted | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_821BBF00` | options-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821BCFF0` | options-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821BDCB0` | options-screen.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821BE040` | options-screen.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821BE290` | options-screen.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_821BEFE8` | options-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821BF2B8` | options-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821BF520` | options-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821BF7F8` | options-screen.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C03D8` | options-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C0640` | options-screen.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C0BF0` | front-end-screen-helpers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C0D70` | front-end-screen-helpers.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C1068` | options-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C2BE0` | options-screen.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821C2E28` | options-screen.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821C3008` | options-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C3398` | options-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C36F0` | options-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C3A10` | options-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C3E48` | options-screen.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C43D0` | options-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C47A0` | options-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C4BE0` | options-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C4DB0` | options-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C5350` | options-screen.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C5D68` | type6-type7-modes.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C6638` | front-end-screen-helpers.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C69D0` | front-end-screen-helpers.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C7188` | options-screen.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_821C7F98` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821C80F0` | options-screen.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821C8578` | options-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C8A60` | options-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C9430` | type6-type7-modes.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C9918` | type6-type7-modes.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821C9E00` | options-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821CA458` | options-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821CB090` | type6-type7-modes.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821CB6E8` | type6-type7-modes.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821CE1D0` | tuning-pages.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821CE4C8` | tuning-machine.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821D12C0` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821D2A80` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821D5028` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821DA4B0` | tuning-machine.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821DAD58` | tuning-machine.md | Native | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_821DBAB0` | tuning-machine.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821DCC28` | tuning-screen.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821DE2D8` | tuning-screen.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821E0FB0` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821E1B48` | tuning-machine.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821E22E8` | tuning-screen.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_821E7568` | tuning-machine.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821E7E48` | tuning-screen.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_821E8128` | tuning-screen.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_821ECC68` | tuning-screen.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821ED108` | tuning-pages.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821EE390` | tuning-pages.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821EE9B0` | tuning-screen.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821F1130` | tuning-pages.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821F1DE0` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821F2378` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821F2AD8` | tuning-screen.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_821F40B8` | tuning-pages.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821F4768` | tuning-pages.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821F4BF8` | tuning-screen.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821F5B98` | tuning-pages.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821F7518` | sticker-editor.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821F77F0` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821F8160` | sticker-editor.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821F8868` | sticker-editor.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_821F8A50` | sticker-editor.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821F8C30` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821F8D68` | sticker-editor.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821FA508` | sticker-editor.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821FA658` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821FAD00` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821FB0B8` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821FB1F0` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821FB450` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821FBA40` | sticker-editor.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821FBB60` | sticker-editor.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_821FC270` | sticker-editor.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821FC7B8` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821FCA80` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821FCC48` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821FCED0` | tuning-screen.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_821FD3F8` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821FD880` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_821FDA80` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821FDCB8` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_821FEAF0` | tuning-pages.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82203110` | tuning-screen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82203B00` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822040A8` | tuning-pages.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822042D8` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82204B70` | tuning-pages.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822087C8` | tuning-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82208CE0` | tuning-pages.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82209738` | tuning-pages.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8220A1C8` | tuning-pages.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8220B408` | user-info.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8220C1E0` | race-screens.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8220CB38` | race-screens.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8220CC60` | race-screens.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8220DD98` | race-screens.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8220DF30` | race-screens.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8220E1F8` | race-screens.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8220E700` | race-screens.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8220FB10` | race-screens.md | Native | `__savegprlr_15`, `__restgprlr_15` | 2 | 0 | 45 | 2 |
| `sub_82210360` | race-screens-unseen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82210940` | race-screens-unseen.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82210CD0` | race-screens-unseen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82211590` | race-screens-unseen.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82211980` | camera-modes.md | Native | `sub_8211AB90`, `__restgprlr_28` | 2 | 0 | 18 | 1 |
| `sub_82211DA0` | race-hud-object.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822122B0` | race-hud-object.md | Native | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_822123C8` | race-hud-object.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82212F00` | race-hud-object.md | Native | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82213638` | race-hud-object.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82213750` | race-hud-object.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82213860` | race-hud-object.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82213968` | race-hud-object.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82213AE0` | race-hud-object.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82213C90` | race-hud-object.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82213F20` | race-hud-object.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82214260` | race-hud-object.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82214330` | race-hud-object.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_822144B0` | race-hud-object.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822145A0` | race-hud-object.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82214790` | race-hud-object.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82214A50` | race-hud-object.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82214BA8` | race-hud-object.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82214E78` | race-hud-object.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82215160` | race-hud-object.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822155C0` | race-hud-object.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82215B48` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82216C40` | alt-hud.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82216E00` | alt-hud.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82216EE8` | alt-hud.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82216FD0` | alt-hud.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82217168` | alt-hud.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82217270` | alt-hud.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822178A0` | alt-hud.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82217A30` | alt-hud.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8221AC00` | race-screens.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8221ACC0` | race-screens.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8221B3C8` | race-screens.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8221BB48` | race-screens.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82221228` | title-screen.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822213B0` | title-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822221A0` | title-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82222E88` | title-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82223050` | title-screen.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822231B0` | title-screen.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82223428` | title-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822235D0` | title-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82223D70` | title-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822240B0` | title-screen.md | Overridden | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82224270` | title-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82225390` | title-screen.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82225D10` | type5-mode.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82225F30` | type5-mode.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82226088` | type5-mode.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82226788` | type5-mode.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82226A10` | type5-mode.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82227430` | location-screen.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82227A50` | location-screen.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82228220` | location-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82228E88` | location-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82228FC8` | location-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8222A218` | location-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8222B200` | location-screen.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8222B438` | location-screen.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8222C188` | location-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8222C270` | location-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8222C300` | location-screen.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8222DCE8` | location-screen.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8222E368` | location-screen.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8222EC08` | location-screen.md | Overridden | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_8222F0B0` | location-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8222F258` | location-screen.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82230458` | type5-mode.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822309C8` | screen-constructors.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82230C88` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82230F80` | type6-type7-modes.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82232328` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822329A8` | type6-type7-modes.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822330F0` | screen-constructors.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822334C8` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82233C80` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82234D50` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82234F08` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82235138` | type6-type7-modes.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82235C18` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82235F28` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822363B8` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82236880` | type6-type7-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82236F00` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82237320` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82237B78` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8223F9A8` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822403A0` | screen-constructors.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822405F8` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82240998` | type6-type7-modes.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82241EF8` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82242420` | type6-type7-modes.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82242BE8` | screen-constructors.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82243018` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82243308` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82243588` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822436A0` | type6-type7-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822441D8` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82244FC0` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82245200` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822454B8` | type6-type7-modes.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82246098` | type6-type7-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822463D8` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82246800` | type6-type7-modes.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82246C90` | type6-type7-modes.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822470A8` | type6-type7-modes.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82247ED0` | type67-screens.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822480F0` | type67-screens.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82248198` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82248F00` | car-list.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82249098` | car-list.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82249128` | car-list.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822492C8` | car-list.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8224A978` | car-list.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8224B248` | car-list.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8224BF30` | car-list.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8224C008` | car-list.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8224C698` | car-list.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8224D580` | car-list.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8224D860` | car-list.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8224DA20` | car-list.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8224DDF8` | car-list.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8224E878` | car-list.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8224EBF0` | car-list.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8224EC50` | car-list.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8224F018` | car-list.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8224F680` | car-list.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8224F758` | car-list.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_8224F888` | car-list.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8224F8F8` | car-list.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8224FF88` | car-list.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822501C8` | car-list.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82250BA0` | power-graph-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82250C40` | power-graph-screen.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822518B0` | type67-screens.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82252778` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82255490` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82255EB0` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82256030` | type67-screens.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82256AC0` | type67-screens.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82256CA0` | type67-screens.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82256E08` | type67-screens.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822579E8` | type67-screens.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82257B78` | type67-screens.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82259410` | type67-screens.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82259750` | type67-screens.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8225B358` | todays-result.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8225BAB8` | todays-result.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8225BD10` | todays-result.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8225C278` | todays-result.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8225CF30` | todays-result.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8225DC38` | todays-result.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82289768` | type10-mode.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82289F48` | type10-mode.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8228A3B8` | type10-mode.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8228A658` | type10-mode.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8228AC48` | type10-mode.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8228D070` | track-zones.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8228DD00` | track-zones.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8228E6A0` | track-zones.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8228F408` | scenery-lists.md | Overridden | `sub_8228F484`, `__restgprlr_28` | 2 | 0 | 167 | 2 |
| `sub_8228F840` | save-interface.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8228FC50` | files-and-saves.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82290378` | save-interface.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82290788` | achievements.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82290A30` | achievements.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82291858` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82292020` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82292A48` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82292CC0` | network-mode.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82293028` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82293600` | matrix-math.md | Native | `__savefpr_26`, `__restfpr_26` | 2 | 0 | 22 | 2 |
| `sub_82293AD8` | matrix-math.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82294610` | files-and-saves.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822947F8` | files-and-saves.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822950C0` | system-services.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822951A8` | system-services.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82295208` | system-services.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82295508` | files-and-saves.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82296270` | functions.md | Overridden | `sub_822972D0` | 2 | 2 | 12 | 0 |
| `sub_82297100` | mode-tick.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822990A0` | heap.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_822997B0` | system-services.md | Lifted | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_8229DB38` | functions.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8229DD70` | command-buffer.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8229E3F0` | command-buffer.md | Overridden | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_8229E9D0` | command-buffer.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8229EDA8` | command-buffer.md | Native | `sub_822AD850`, `__restgprlr_28` | 2 | 1 | 68 | 1 |
| `sub_8229F280` | functions.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8229FC40` | lifted.md | Lifted | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_822A04F8` | functions.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822A3518` | device-state.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822A3758` | device-state.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822A45B8` | graphics-resources.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822A4A40` | graphics-resources.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822A4B98` | graphics-resources.md | Native | `__savegprlr_15`, `__restgprlr_15` | 2 | 0 | 45 | 2 |
| `sub_822A54B0` | graphics-resources.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822A5608` | graphics-resources.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_822A5910` | graphics-resources.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_822A5D08` | graphics-resources.md | Overridden | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_822A5E28` | graphics-resources.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822A6008` | device-state.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822A6A88` | device-state.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822A8740` | shader-objects.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822A8980` | app-start-up.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822A9EE8` | shader-objects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822AA018` | shader-objects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822AC638` | functions.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_822AC858` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822ACE00` | functions.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_822AD298` | functions.md | Overridden | `sub_822AD190`, `sub_822B2388` | 2 | 0 | 128 | 0 |
| `sub_822ADB50` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822AE340` | functions.md | Overridden | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_822AEAA0` | lifted.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_822AEBF0` | lifted.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_822AED18` | functions.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_822AF978` | command-buffer.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822B43E8` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822B4F10` | functions.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822B5EF8` | graphics-resources.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822B60C8` | predicated-tiling.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_822B6188` | predicated-tiling.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_822B69A8` | predicated-tiling.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_822B6AD8` | predicated-tiling.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_822B6D70` | predicated-tiling.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_8235A720` | graphics-resources.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82368D98` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82368F58` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82369050` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823691D8` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_823692D0` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_823693C8` | texture-compression.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_823694C8` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82369678` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82369770` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82369860` | texture-compression.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_823699F8` | texture-compression.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82369B48` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82369DB8` | texture-compression.md | Lifted | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_82369FF8` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8236A4F0` | texture-compression.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_8236AEA8` | texture-compression.md | Lifted | `__savefpr_20`, `__restfpr_20` | 2 | 0 | 34 | 2 |
| `sub_8236BF40` | texture-compression.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_823EA800` | texture-compression.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_823EACC8` | texture-compression.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_823EB910` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823EBCD0` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_823EC0A0` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823ECD38` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823ECF68` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823EDC18` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823EDFD8` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823EEBD8` | texture-compression.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_823EEE50` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823EF198` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823EF488` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_823EF850` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_823F0080` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823F0C68` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823F0EF0` | texture-compression.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_823F1640` | texture-compression.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_823F1D90` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823F3A88` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_823F3E90` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_823F40C8` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_823F4738` | texture-compression.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_823F4DA0` | texture-compression.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_823F59C8` | texture-compression.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_823F5FA0` | texture-compression.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_823F6298` | texture-compression.md | Overridden | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_823F6858` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_823F6C60` | texture-compression.md | Native | `sub_823EAB58`, `sub_823F55B0` | 2 | 2 | 197 | 0 |
| `sub_82420F50` | texture-compression.md | Native | `__savefpr_19`, `__restfpr_19` | 2 | 0 | 36 | 2 |
| `sub_82421938` | texture-compression.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82459E80` | xaudio.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8245A5E8` | xaudio.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245AB60` | xaudio-engine.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8245ACE0` | xaudio-engine.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8245AE60` | xaudio.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8245AFA0` | xaudio.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8245B3A0` | xaudio-engine.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245B480` | xaudio-engine.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8245B638` | xaudio-engine.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245B808` | audio.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8245B940` | xaudio-engine.md | Lifted | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_8245BE08` | xaudio-engine.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245C198` | xaudio-engine.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8245C958` | audio.md | Overridden | `sub_8245D118`, `__restgprlr_28` | 2 | 1 | 20 | 1 |
| `sub_8245D138` | audio.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8245D760` | xaudio-engine.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8245DDC8` | xaudio.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245E1B8` | xaudio.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245E428` | xaudio.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245E6A8` | xaudio.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245E7D0` | xaudio.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8245F108` | xaudio.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8245F298` | xaudio.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82460000` | xaudio-engine.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82460AB8` | xaudio.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82460BF0` | xaudio.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82460EF0` | xaudio.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82461158` | xaudio.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82461F20` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82462148` | xaudio-objects.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_824624F8` | xaudio-objects.md | Native | `__savegprlr_15`, `__restgprlr_15` | 2 | 0 | 45 | 2 |
| `sub_82462CF8` | xaudio-objects.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82463060` | xaudio-objects.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82463620` | xaudio-objects.md | Native | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_82463B90` | xaudio-objects.md | Native | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_82465970` | xaudio-objects.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82465CF8` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82465E50` | xaudio-objects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82466058` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82466188` | xaudio-objects.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82466888` | xaudio-objects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82466F40` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82467078` | xaudio-objects.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82467230` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_824675B8` | xaudio-objects.md | Native | `__savefpr_26`, `__restfpr_26` | 2 | 0 | 22 | 2 |
| `sub_82467A18` | xaudio-objects.md | Native | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_82469AF8` | xaudio-objects.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82469EF8` | xaudio-objects.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8246A1C0` | xaudio-objects.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8246A430` | xaudio-objects.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8246B1C8` | audio.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8246B318` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8246B480` | audio.md | Lifted | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_8246BB98` | audio.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8246BE20` | audio.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8246BFD8` | audio.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8246C1D0` | xaudio-objects.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8246C320` | audio.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8246D348` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8246D500` | xaudio-objects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8246D750` | xaudio-objects.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8246D860` | xaudio-objects.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8246E208` | xaudio-objects.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8246E4C8` | xaudio-objects.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8246E7F8` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8246EC08` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8246F300` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8246F540` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8246F7C0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8246FAC0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82470668` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82470CA0` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82471510` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_824717F8` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82471B40` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82471F68` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_824728B0` | lifted.md | Lifted | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82472BB8` | lifted.md | Lifted | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_82472F30` | lifted.md | Lifted | `__savegprlr_15`, `__restgprlr_15` | 2 | 0 | 45 | 2 |
| `sub_824733C0` | lifted.md | Lifted | `__savegprlr_15`, `__restgprlr_15` | 2 | 0 | 45 | 2 |
| `sub_82473E48` | xaudio-objects.md | Native | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82474188` | xaudio-objects.md | Native | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_82474578` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82474AE8` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82475200` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82475688` | lifted.md | Lifted | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_824759D8` | lifted.md | Lifted | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_82475E20` | lifted.md | Lifted | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_82476418` | lifted.md | Lifted | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_82476FF0` | lifted.md | Lifted | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82477348` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82477760` | lifted.md | Lifted | `__savegprlr_15`, `__restgprlr_15` | 2 | 0 | 45 | 2 |
| `sub_82477D20` | lifted.md | Lifted | `__savegprlr_15`, `__restgprlr_15` | 2 | 0 | 45 | 2 |
| `sub_8247A320` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8247AC90` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8247BEB0` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8247EBC8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8247EDA8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8247EFD8` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8247F320` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8247F558` | xaudio-objects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8247F7A8` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8247FB28` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82481B98` | lifted.md | Lifted | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_82482068` | lifted.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82575B08` | system-services.md | Native | `sub_82459C10` | 2 | 0 | 67 | 1 |
| `sub_82575BF0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82575C90` | system-services.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82575D78` | system-services.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82575E50` | system-services.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825761D8` | system-services.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825763D0` | system-services.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825765F0` | system-services.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825766E0` | system-services.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82576BE0` | system-services.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82576E78` | system-services.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_82577190` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825776A8` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82577788` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825786B8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825788C0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82578F00` | lifted.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825791B0` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82579318` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82579448` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82579590` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825796C0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82579A88` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8257BC48` | lifted.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_8257BEC0` | lifted.md | Lifted | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_8257C2B0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8257CCD8` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8257D020` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8257D298` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8257EF80` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8257EFF8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8257F070` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82582158` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825821F0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82582658` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825828D0` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82582CC8` | lifted.md | Lifted | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_82583688` | lifted.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82583910` | lifted.md | Lifted | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82584270` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825845A8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82584758` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82584A70` | lifted.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82584B90` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82586048` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825862D8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82586460` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825865E8` | lifted.md | Lifted | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_82586758` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825868E8` | lifted.md | Lifted | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_82587728` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82587A88` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825881E0` | lifted.md | Lifted | `__savegprlr_17`, `__restgprlr_17` | 2 | 0 | 41 | 2 |
| `sub_82588FF0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82589E78` | frame-driver.md | Native | `__savegprlr_20`, `__restgprlr_20` | 2 | 0 | 35 | 2 |
| `sub_8258A9E8` | car-draw.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8258ADA0` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8258AF80` | lifted.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_8258B1C0` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8258B368` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8258B678` | second-set.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8258B980` | scenery-lists.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8258C0B8` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8258C3A8` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8258D568` | video-output.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8258DAA8` | video-output.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8258E098` | shader-objects.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8258E398` | models.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8258EC38` | device-shaders.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8258ED00` | device-shaders.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8258F080` | single-commands.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8258F4E8` | single-commands.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8258FCE0` | functions.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82590040` | functions.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825901B0` | functions.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82590268` | functions.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825902E0` | functions.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825904C8` | functions.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82590698` | functions.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82590CB0` | device-shaders.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82591238` | device-shaders.md | Overridden | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_82594570` | models.md | Native | `sub_825981C8`, `__restgprlr_28` | 2 | 1 | 72 | 1 |
| `sub_82594970` | models.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_82594C18` | second-set.md | Overridden | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_82594D20` | models.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82594E88` | car-draw.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_82594FD0` | car-draw.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_82595110` | car-draw.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825960D0` | scenery-lists.md | Overridden | `__savegprlr_16`, `__restgprlr_16` | 2 | 0 | 43 | 2 |
| `sub_82596B18` | scenery-lists.md | Native | `__savegprlr_18`, `__restgprlr_18` | 2 | 0 | 39 | 2 |
| `sub_82596D48` | scenery-lists.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_82597518` | car-scene-draw.md | Overridden | `__savefpr_27`, `__restfpr_27` | 2 | 0 | 20 | 2 |
| `sub_82597610` | car-scene-draw.md | Overridden | `__savefpr_28`, `__restfpr_28` | 2 | 0 | 18 | 2 |
| `sub_825998B8` | model-animation.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_82599AF0` | models.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8259A2B0` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8259A9E0` | models.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8259AF20` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_8259D1F0` | files-and-saves.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_8259D478` | sound-cues.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8259D7B0` | ui-movies.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_8259DAD0` | ui-movies.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8259E098` | ui-movies.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_8259F848` | ui-movies.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_8259FB30` | ui-movies.md | Native | `__savefpr_26`, `__restfpr_26` | 2 | 0 | 22 | 2 |
| `sub_825A0548` | ui-movies.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825A0FE0` | model-animation.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825A1438` | model-animation.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825A1810` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825A1C88` | model-animation.md | Native | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825A2CD8` | lifted.md | Lifted | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_825A3108` | lifted.md | Lifted | `__savefpr_22`, `__restfpr_22` | 2 | 0 | 30 | 2 |
| `sub_825A31F0` | lifted.md | Lifted | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_825A3408` | lifted.md | Lifted | `__savefpr_14`, `__restfpr_14` | 2 | 0 | 46 | 2 |
| `sub_825A3980` | lifted.md | Lifted | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_825A3B80` | lifted.md | Lifted | `__savefpr_25`, `__restfpr_25` | 2 | 0 | 24 | 2 |
| `sub_825A5408` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825A5838` | lifted.md | Lifted | `__savefpr_22`, `__restfpr_22` | 2 | 0 | 30 | 2 |
| `sub_825A5E28` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825A9A90` | heap.md | Overridden | `__restgprlr_28`, `sub_825AD150` | 2 | 1 | 36 | 1 |
| `sub_825AC248` | heap.md | Native | `__restgprlr_28`, `sub_825AD150` | 2 | 1 | 36 | 1 |
| `sub_825AC970` | c-runtime-core.md | Overridden | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825ACF70` | c-runtime-core.md | Overridden | `sub_825AD02C`, `sub_825AD4C0` | 2 | 0 | 27 | 0 |
| `sub_825ADF28` | c-runtime-core.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825AE1A8` | c-runtime-core.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825AE2C0` | c-runtime-core.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825AE708` | c-runtime.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825AE9D0` | c-runtime-core.md | Overridden | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825AEE28` | c-runtime-core.md | Overridden | `__restgprlr_28`, `sub_825AD4C0` | 2 | 0 | 16 | 1 |
| `sub_825B0D10` | c-runtime-core.md | Native | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825B0F48` | files-and-saves.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_825B49C8` | c-runtime-core.md | Native | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825B4E78` | c-runtime-core.md | Overridden | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825BED08` | predicated-tiling.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825BF200` | predicated-tiling.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825BF7A8` | lifted.md | Lifted | `sub_825BF8D8`, `sub_825BFA68` | 2 | 2 | 253 | 0 |
| `sub_825BFF38` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C0178` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C0248` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C05C8` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825C08C0` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C0B60` | xact.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825C0C78` | xact.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825C0D90` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825C0F90` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C1090` | xact.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825C15E8` | lifted.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825C1788` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C1990` | xact.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825C1BC0` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825C38A8` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C39F8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C3B68` | xact.md | Native | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_825C4138` | xact.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825C4580` | lifted.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825C4FE8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C5060` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825C5128` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C5238` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C54A0` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825C5790` | xact.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825C5960` | xact.md | Native | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825C5A70` | xact.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825C5C80` | xact.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825C5E58` | xact.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825C6148` | xact.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825C6498` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C6678` | lifted.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825C6B50` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C6C10` | xact.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825C6DF8` | lifted.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825C70C0` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825C7798` | xact.md | Overridden | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CA560` | xact-waves.md | Lifted | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825CA6F8` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CA980` | xact.md | Overridden | `sub_825D0540` | 2 | 2 | 19 | 0 |
| `sub_825CAA00` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CB708` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825CB958` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CBD10` | xact.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825CC470` | xact.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CCA88` | xact.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825CDA40` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CE2F8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CE5D8` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CE7C8` | lifted.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825CF348` | lifted.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825CF598` | lifted.md | Lifted | `__savegprlr_21`, `__restgprlr_21` | 2 | 0 | 33 | 2 |
| `sub_825CFB28` | lifted.md | Lifted | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_825D2170` | xact-waves.md | Native | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825D2AE8` | xact-waves.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825D2B80` | xact-waves.md | Lifted | `__savegprlr_24`, `__restgprlr_24` | 2 | 0 | 27 | 2 |
| `sub_825D2D68` | xact-waves.md | Lifted | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825D2EF0` | xact-waves.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825D2F90` | xact-waves.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825D3030` | xact-waves.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825D3B10` | xact-waves.md | Overridden | `__savegprlr_19`, `__restgprlr_19` | 2 | 0 | 37 | 2 |
| `sub_825D3F28` | xact-waves.md | Lifted | `sub_825D3FD0`, `sub_825D41F0` | 2 | 2 | 177 | 0 |
| `sub_825D4020` | xact-waves.md | Lifted | `sub_825D3FD0`, `sub_825D41F0` | 2 | 2 | 177 | 0 |
| `sub_825D46E8` | xact-waves.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825D4B78` | xact-waves.md | Lifted | `__savegprlr_25`, `__restgprlr_25` | 2 | 0 | 25 | 2 |
| `sub_825D50F8` | xact-waves.md | Lifted | `__savegprlr_26`, `__restgprlr_26` | 2 | 0 | 23 | 2 |
| `sub_825D5A48` | xact-tracks.md | Native | `__savegprlr_22`, `__restgprlr_22` | 2 | 0 | 31 | 2 |
| `sub_825D7480` | xact-tracks.md | Lifted | `__savegprlr_27`, `__restgprlr_27` | 2 | 0 | 21 | 2 |
| `sub_825D8F88` | matrix-math.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_825D9060` | matrix-math.md | Overridden | `__savegprlr_23`, `__restgprlr_23` | 2 | 0 | 29 | 2 |
| `sub_820900C0` | c-runtime.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82090CC0` | files-and-saves.md | Overridden | `sub_8258BE48` | 1 | 0 | 6 | 1 |
| `sub_820911A8` | c-runtime.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82091328` | std-string.md | Overridden | `sub_825A7270` | 1 | 0 | 77 | 0 |
| `sub_82091C48` | app-states.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82092CD0` | app-states.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82093638` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820941F0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820954F8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82098630` | frame-passes.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820998B8` | system-services.md | Lifted | `sub_82099828` | 1 | 0 | 45 | 0 |
| `sub_82099EE8` | framework.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8209AE48` | scene-target.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8209AF40` | scene-target.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8209BE88` | app-start-up.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8209DB90` | archive.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8209F340` | racer-controllers.md | Native | `sub_82591C08` | 1 | 0 | 6 | 0 |
| `sub_8209F960` | cameras.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820A06D0` | models.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820A2F40` | cameras.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820A3978` | cameras.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820A3DB0` | cameras.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820A48B8` | files-and-saves.md | Overridden | `sub_82295C10` | 1 | 0 | 5 | 0 |
| `sub_820A4960` | files-and-saves.md | Overridden | `sub_822952E0` | 1 | 0 | 5 | 0 |
| `sub_820A4EF8` | files-and-saves.md | Overridden | `sub_82295FC8` | 1 | 0 | 113 | 0 |
| `sub_820A5110` | files-and-saves.md | Overridden | `sub_82295BF8` | 1 | 0 | 5 | 0 |
| `sub_820A51E0` | files-and-saves.md | Overridden | `sub_82295BE8` | 1 | 0 | 5 | 0 |
| `sub_820A54E8` | save-interface.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820A5660` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820A8F80` | cameras.md | Merged | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820A9BD8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820AD190` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820AD228` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820AE348` | app-states.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820AE688` | movies.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820AEA88` | movies.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820AFAE8` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B6328` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B6D48` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B6F38` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B70C8` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B72C8` | car-physics.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B77D0` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B8BA8` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B9110` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B92C8` | car-physics.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B94B0` | car-physics.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B9680` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B98B0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B99D0` | collision.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820B9F00` | collision.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820BA2F0` | collision.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820BCCE8` | collision.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820BCDD0` | collision.md | Native | `sub_820BACB8` | 1 | 0 | 9 | 0 |
| `sub_820BCE80` | collision.md | Native | `sub_820BACB8` | 1 | 0 | 9 | 0 |
| `sub_820BD8F8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C3CF8` | scene.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C4C80` | scene.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C5008` | scene.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C52A0` | scene.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C59B0` | scene.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C8950` | scene.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C8B40` | scene.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C9110` | scene.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820C9960` | car-specs.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D09B8` | scene-loaders.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D1A68` | material-passes.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D3B58` | scene-loaders.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D6238` | scene-loaders.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D63A0` | scene-loaders.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D6528` | scene-loaders.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D67E8` | car-models.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D7090` | scene-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D75F8` | scene-loaders.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D7730` | car-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D7AB0` | scene-loaders.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D8D20` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D9010` | scene-loaders.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D9110` | car-models.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820D9870` | scene-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DB330` | second-set.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DBAA0` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DBB08` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DBE80` | scene-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DC190` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DCAA0` | scene-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DDCF8` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE140` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE328` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE3C0` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE4E8` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE548` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE600` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE668` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE6E0` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DE8C8` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DEA20` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DEAF0` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DEBF0` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DEC58` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DED20` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DEED8` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DEFA8` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DF0C0` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DF1D0` | scene-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820DF7F0` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E0080` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E0B40` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E0E38` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E1138` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E1260` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E13E0` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E2A10` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E2FC8` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E30C0` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E3440` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E37F0` | scene-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E3AD8` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E3CF0` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E4078` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E4FD8` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E50C0` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E5538` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E5668` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E56D0` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E5738` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E57A0` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E5808` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E5998` | scene-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E68E8` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E74B8` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E7918` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E7C08` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820E7F90` | second-set.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820ECA30` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820ECB68` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820ED020` | car-models.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820ED528` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820EF660` | accessories.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820F0070` | accessories.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820F1930` | course-scenery.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820F1A70` | course-scenery.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820F77A0` | collision-world.md | Lifted | `sub_820F69B0` | 1 | 0 | 27 | 0 |
| `sub_820F8470` | collision-world.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820F9EA8` | track-route.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820FA0F8` | track-route.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820FA350` | track-route.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820FA4A8` | track-route.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820FB1B8` | track-route.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820FDF38` | collision-world.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820FE8F8` | collision-world.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820FF4D0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_820FF860` | race-setup.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82101EB0` | race-setup.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821043F0` | replay-snapshots.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82104EA8` | replay-snapshots.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82105C18` | race-states.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82105F50` | race-states.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821060A0` | game-modes.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821062E8` | game-modes.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82107B90` | boot-mode.md | Overridden | `sub_825894C8` | 1 | 0 | 75 | 0 |
| `sub_82107BF8` | boot-mode.md | Overridden | `sub_825895A0` | 1 | 0 | 61 | 0 |
| `sub_82107DD0` | boot-mode.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82108A70` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210A490` | strings-and-font.md | Overridden | `sub_825A9A80` | 1 | 0 | 7 | 0 |
| `sub_8210A5E8` | strings-and-font.md | Overridden | `sub_825A7EF8` | 1 | 0 | 20 | 0 |
| `sub_8210ADD0` | strings-and-font.md | Overridden | `sub_82109A00` | 1 | 0 | 23 | 1 |
| `sub_8210BAB0` | action-mappers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210BB38` | action-mappers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210C370` | action-mappers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210D320` | car-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210D7F8` | car-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210DB60` | car-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210E030` | car-sounds.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210E5C0` | car-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210ECF0` | car-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8210EDB0` | car-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82111FC8` | career.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82113730` | save-data.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82114A88` | career.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82114B08` | career.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82114B88` | career.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82116CE0` | career.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82117348` | sound-cues.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82117FF0` | menu-sounds.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82118320` | menu-sounds.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82118850` | sound-cues.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821189F0` | menu-sounds.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82118CF0` | menu-sounds.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82118E50` | menu-sounds.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211AAA8` | ui-movies-game.md | Overridden | `sub_8259D648` | 1 | 0 | 7 | 0 |
| `sub_8211AD00` | ui-movies-game.md | Overridden | `sub_8259D798` | 1 | 0 | 8 | 0 |
| `sub_8211AF90` | ui-movies-game.md | Merged | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211B018` | ui-movies-game.md | Merged | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211B660` | ui-movies-game.md | Overridden | `sub_8211B310` | 1 | 0 | 30 | 0 |
| `sub_8211B7B8` | ui-movies-game.md | Native | `sub_8211B310` | 1 | 0 | 30 | 0 |
| `sub_8211B940` | ui-movies-game.md | Native | `sub_8211B310` | 1 | 0 | 30 | 0 |
| `sub_8211B9B8` | ui-movies-game.md | Native | `sub_8211B310` | 1 | 0 | 30 | 0 |
| `sub_8211C1F8` | ui-movies-game.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211C2C8` | ui-movies-game.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211C740` | ui-movies-game.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211C7E8` | ui-movies-game.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211CA18` | ui-movies-game.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211D880` | ui-movies-game.md | Overridden | `sub_8211C930` | 1 | 0 | 49 | 1 |
| `sub_8211D8D8` | ui-movies-game.md | Overridden | `sub_8211C9C0` | 1 | 0 | 31 | 1 |
| `sub_8211DE38` | ui-movies-game.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8211E808` | ui-movies-game.md | Overridden | `sub_8211A9E8` | 1 | 0 | 52 | 0 |
| `sub_8211F300` | race-overlays.md | Overridden | `sub_8211F3B4` | 1 | 0 | 5 | 0 |
| `sub_82121760` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82122958` | race-overlays.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82125610` | race-overlays.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82129A08` | sprites.md | Lifted | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_8212E660` | race-overlays.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8212FE80` | screen-objects.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82131F50` | screen-objects.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82133D00` | post-effects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82134410` | lifted.md | Lifted | `sub_82134698` | 1 | 1 | 729 | 0 |
| `sub_82134618` | lifted.md | Lifted | `sub_82134698` | 1 | 1 | 729 | 0 |
| `sub_82139750` | ai-drivers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82139EA8` | ai-drivers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8213AB98` | ai-drivers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8213B428` | ai-drivers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8213C908` | ai-drivers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8213CD50` | ai-drivers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8213DC08` | ai-drivers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8213EAB0` | ai-drivers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8213F530` | ai-drivers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82142C80` | race-event.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82142CE0` | race-event.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82143528` | race-event.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82144300` | race-event.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82144458` | race-event.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82144950` | race-event.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82144E58` | race-event.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82147958` | scenery-lists.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82147FB8` | scenery-lists.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82148650` | scenery-lists.md | Native | `sub_8209BD40` | 1 | 0 | 6 | 1 |
| `sub_82149B08` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8214B330` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8214C828` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8215D2B0` | race-update.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8215E160` | race-overlays.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8215F5F0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8215F678` | ai-drivers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8215F7E8` | racer-controllers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8215FB08` | race-flow.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8215FB90` | racer-controllers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8215FC38` | racer-controllers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82160948` | racer-controllers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82164F08` | race-states.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82168708` | race-states.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8216D198` | race-hud-machine.md | Overridden | `sub_8221C168` | 1 | 0 | 44 | 0 |
| `sub_8216D270` | race-hud-machine.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82170668` | race-world.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82170FF8` | racer-controllers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82174BC0` | race-world.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821762F8` | race-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82176618` | race-sounds.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821767A8` | race-sounds.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821769D0` | race-sounds.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82176F28` | race-sounds.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82177130` | race-sounds.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8217B3D8` | stand-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8217D2B0` | front-end-car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8217FCB0` | stand-models.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8217FDC8` | car-makers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8217FE60` | front-end-car-draw.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821800B0` | type6-type7-modes.md | Native | `sub_8217D760` | 1 | 0 | 37 | 1 |
| `sub_821807D8` | car-list.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82180E70` | car-dealer.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82180F20` | car-dealer.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82181FC0` | car-dealer.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82182A80` | car-dealer.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82182C28` | car-dealer.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82182DD0` | car-dealer.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82184650` | car-dealer.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821869B8` | type2-dialog.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82186E40` | type2-dialog.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82187CC8` | type2-dialog.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82187D80` | type2-dialog.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821880D0` | type2-dialog.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82188C00` | race-menu.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821890D8` | race-menu.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8218C9C8` | race-menu.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8218D708` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8218E078` | type2-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8218E1A8` | type2-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8218E420` | type2-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8218E530` | type2-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8218E810` | type2-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8218FB20` | type2-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8218FCC8` | type2-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821903D0` | type2-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82191628` | garage-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82191D70` | garage-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82192EF8` | garage-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82193180` | garage-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82193288` | garage-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82193320` | garage-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82193B10` | garage-screen.md | Native | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_82193BA8` | garage-screen.md | Native | `sub_825A9934` | 1 | 0 | 19 | 0 |
| `sub_821955E0` | garage-screen.md | Native | `sub_8211A630` | 1 | 0 | 15 | 0 |
| `sub_82195E30` | garage-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82195F10` | garage-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82196030` | garage-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82196170` | garage-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82197B00` | type2-mode.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821982F0` | type2-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82198830` | type2-screens.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82198F08` | type2-screens.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82199350` | type2-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82199688` | type2-screens.md | Native | `sub_8211A630` | 1 | 0 | 15 | 0 |
| `sub_821997E8` | type2-screens.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82199AA8` | type2-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8219A8E0` | type2-screens.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8219BE98` | type2-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8219D1B0` | build-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8219D4B8` | build-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821A1008` | build-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821A1258` | build-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821A26F0` | save-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821A28E0` | save-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821A5118` | save-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821A5540` | front-end-screen-helpers.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821AA408` | car-settings.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821AA490` | car-settings.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821AB2C0` | car-settings.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821ABEF8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821ACF28` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821AD5D8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821ADAF0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821ADC48` | car-settings.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821ADD00` | car-settings.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821ADDB8` | car-settings.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821ADE70` | car-settings.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821AEB48` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B2480` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B38A0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B3C68` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B3ED0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B4548` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B46D8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B51C0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B5570` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B56C8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821B8900` | type2-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821BB9A8` | options-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821BBBA0` | options-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821BF158` | options-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821BF3C0` | options-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821BF698` | options-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821BF908` | options-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821BFA68` | options-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821BFD98` | type6-type7-modes.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C0030` | type6-type7-modes.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C04E0` | options-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C0750` | options-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C0EF0` | options-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C1498` | options-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C2778` | options-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C2A10` | options-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C3278` | options-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C51D0` | options-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C6D48` | front-end-screen-helpers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821C6E50` | front-end-screen-helpers.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821CBEE8` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821CBFC0` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821CE9E8` | tuning-machine.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_821D1A18` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821D32C8` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821D4540` | tuning-pages.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821D5610` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821D6230` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821D6420` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821D8670` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821DF350` | power-graph.md | Overridden | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_821DFD48` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821E07B0` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821E7368` | tuning-machine.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821E8068` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821E8A88` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821E9730` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EA460` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EAFA0` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EB8E8` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EBDC8` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EC038` | tuning-pages.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EC6A0` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EDB70` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EE6D0` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821EE760` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821F32C8` | tuning-pages.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821F35C0` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821F3A68` | tuning-pages.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821F8070` | sticker-editor.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821FAA28` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821FBF18` | sticker-editor.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821FD128` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821FDF10` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821FED30` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_821FEE10` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822019D0` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82203400` | tuning-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82203A30` | tuning-pages.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82206A50` | tuning-pages.md | Native | `sub_82200DC0` | 1 | 0 | 418 | 1 |
| `sub_82206F48` | tuning-pages.md | Native | `sub_822012E8` | 1 | 0 | 268 | 0 |
| `sub_82207320` | tuning-pages.md | Native | `sub_82201620` | 1 | 0 | 320 | 0 |
| `sub_8220B070` | user-info.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8220C4C8` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8220D5D0` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8220E0F8` | race-screens.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8220E418` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8220E530` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8220F500` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8220F840` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8220FC98` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822106E8` | race-screens-unseen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82210B08` | race-screens-unseen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822121C0` | race-hud-object.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822129A8` | race-hud-object.md | Native | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_82213510` | race-hud-object.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822135B0` | race-hud-object.md | Native | `sub_8211AB90` | 1 | 0 | 8 | 0 |
| `sub_822141A0` | race-hud-object.md | Native | `sub_8211AB90` | 1 | 0 | 8 | 0 |
| `sub_822141E8` | race-hud.md | Overridden | `sub_82212908` | 1 | 0 | 27 | 1 |
| `sub_82214C88` | race-hud-object.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82217380` | alt-hud.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82217778` | alt-hud.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822197A8` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82219D00` | race-screens.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8221A3E8` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8221B5C8` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8221BA68` | race-screens.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822204F8` | title-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82220610` | title-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82220698` | title-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82220C18` | title-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82221EC0` | title-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82221FC8` | title-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82222038` | title-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82222A50` | title-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82223700` | title-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82223840` | title-screen.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82223B78` | title-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82223C38` | title-screen.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82226618` | type5-mode.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82226978` | type5-mode.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82229678` | location-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8222AAB0` | location-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8222D4F8` | location-screen.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82230790` | type67-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_822324E8` | type6-type7-modes.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822325E0` | type6-type7-modes.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82232910` | type6-type7-modes.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82233378` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82233B18` | type6-type7-modes.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82233E80` | type6-type7-modes.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82236FD8` | type67-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_822416D8` | screen-constructors.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822417E8` | car-list.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822420C0` | type6-type7-modes.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82242388` | type6-type7-modes.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82242F70` | type67-screens.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_822433D8` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82244080` | type6-type7-modes.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_822443E0` | type6-type7-modes.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822478D0` | race-loading.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82247AA0` | data-load.md | Native | `sub_8211A9E8` | 1 | 0 | 52 | 0 |
| `sub_82247FE0` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82248260` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82248300` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822483B0` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82248650` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82249EB8` | car-list.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224A490` | car-list.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224A570` | car-list.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224D2D0` | car-list.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224D358` | car-list.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224D4A0` | car-list.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224D518` | car-list.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224E050` | car-list.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224E108` | car-list.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224E1A8` | car-list.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224E288` | car-list.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224E7E8` | car-list.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8224FB20` | car-list.md | Native | `sub_8211A630` | 1 | 0 | 15 | 0 |
| `sub_82250488` | type6-type7-modes.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82250D70` | power-graph-screen.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_82251D00` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82251E10` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82252068` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82252A88` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82252BB8` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82252D38` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82255890` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82256360` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82256908` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82259368` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82259B70` | type67-screens.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8225A8A8` | todays-result.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8225A930` | todays-result.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8225BF78` | todays-result.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8225C160` | todays-result.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8225C648` | todays-result.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8225CA50` | todays-result.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8225D918` | todays-result.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82260800` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82288E68` | type10-mode.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822894E8` | type10-mode.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82289570` | type10-mode.md | Overridden | `sub_8211BE98` | 1 | 0 | 27 | 0 |
| `sub_8228A0D8` | type10-mode.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8228CE20` | track-zones.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8228D418` | track-zones.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8228D778` | track-zones.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8228D870` | track-zones.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8228E788` | track-zones.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8228FD50` | save-interface.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822902A0` | save-interface.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822912A0` | data-load.md | Native | `sub_8211A9E8` | 1 | 0 | 52 | 0 |
| `sub_82291328` | data-load.md | Overridden | `sub_8259D798` | 1 | 0 | 8 | 0 |
| `sub_82292DB8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82293988` | matrix-math.md | Overridden | `sub_82293598` | 1 | 0 | 5 | 0 |
| `sub_82295D48` | files-and-saves.md | Native | `sub_825A7F30` | 1 | 0 | 12 | 0 |
| `sub_822962B0` | c-runtime.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82296720` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82297540` | functions.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822977B0` | frame-driver.md | Native | `sub_82296E68` | 1 | 1 | 5 | 0 |
| `sub_82297888` | frame-driver.md | Native | `sub_82296E68` | 1 | 1 | 5 | 0 |
| `sub_82297B38` | c-runtime-core.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82297CE0` | system-services.md | Overridden | `sub_82297C08` | 1 | 0 | 66 | 0 |
| `sub_82297D58` | video-output.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82298198` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8229E100` | command-buffer.md | Native | `sub_8229DC28` | 1 | 0 | 55 | 1 |
| `sub_8229E768` | command-buffer.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8229EF48` | command-buffer.md | Overridden | `sub_8229E5D0` | 1 | 0 | 119 | 1 |
| `sub_8229F228` | command-buffer.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822A0718` | functions.md | Lifted | `sub_822A0688` | 1 | 1 | 43 | 0 |
| `sub_822A3450` | device-state.md | Overridden | `sub_822A0F80` | 1 | 0 | 267 | 0 |
| `sub_822A3A20` | device-state.md | Native | `sub_822B0C38` | 1 | 0 | 92 | 0 |
| `sub_822A3B90` | device-state.md | Overridden | `sub_822A0F80` | 1 | 0 | 267 | 0 |
| `sub_822A5C50` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822A6630` | graphics-resources.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822A6830` | graphics-resources.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822A6968` | graphics-resources.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822A8618` | device-shaders.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822A8808` | device-shaders.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822AD500` | functions.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822AE9E8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822AF550` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822AFA58` | functions.md | Overridden | `sub_822A0F80` | 1 | 0 | 267 | 0 |
| `sub_822B1CB0` | functions.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822B4250` | functions.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822B64A0` | predicated-tiling.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822B6528` | predicated-tiling.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822B8F10` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_822B8F78` | matrix-math.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8235A688` | car-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8235A990` | models.md | Native | `sub_822A5B68` | 1 | 0 | 7 | 1 |
| `sub_8236B718` | texture-compression.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_823EA770` | texture-compression.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_823EAE40` | texture-compression.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_823F5E30` | texture-compression.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82421EB0` | texture-compression.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82459D60` | xaudio-engine.md | Overridden | `sub_8245C638` | 1 | 0 | 9 | 0 |
| `sub_8245A050` | xaudio.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8245B208` | xaudio-engine.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8245B720` | xaudio-engine.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8245BF80` | xaudio-engine.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8245C538` | xaudio-engine.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8245C8D0` | audio.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8245EAA0` | xaudio.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8245F3D0` | xaudio.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8245FE88` | xaudio-engine.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_824600F0` | xaudio-engine.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82460688` | xaudio.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82460880` | xaudio.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82460A58` | xaudio.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82460B50` | xaudio.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82460CB8` | xaudio.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82460D50` | xaudio.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82461240` | xaudio.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82461798` | xaudio.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_824619B0` | xaudio.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82461D50` | xaudio.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82463410` | xaudio-objects.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82465710` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82467140` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82467360` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_824696F0` | xaudio-objects.md | Native | `sub_82469728` | 1 | 1 | 48 | 0 |
| `sub_8246AB48` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246AC40` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246AD98` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246B0C8` | audio.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246D008` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246D0C8` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246D1E0` | xaudio-objects.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246D410` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246D630` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8246F140` | xaudio-objects.md | Overridden | `sub_8246F270` | 1 | 1 | 39 | 0 |
| `sub_824701E0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8247D110` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82480E90` | car-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82480F80` | car-sounds.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82576328` | system-services.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82577630` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82578D70` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82579C48` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8257C3E0` | app-start-up.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8257C610` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8257EAF0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8257EDE8` | app-start-up.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8257F970` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8257FB38` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82582EC0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82589158` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82589990` | frame-passes.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258AB18` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258AC58` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258BFA0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258C820` | files-and-saves.md | Overridden | `sub_82295FC8` | 1 | 0 | 113 | 0 |
| `sub_8258C898` | files-and-saves.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258DF18` | frame-driver.md | Overridden | `sub_825BA158` | 1 | 0 | 7 | 1 |
| `sub_8258E1D8` | shader-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258E630` | device-shaders.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258EA58` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258EAE0` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258EF60` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258EFC0` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258F028` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258F3E0` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258F438` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258F490` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258F568` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258F8F0` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258F948` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258F9A0` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258FBD0` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258FC30` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258FC90` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258FF50` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258FFA0` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8258FFF0` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825900B8` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590360` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590428` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590478` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590620` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590768` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590860` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825908E0` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590968` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825909E8` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590A68` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82590AD0` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825916F0` | single-commands.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82591798` | functions.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82592400` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82592740` | std-string.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82593040` | std-string.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82594850` | models.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825953D0` | scenery-lists.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82595490` | scenery-lists.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82595550` | models.md | Overridden | `sub_825A1148` | 1 | 1 | 54 | 0 |
| `sub_82596E88` | models.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82596F68` | models.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_82597210` | car-scene-draw.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8259B660` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8259C9B0` | sound-cues.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8259D688` | ui-movies.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_8259F770` | ui-movies.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A16B0` | model-animation.md | Lifted | `sub_825A1708` | 1 | 1 | 37 | 0 |
| `sub_825A17A0` | model-animation.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A2250` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A22E8` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A5020` | lifted.md | Lifted | `sub_825A5610` | 1 | 1 | 131 | 0 |
| `sub_825A5320` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A5790` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A5D48` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A6138` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A6460` | xaudio-objects.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A6570` | ui-movies.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825A6948` | c-runtime-core.md | Native | `sub_825AD4C0` | 1 | 0 | 6 | 0 |
| `sub_825A71B0` | c-runtime-core.md | Overridden | `sub_825A70F0` | 1 | 0 | 65 | 0 |
| `sub_825AA218` | c-runtime.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825AE8E0` | c-runtime.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825B31D8` | c-runtime-core.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825B40B8` | c-runtime-core.md | Overridden | `sub_825AD4C0` | 1 | 0 | 6 | 0 |
| `sub_825BFCC8` | lifted.md | Lifted | `sub_825BFCFC` | 1 | 1 | 44 | 0 |
| `sub_825C0360` | xact.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C0E88` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C11A0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C1238` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C12F0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C1800` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C3EE0` | xact.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C4C20` | xact.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C4CF8` | xact.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C4D90` | xact.md | Overridden | `sub_825C4DC8` | 1 | 1 | 18 | 0 |
| `sub_825C5DA0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C63D0` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C6A20` | xact.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C7018` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C7A38` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C7F30` | xact.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825C7F98` | xact.md | Overridden | `sub_825C7FE8` | 1 | 1 | 53 | 0 |
| `sub_825C8230` | xact.md | Overridden | `sub_825C7B10` | 1 | 1 | 32 | 0 |
| `sub_825C8C98` | lifted.md | Lifted | `sub_825C8CD4` | 1 | 1 | 65 | 0 |
| `sub_825CA4B8` | xact-waves.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CAE90` | xact.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CBB28` | xact.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CBC58` | xact.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CC1D0` | xact.md | Overridden | `sub_825CC148` | 1 | 1 | 56 | 0 |
| `sub_825CC380` | xact.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CC9C8` | xact.md | Overridden | `sub_825CC858` | 1 | 1 | 48 | 0 |
| `sub_825CCDB0` | xact.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CCF10` | xact.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CD020` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CD6A8` | lifted.md | Lifted | `sub_825CD6EC` | 1 | 1 | 22 | 0 |
| `sub_825CD858` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CDB10` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CDC50` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CE450` | lifted.md | Lifted | `sub_825CE49C` | 1 | 1 | 25 | 0 |
| `sub_825CECE8` | xact.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825CFF90` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825D02F8` | xact-tracks.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825D09D0` | xact-tracks.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825D1478` | xact-tracks.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825D2030` | xact-waves.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825D3428` | xact-waves.md | Lifted | `sub_825D3470` | 1 | 1 | 32 | 0 |
| `sub_825D53D8` | xact-tracks.md | Native | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825D86F8` | xact-tracks.md | Overridden | `__restgprlr_28` | 1 | 0 | 10 | 1 |
| `sub_825D9310` | lifted.md | Lifted | `__restgprlr_28` | 1 | 0 | 10 | 1 |

## Ours functions whose original body is still reachable

Ours by their rows, but something reaches their recompiled body: an `__imp__` fallback in our C++ (taken when the native C++ declines: a block not in our heap, a flag of the callee's off), a guest call to a function with no override and no C++, or an unported caller of one without an override. Each with its callers in that variant (native ones marked `n`):



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
