# B7's static map

Made by `itc_recomp/tools/static_map.py` on every run; never edit it by hand (run the tool again). What it reads, how edges are found and its gaps: the tool's docstring and "The static map" in `docs/coverage.md`.

Made 2026-11-16 at commit `ec9c86b4` (`rexglue-removal-project`).

## Totals

- Recompiled `sub_` functions: 16552; 144 jump-table cases folded into their parents, leaving 16408.
- Reachable: 13777 (11866 from `main`, 1911 more from 190 extra roots: functions seen in play that `main` doesn't reach, 2 of them not under another).
- Ours: 10187. Excluded (replaced libraries): 3581.
- **Unported: 9** (6 under `main`): seen in play 1, never seen 8; 1282 generated lines.
- Of the unported, 6 hang under unported code (or a root); 3 are reached only through ours (0 seen in play), 2 of them called by our C++ itself (a guest call by name or address, a fallback for a branch, a table or vtable it stores) and the rest under those; the TSV's `behind_ours` column lists them.
- Ours functions' edges come from our C++ (`src_edges.py`): 47610 definitions in 1238 files of `src/` (harnesses left out), 10398 functions with C++ (an override or `ITC_NATIVE_RUNNING`), 10166 of them reachable. Ours functions without C++ (Merged, or whose print names no address) keep the original's edges where something still reaches them.
- Ours functions whose original body is still reachable: 7 (7 from our C++: an `__imp__` fallback when the native C++ declines, or a guest call to a function with neither override nor C++); listed below.
- Host code no native function's C++ reaches but that names guest functions (constants defined but not resolved to a user, a few hooks' helpers): 3760 functions named, taken as roots; they add 1674 reachable functions.
- Before 2026-11-14's fix the graph followed the original's edges through ours too: 15901 reachable. **Originals no longer called: 538** (279 not ours, 259 ours whose work our C++ took in; 38483 lines, 135 seen in play): reachable through the original's graph (outside the replaced libraries), not through ours; below, by the native function that left the call out.
- Frontier (ready now): 7 functions.
- Reachable functions calling imports: 325 (2 unported).
- Image read: 13492 code-built data addresses scanned; 181 function pointers in data that no code-built address reaches (136 functions; not edges).

A `*` after a function: never seen in play. Sizes are generated lines.

## Blockers, by what they unblock

Unported functions ranked by how many unported functions above them (through unported callers) wait on them; "frees" is how many would join the frontier if this one alone became ours.

| Function | Unblocks | Frees | Lines | Seen | Ready | File |
|---|---|---|---|---|---|---|
| `sub_825B3400` | 1 | 1 | 18 | no | yes | unmapped |
| `sub_825B88D4` | 1 | 1 | 21 | no | yes | unmapped |
| `sub_821743A8` | 0 | 0 | 657 | no | yes | race-world.md |
| `sub_822785E0` | 0 | 0 | 223 | no | yes | network-mode.md |
| `xstart` | 0 | 0 | 148 | yes | yes | unmapped |
| `sub_825B0BB0` | 0 | 0 | 8 | no | yes | unmapped |
| `sub_825B0BE4` | 0 | 0 | 12 | no | yes | unmapped |
| `sub_825B32F0` | 0 | 0 | 89 | no | no | unmapped |
| `sub_825B8778` | 0 | 0 | 106 | no | no | unmapped |

## Frontier clusters, by what they unblock

7 clusters (frontier functions sharing an unported caller within 0x4000 bytes, neighbors in address order, or a call cycle). Each: what it unblocks, its functions with their sizes, its function files.

1. **sub_825B3400**-825B3400: unblocks 1; 1 functions, 18 lines, 0 seen; unmapped. `sub_825B3400`* 18
2. **sub_825B88D4**-825B88D4: unblocks 1; 1 functions, 21 lines, 0 seen; unmapped. `sub_825B88D4`* 21
3. **sub_821743A8**-821743A8: unblocks 0; 1 functions, 657 lines, 0 seen; race-world.md. `sub_821743A8`* 657
4. **sub_822785E0**-822785E0: unblocks 0; 1 functions, 223 lines, 0 seen; network-mode.md. `sub_822785E0`* 223
5. **xstart**-rt: unblocks 0; 1 functions, 148 lines, 1 seen; unmapped. `xstart` 148
6. **sub_825B0BB0**-825B0BB0: unblocks 0; 1 functions, 8 lines, 0 seen; unmapped. `sub_825B0BB0`* 8
7. **sub_825B0BE4**-825B0BE4: unblocks 0; 1 functions, 12 lines, 0 seen; unmapped. `sub_825B0BE4`* 12

## Unmapped areas

Clusters of unported reachable functions with no rows in any function file: 7 (7 functions, 402 lines). Largest first; the callers named are outside the cluster.

- **xstart**-rt: 1 functions, 148 lines, 1 seen, unblocks 0; called from nothing outside it.
- **sub_825B0BB0**-825B0BB0: 1 functions, 8 lines, 0 seen, unblocks 0; called from `sub_825B0068` (lifted.md, ours).
- **sub_825B0BE4**-825B0BE4: 1 functions, 12 lines, 0 seen, unblocks 0; called from `sub_825B0470` (lifted.md, ours).
- **sub_825B32F0**-825B32F0: 1 functions, 89 lines, 0 seen, unblocks 0; called from `sub_825B3430` (c-runtime.md, ours).
- **sub_825B3400**-825B3400: 1 functions, 18 lines, 0 seen, unblocks 1; called from `sub_825B32F0` (unmapped, unported).
- **sub_825B8778**-825B8778: 1 functions, 106 lines, 0 seen, unblocks 0; called from `sub_825B4490` (lifted.md, ours).
- **sub_825B88D4**-825B88D4: 1 functions, 21 lines, 0 seen, unblocks 1; called from `sub_825B8778` (unmapped, unported).

## Extra roots

Seen in play but not reachable from `main` (static edges miss them, or they run outside `main`: start-up, threads, callbacks), not under another extra root:

`xstart` (unported), `sub_822ADB50` (ours)

## Originals no longer called

Reachable through the original's graph but not through ours: the native function's C++ left the call out (plugged-in data, a dropped call, work merged into the C++). Grouped by that native function: its original's callees no longer reached, then how many functions hang only under them (a function under several groups counts in each; "not ours" leaves out those ours already, mostly Merged). Candidates for Dropped or "Not called" rows, for the user's review; the TSV's `lost` column lists them all.

| Native caller | File | Status | Lost callees | Under them | Not ours | Lines | Seen |
|---|---|---|---|---|---|---|---|
| `sub_822ACB90` | device-state.md | Native | `sub_822A1280`, `sub_822A12B0`, `sub_822A12F0`, `sub_822A1380`, `sub_822A1410`, `sub_822A14A0`, `sub_822A1530`, `sub_822A15A0`, ... 90 more | 98 | 98 | 739 | 0 |
| `sub_8235AAC0` | texture-compression.md | Native | `sub_82365D88`, `sub_82366458`, `sub_82368170`, `sub_8236D380` | 56 | 54 | 5486 | 1 |
| `sub_82107670` | boot-mode.md | Lifted | `sub_8223D3B8`, `sub_8223D4D0`, `sub_8223D5D8`, `sub_8223D8E8`, `sub_8223D960`, `sub_8223DA00`, `sub_8223DBC0`, `sub_8223DD90` | 54 | 0 | 5652 | 0 |
| `sub_821076F8` | boot-mode.md | Lifted | `sub_8223D3B8`, `sub_8223D4D0`, `sub_8223D5D8`, `sub_8223D8E8`, `sub_8223D960`, `sub_8223DA00`, `sub_8223DBC0`, `sub_8223DD90` | 54 | 0 | 5652 | 0 |
| `sub_821A1380` | car-makers.md | Lifted | `sub_8219C528`, `sub_8219C6D0`, `sub_8219C850`, `sub_8219C8F8`, `sub_8219CA08`, `sub_8219CB18`, `sub_8219CBD8`, `sub_8219FC60`, ... 6 more | 28 | 0 | 2045 | 28 |
| `sub_8214E268` | system-services.md | Overridden | `sub_82149080`, `sub_8214ADC0`, `sub_8214AEA8`, `sub_8214F160`, `sub_8214F290`, `sub_8214F810`, `sub_8214FA78` | 23 | 19 | 1662 | 0 |
| `sub_8222ED98` | location-screen.md | Lifted | `sub_8222A470`, `sub_8222A718`, `sub_8222AD50`, `sub_8222AE00`, `sub_8222B8A8`, `sub_8222BE60`, `sub_8222CFF0`, `sub_8222D108`, ... 10 more | 23 | 0 | 2988 | 23 |
| `sub_822979E0` | c-runtime-core.md | Lifted | `sub_825A6E48`, `sub_825B6958`, `sub_825D9590` | 15 | 0 | 1072 | 0 |
| `sub_822185B0` | race-screens.md | Native | `sub_82217D40`, `sub_82217E20`, `sub_82217F40`, `sub_82218168`, `sub_822183A8`, `sub_82218478` | 12 | 10 | 893 | 2 |
| `sub_8220F638` | race-screens.md | Native | `sub_8220C578`, `sub_8220C5D0`, `sub_8220D898`, `sub_8220DB98`, `sub_8220E988`, `sub_8220EBD8` | 11 | 0 | 1175 | 10 |
| `sub_8259C3A8` | models.md | Native | `sub_8259B748`, `sub_8259BE40` | 11 | 11 | 1838 | 0 |
| `sub_8221BD20` | race-screens.md | Native | `sub_8221A4A8`, `sub_8221A610`, `sub_8221A680`, `sub_8221A6C8`, `sub_8221A720`, `sub_8221B298`, `sub_8221B860`, `sub_8221B8A8` | 10 | 5 | 615 | 5 |
| `sub_825C3050` | lifted.md | Lifted | `sub_825C31D8` | 10 | 2 | 809 | 9 |
| `sub_825C3110` | xact.md | Lifted | `sub_825C31D8` | 10 | 2 | 809 | 9 |
| `sub_825B13D0` | c-runtime.md | Native | `sub_825B1368`, `sub_825B7EB8`, `sub_825BA4A0` | 8 | 5 | 701 | 0 |
| `sub_820990F0` | input.md | Overridden | `sub_82591918`, `sub_82591C18` | 7 | 2 | 617 | 0 |
| `sub_822A08A0` | functions.md | Overridden | `sub_822A05A8`, `sub_822B1F68`, `sub_822B20D8`, `sub_822B2388`, `sub_822B3FE0`, `sub_822B5C58` | 7 | 2 | 758 | 0 |
| `sub_82092358` | frame-driver.md | Lifted | `sub_82091A60`, `sub_82091EF8`, `sub_820920D8` | 6 | 2 | 273 | 5 |
| `sub_822968B8` | functions.md | Overridden | `sub_82297678` | 6 | 0 | 613 | 0 |
| `sub_820CDC18` | race-setup.md | Native | `sub_820CC450`, `sub_820CC458`, `sub_820CC460`, `sub_820CC468`, `sub_820CC478` | 5 | 1 | 37 | 4 |
| `sub_8221A090` | race-screens.md | Native | `sub_822191D0`, `sub_82219A78`, `sub_82219B60`, `sub_82219C30` | 4 | 2 | 249 | 2 |
| `sub_82299D60` | heap.md | Overridden | `sub_82299208`, `sub_8229A5F4` | 4 | 4 | 704 | 4 |
| `sub_8229A930` | heap.md | Overridden | `sub_82298A88`, `sub_8229B16C` | 4 | 4 | 938 | 4 |
| `sub_82579DA0` | lifted.md | Lifted | `sub_82579D18`, `sub_82579E18`, `sub_8257A498` | 4 | 4 | 436 | 0 |
| `sub_825AF130` | c-runtime.md | Native | `sub_825AF000`, `sub_825AF098`, `sub_825B6890` | 4 | 2 | 207 | 0 |
| `sub_820921A8` | frame-driver.md | Lifted | `sub_82091EF8` | 3 | 0 | 114 | 2 |
| `sub_820EF168` | car-models.md | Native | `sub_820EF0B8` | 3 | 3 | 174 | 0 |
| `sub_8229A648` | heap.md | Overridden | `sub_82298548`, `sub_82299448`, `sub_8229A8DC` | 3 | 3 | 581 | 3 |
| `sub_825A6F60` | c-runtime.md | Native | `sub_825A7044`, `sub_825AA1A8` | 3 | 0 | 27 | 3 |
| `sub_8209D130` | mode-tick.md | Overridden | `__savevmx_124`, `__restvmx_124` | 2 | 0 | 34 | 2 |
| `sub_8209E118` | files-and-saves.md | Overridden | `sub_8209DD00`, `sub_822954C8` | 2 | 1 | 63 | 0 |
| `sub_820A4610` | system-services.md | Overridden | `sub_820A45C0` | 2 | 0 | 33 | 0 |
| `sub_820A4C58` | files-and-saves.md | Overridden | `sub_82295BC8`, `sub_82295C10` | 2 | 0 | 18 | 0 |
| `sub_820B2808` | car-physics.md | Native | `sub_820B6938`, `sub_820B6940` | 2 | 2 | 11 | 0 |
| `sub_820B3F80` | car-physics.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820B4778` | car-physics.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_820B4AA8` | car-physics.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820BD508` | collision.md | Native | `sub_820BACB8`, `sub_820BCF28` | 2 | 1 | 162 | 0 |
| `sub_820BD6E0` | collision.md | Native | `sub_820BACB8`, `sub_820BCF28` | 2 | 1 | 162 | 0 |
| `sub_820BDA48` | lifted.md | Lifted | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820BF320` | collision.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820C1E00` | vehicle.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820C5DE8` | car-scene-draw.md | Overridden | `__savefpr_18`, `__restfpr_18` | 2 | 0 | 38 | 2 |
| `sub_820C9170` | car-draw.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_820D3E98` | scene-loaders.md | Native | `__savevmx_123`, `__restvmx_123` | 2 | 0 | 40 | 2 |
| `sub_820D9F50` | second-set.md | Native | `__savevmx_123`, `__restvmx_123` | 2 | 0 | 40 | 2 |
| `sub_820F9768` | track-route.md | Overridden | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_820FC900` | collision-world.md | Native | `__savefpr_15`, `__restfpr_15` | 2 | 0 | 44 | 2 |
| `sub_82106158` | game-modes.md | Overridden | `sub_82105FE0`, `sub_82106100` | 2 | 0 | 57 | 2 |
| `sub_8210D478` | car-sounds.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_8210E938` | car-sounds.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82120008` | lifted.md | Lifted | `__savefpr_15`, `__restfpr_15` | 2 | 0 | 44 | 2 |
| `sub_82120868` | lifted.md | Lifted | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82121D78` | lifted.md | Lifted | `__savefpr_15`, `__restfpr_15` | 2 | 0 | 44 | 2 |
| `sub_82123E48` | race-overlays.md | Lifted | `__savefpr_19`, `__restfpr_19` | 2 | 0 | 36 | 2 |
| `sub_82125B60` | race-overlays.md | Overridden | `__savefpr_21`, `__restfpr_21` | 2 | 0 | 32 | 2 |
| `sub_8212BB20` | sprites.md | Lifted | `__savefpr_21`, `__restfpr_21` | 2 | 0 | 32 | 2 |
| `sub_82130618` | lifted.md | Lifted | `__savefpr_18`, `__restfpr_18` | 2 | 0 | 38 | 2 |
| `sub_82131DD8` | screen-objects.md | Native | `sub_8212F030`, `sub_821318B8` | 2 | 2 | 358 | 0 |
| `sub_82132BD8` | screen-objects.md | Native | `sub_8212EFA8`, `sub_82132318` | 2 | 2 | 361 | 0 |
| `sub_82134060` | post-effects.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82136FA8` | post-effects.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82148658` | sprites.md | Native | `sub_8209F5A0`, `sub_8209F5C8` | 2 | 0 | 21 | 0 |
| `sub_82173E28` | race-world.md | Native | `__savefpr_24`, `__restfpr_24` | 2 | 0 | 26 | 2 |
| `sub_82228588` | location-screen.md | Native | `__savefpr_20`, `__restfpr_20` | 2 | 0 | 34 | 2 |
| `sub_82293998` | matrix-math.md | Native | `sub_822936D8` | 2 | 0 | 118 | 0 |
| `sub_82296270` | functions.md | Overridden | `sub_822972D0` | 2 | 2 | 12 | 0 |
| `sub_822AD298` | functions.md | Overridden | `sub_822AD190`, `sub_822B2388` | 2 | 0 | 128 | 0 |
| `sub_8236AEA8` | texture-compression.md | Lifted | `__savefpr_20`, `__restfpr_20` | 2 | 0 | 34 | 2 |
| `sub_8236B418` | texture-compression.md | Lifted | `__savefpr_19`, `__restfpr_19` | 2 | 0 | 36 | 2 |
| `sub_823F6C60` | texture-compression.md | Native | `sub_823EAB58`, `sub_823F55B0` | 2 | 2 | 197 | 0 |
| `sub_82420AB8` | texture-compression.md | Native | `__savefpr_15`, `__restfpr_15` | 2 | 0 | 44 | 2 |
| `sub_82420F50` | texture-compression.md | Native | `__savefpr_19`, `__restfpr_19` | 2 | 0 | 36 | 2 |
| `sub_82422178` | texture-compression.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82468848` | xaudio-objects.md | Native | `__savefpr_17`, `__restfpr_17` | 2 | 0 | 40 | 2 |
| `sub_82481390` | car-sounds.md | Native | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_82575B08` | system-services.md | Native | `sub_82459C10` | 2 | 0 | 67 | 1 |
| `sub_82598768` | car-scene-draw.md | Overridden | `__savefpr_18`, `__restfpr_18` | 2 | 0 | 38 | 2 |
| `sub_8259BB98` | lifted.md | Lifted | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_825A3980` | lifted.md | Lifted | `__savefpr_23`, `__restfpr_23` | 2 | 0 | 28 | 2 |
| `sub_825A3E18` | lifted.md | Lifted | `__savefpr_21`, `__restfpr_21` | 2 | 0 | 32 | 2 |
| `sub_825A6880` | heap.md | Overridden | `sub_820911E8`, `sub_825DB3F8` | 2 | 2 | 21 | 0 |
| `sub_825BF7A8` | lifted.md | Lifted | `sub_825BF8D8`, `sub_825BFA68` | 2 | 2 | 253 | 0 |
| `sub_825D3F28` | xact-waves.md | Lifted | `sub_825D3FD0`, `sub_825D41F0` | 2 | 2 | 177 | 0 |
| `sub_825D4020` | xact-waves.md | Lifted | `sub_825D3FD0`, `sub_825D41F0` | 2 | 2 | 177 | 0 |
| `sub_82090A08` | framework.md | Lifted | `sub_820902C8` | 1 | 0 | 46 | 1 |
| `sub_820998B8` | system-services.md | Lifted | `sub_82099828` | 1 | 0 | 45 | 0 |
| `sub_8209A0E8` | framework.md | Native | `sub_82091CD0` | 1 | 1 | 25 | 0 |
| `sub_8209F340` | racer-controllers.md | Native | `sub_82591C08` | 1 | 0 | 6 | 0 |
| `sub_820A48B8` | files-and-saves.md | Overridden | `sub_82295C10` | 1 | 0 | 5 | 0 |
| `sub_820A4960` | files-and-saves.md | Overridden | `sub_822952E0` | 1 | 0 | 5 | 0 |
| `sub_820A49D0` | files-and-saves.md | Overridden | `sub_82295C08` | 1 | 0 | 5 | 0 |
| `sub_820A4B18` | files-and-saves.md | Overridden | `sub_82295BC8` | 1 | 0 | 13 | 0 |
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
| `sub_820E42E8` | functions.md | Overridden | `sub_820D3740` | 1 | 0 | 114 | 0 |
| `sub_820E45A8` | functions.md | Overridden | `sub_820D3688` | 1 | 0 | 74 | 0 |
| `sub_820F77A0` | collision-world.md | Lifted | `sub_820F69B0` | 1 | 0 | 27 | 0 |
| `sub_820F9E28` | collision-world.md | Lifted | `sub_820F9050` | 1 | 0 | 166 | 0 |
| `sub_82107B90` | boot-mode.md | Overridden | `sub_825894C8` | 1 | 0 | 75 | 0 |
| `sub_82107BF8` | boot-mode.md | Overridden | `sub_825895A0` | 1 | 0 | 61 | 0 |
| `sub_82109560` | strings-and-font.md | Native | `sub_82108F68` | 1 | 1 | 53 | 0 |
| `sub_8211F300` | race-overlays.md | Overridden | `sub_8211F3B4` | 1 | 0 | 5 | 0 |
| `sub_82134410` | lifted.md | Lifted | `sub_82134698` | 1 | 1 | 729 | 0 |
| `sub_82134618` | lifted.md | Lifted | `sub_82134698` | 1 | 1 | 729 | 0 |
| `sub_82148650` | scenery-lists.md | Native | `sub_8209BD40` | 1 | 0 | 6 | 1 |
| `sub_821800B0` | type6-type7-modes.md | Native | `sub_8217D760` | 1 | 0 | 37 | 1 |
| `sub_8218C678` | race-menu.md | Overridden | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_821D8978` | tuning-screen.md | Native | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_821DF350` | power-graph.md | Overridden | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_821E3AD0` | tuning-screen.md | Native | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_821E6C50` | tuning-screen.md | Overridden | `sub_821E6F90` | 1 | 0 | 78 | 1 |
| `sub_82206A50` | tuning-pages.md | Native | `sub_82200DC0` | 1 | 0 | 418 | 1 |
| `sub_82206F48` | tuning-pages.md | Native | `sub_822012E8` | 1 | 0 | 268 | 0 |
| `sub_82207320` | tuning-pages.md | Native | `sub_82201620` | 1 | 0 | 320 | 0 |
| `sub_822129A8` | race-hud-object.md | Native | `sub_8211BA00` | 1 | 0 | 92 | 0 |
| `sub_822141E8` | race-hud.md | Overridden | `sub_82212908` | 1 | 0 | 27 | 1 |
| `sub_8227C9F0` | network-mode.md | Overridden | `sub_8214F620` | 1 | 0 | 25 | 0 |
| `sub_8227CB38` | network-mode.md | Overridden | `sub_8214F620` | 1 | 0 | 25 | 0 |
| `sub_8228F408` | scenery-lists.md | Overridden | `sub_8228F484` | 1 | 0 | 157 | 1 |
| `sub_82293988` | matrix-math.md | Overridden | `sub_82293598` | 1 | 0 | 5 | 0 |
| `sub_822977B0` | frame-driver.md | Native | `sub_82296E68` | 1 | 1 | 5 | 0 |
| `sub_82297888` | frame-driver.md | Native | `sub_82296E68` | 1 | 1 | 5 | 0 |
| `sub_82297CE0` | system-services.md | Overridden | `sub_82297C08` | 1 | 0 | 66 | 0 |
| `sub_8229E100` | command-buffer.md | Native | `sub_8229DC28` | 1 | 0 | 55 | 1 |
| `sub_8229E848` | command-buffer.md | Overridden | `sub_8229DFF8` | 1 | 1 | 48 | 0 |
| `sub_8229EDA8` | command-buffer.md | Native | `sub_822AD850` | 1 | 1 | 58 | 0 |
| `sub_822A0718` | functions.md | Lifted | `sub_822A0688` | 1 | 1 | 43 | 0 |
| `sub_822A3A20` | device-state.md | Native | `sub_822B0C38` | 1 | 0 | 92 | 0 |
| `sub_822A63F8` | graphics-resources.md | Native | `sub_8229DDD8` | 1 | 0 | 55 | 1 |
| `sub_822AC918` | functions.md | Lifted | `sub_822AC5A0` | 1 | 1 | 7 | 0 |
| `sub_822B0AB0` | functions.md | Overridden | `sub_822B0760` | 1 | 0 | 24 | 0 |
| `sub_8235A990` | models.md | Native | `sub_822A5B68` | 1 | 0 | 7 | 1 |
| `sub_82459D60` | xaudio-engine.md | Overridden | `sub_8245C638` | 1 | 0 | 9 | 0 |
| `sub_8245C650` | audio.md | Overridden | `sub_82295728` | 1 | 1 | 127 | 0 |
| `sub_8245C958` | audio.md | Overridden | `sub_8245D118` | 1 | 1 | 10 | 0 |
| `sub_82465258` | xaudio-objects.md | Native | `sub_82467A00` | 1 | 0 | 12 | 0 |
| `sub_824696F0` | xaudio-objects.md | Native | `sub_82469728` | 1 | 1 | 48 | 0 |
| `sub_8246F140` | xaudio-objects.md | Overridden | `sub_8246F270` | 1 | 1 | 39 | 0 |
| `sub_8259F700` | ui-movies.md | Native | `sub_8259EC58` | 1 | 1 | 84 | 0 |
| `sub_825A16B0` | model-animation.md | Lifted | `sub_825A1708` | 1 | 1 | 37 | 0 |
| `sub_825A5020` | lifted.md | Lifted | `sub_825A5610` | 1 | 1 | 131 | 0 |
| `sub_825A82C8` | c-runtime.md | Native | `sub_825AFD98` | 1 | 1 | 16 | 0 |
| `sub_825A8ED0` | c-runtime.md | Overridden | `sub_825A8D50` | 1 | 1 | 127 | 0 |
| `sub_825AB0C0` | c-runtime.md | Native | `sub_825AFD98` | 1 | 1 | 16 | 0 |
| `sub_825ACD90` | c-runtime-core.md | Native | `sub_825ACEB4` | 1 | 0 | 16 | 0 |
| `sub_825ACF70` | c-runtime-core.md | Overridden | `sub_825AD02C` | 1 | 0 | 21 | 0 |
| `sub_825AD280` | c-runtime-core.md | Native | `sub_825AD488` | 1 | 0 | 21 | 0 |
| `sub_825B11A8` | c-runtime-core.md | Overridden | `sub_825B1304` | 1 | 0 | 21 | 0 |
| `sub_825B4828` | files-and-saves.md | Overridden | `sub_825B4978` | 1 | 0 | 21 | 0 |
| `sub_825B65D0` | c-runtime-core.md | Overridden | `sub_825B6730` | 1 | 0 | 21 | 0 |
| `sub_825BFCC8` | lifted.md | Lifted | `sub_825BFCFC` | 1 | 1 | 44 | 0 |
| `sub_825C35C0` | lifted.md | Lifted | `sub_825C35E4` | 1 | 1 | 30 | 0 |
| `sub_825C4D90` | xact.md | Overridden | `sub_825C4DC8` | 1 | 1 | 18 | 0 |
| `sub_825C7F98` | xact.md | Overridden | `sub_825C7FE8` | 1 | 1 | 53 | 0 |
| `sub_825C8C98` | lifted.md | Lifted | `sub_825C8CD4` | 1 | 1 | 65 | 0 |
| `sub_825CD6A8` | lifted.md | Lifted | `sub_825CD6EC` | 1 | 1 | 22 | 0 |
| `sub_825CDEA8` | lifted.md | Lifted | `sub_825CE05C` | 1 | 1 | 176 | 0 |
| `sub_825CE450` | lifted.md | Lifted | `sub_825CE49C` | 1 | 1 | 25 | 0 |
| `sub_825D0F00` | xact-tracks.md | Native | `sub_825D0F4C` | 1 | 1 | 55 | 0 |
| `sub_825D3178` | xact-waves.md | Lifted | `sub_825D31D8` | 1 | 1 | 102 | 0 |
| `sub_825D3428` | xact-waves.md | Lifted | `sub_825D3470` | 1 | 1 | 32 | 0 |
| `sub_825D7AA0` | xact-tracks.md | Lifted | `sub_825D7B78` | 1 | 0 | 5 | 1 |
| `sub_825D7BA0` | xact-tracks.md | Lifted | `sub_825D7B78` | 1 | 0 | 5 | 1 |

## Ours functions whose original body is still reachable

Ours by their rows, but something reaches their recompiled body: an `__imp__` fallback in our C++ (taken when the native C++ declines: a block not in our heap, a flag of the callee's off), a guest call to a function with no override and no C++, or an unported caller of one without an override. Each with its callers in that variant (native ones marked `n`):

`sub_8211AB28` (Merged; from sub_82229238 n), `sub_8211B298` (Merged; from sub_8211DBE0 n), `sub_8211C9C0` (Merged; from sub_8211D8D8 n), `sub_8217C348` (Merged; from sub_8209D130 n), `sub_82296FE0` (Merged; from sub_82295890 n), `sub_8258BE48` (Merged; from sub_82090CC0 n), `sub_825A9A80` (Merged; from sub_8210A490 n)

## Reached past the movie library's end (82575B08-8257FFFF)

`b7-loop.md` ends the movie library's first range at 82575B07; `movies.md` at 8257FFFF. Reachable there: 151 functions (151 ours, 0 unported, 0 reached only through the replaced libraries). Entered from 33 functions outside it:

- `sub_82148868` (ours, system-services.md): `sub_82575B08` (ours).
- `sub_82148D00` (ours, lifted.md): `sub_825765F0` (ours).
- `sub_82149178` (ours, system-services.md): `sub_82576E78` (ours).
- `sub_82149430` (ours, lifted.md): `sub_825766E0` (ours).
- `sub_8214BA60` (ours, lifted.md): `sub_825767D0` (ours).
- `sub_8214E678` (ours, lifted.md): `sub_82576080` (ours).
- `sub_8214E6C8` (ours, lifted.md): `sub_82575E50` (ours).
- `sub_8214E778` (ours, lifted.md): `sub_82575FE0` (ours).
- `sub_8214E850` (ours, lifted.md): `sub_82576130` (ours).
- `sub_8214E908` (ours, lifted.md): `sub_825761D8` (ours).
- `sub_8214E9C8` (ours, lifted.md): `sub_82576280` (ours), `sub_825766E0` (ours).
- `sub_8214EB28` (ours, lifted.md): `sub_82576328` (ours), `sub_825766E0` (ours).
- `sub_8214EC80` (ours, lifted.md): `sub_825763D0` (ours).
- `sub_8214ED30` (ours, lifted.md): `sub_82576080` (ours), `sub_825764A8` (ours), `sub_82576540` (ours).
- `sub_8214F938` (ours, lifted.md): `sub_82576EE0` (ours).
- `sub_8214FDF8` (ours, system-services.md): `sub_82576EE0` (ours).
- `sub_8214FF78` (ours, lifted.md): `sub_82576EE0` (ours).
- `sub_82260908` (ours, system-services.md): `sub_82576EE0` (ours).
- `sub_822917B8` (ours, lifted.md): `sub_82575E50` (ours).
- `sub_82291858` (ours, lifted.md): `sub_825761D8` (ours).
- `sub_82291920` (ours, lifted.md): `sub_825764A8` (ours).
- `sub_822919A0` (ours, lifted.md): `sub_82576540` (ours).
- `sub_82291A40` (ours, lifted.md): `sub_82576328` (ours).
- `sub_82291AD0` (ours, lifted.md): `sub_82576080` (ours).
- `sub_822923D8` (ours, lifted.md): `sub_82576900` (ours), `sub_82576FE8` (ours).
- `sub_82292518` (ours, lifted.md): `sub_82576900` (ours), `sub_82576FE8` (ours).
- `sub_82292B28` (ours, lifted.md): `sub_825766E0` (ours).
- `sub_82292E98` (ours, lifted.md): `sub_82576080` (ours).
- `sub_82293028` (ours, lifted.md): `sub_82576BE0` (ours), `sub_82576F30` (ours).
- `sub_82293190` (ours, lifted.md): `sub_82576080` (ours).
- `sub_8245A410` (ours, sound-cues.md): `sub_82578AF0` (ours).
- `sub_82580020` (ours, lifted.md): `sub_8257FF68` (ours).
- `sub_82580028` (ours, lifted.md): `sub_8257FF68` (ours).

Each function there, with its callers: `sub_82575B08` (ours) from `sub_82148868`; `sub_82575B68` (ours) from `sub_825767D0`, `sub_82576900`, `sub_82576BE0`; `sub_82575BF0` (ours) from `sub_825767D0`, `sub_82576900`, `sub_82576BE0`; `sub_82575C90` (ours) from `sub_825767D0`, `sub_82576900`, `sub_82576BE0`; `sub_82575D78` (ours) from `sub_82576900`, `sub_82576BE0`; `sub_82575E50` (ours) from `sub_8214E6C8`, `sub_822917B8`; `sub_82575FE0` (ours) from `sub_8214E778`; `sub_82576080` (ours) from `sub_8214E678`, `sub_8214ED30`, `sub_82291AD0` ...; `sub_82576130` (ours) from `sub_8214E850`; `sub_825761D8` (ours) from `sub_8214E908`, `sub_82291858`; `sub_82576280` (ours) from `sub_8214E9C8`; `sub_82576328` (ours) from `sub_8214EB28`, `sub_82291A40`; `sub_825763D0` (ours) from `sub_8214EC80`; `sub_825764A8` (ours) from `sub_8214ED30`, `sub_82291920`; `sub_82576540` (ours) from `sub_8214ED30`, `sub_822919A0`; `sub_825765F0` (ours) from `sub_82148D00`; `sub_825766E0` (ours) from `sub_82149430`, `sub_8214E9C8`, `sub_8214EB28` ...; `sub_825767A0` (ours) from `sub_825767D0`, `sub_82576900`, `sub_82576BE0`; `sub_825767D0` (ours) from `sub_8214BA60`; `sub_82576900` (ours) from `sub_822923D8`, `sub_82292518`; `sub_82576BE0` (ours) from `sub_82293028`; `sub_82576E78` (ours) from `sub_82149178`; `sub_82576EE0` (ours) from `sub_8214F938`, `sub_8214FDF8`, `sub_8214FF78` ...; `sub_82576F30` (ours) from `sub_82293028`; `sub_82576FE8` (ours) from `sub_822923D8`, `sub_82292518`; `sub_82577088` (ours) from `sub_82577850`, `sub_825778D8`; `sub_825770B8` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82577110` (ours) from `sub_825786B8`; `sub_82577140` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82577190` (ours) from `sub_825778D8`; `sub_825774D0` (ours) from `sub_82575BF0`, `sub_82577630`; `sub_82577528` (ours) from `sub_825778D8`, `sub_825786B8`; `sub_82577560` (ours) from `sub_82575B08`; `sub_82577630` (ours) from `sub_82577788`; `sub_825776A8` (ours) from `sub_82575C90`; `sub_82577788` (ours) from `sub_82575D78`; `sub_82577850` (ours) from `sub_825778D8`; `sub_825778D8` (ours) from `sub_825780A0`; `sub_825780A0` (ours) from `sub_82575D78`; `sub_825780D0` (ours) from `sub_82577630`, `sub_825776A8`, `sub_82577788` ...; `sub_825780F0` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82578118` (ours) from `sub_82578168`, `sub_825784A8`, `sub_82578530` ...; `sub_82578168` (ours) from `sub_82578500`; `sub_82578248` (ours) from `sub_825778D8`, `sub_825787D0`; `sub_825782C0` (ours) from `sub_825778D8`, `sub_825787D0`; `sub_82578340` (ours) from `sub_825778D8`, `sub_825787D0`; `sub_825783C8` (ours) from `sub_825778D8`, `sub_825787D0`; `sub_82578470` (ours) from `sub_825778D8`; `sub_825784A8` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82578500` (ours) from `sub_825770B8`, `sub_82577140`, `sub_82577190` ...; `sub_82578530` (ours) from `sub_82577088`, `sub_825770B8`, `sub_82577110` ...; `sub_82578580` (ours) from `sub_82577190`, `sub_825778D8`; `sub_82578588` (ours) from `sub_825778D8`; `sub_82578608` (ours) from `sub_825778D8`, `sub_825786B8`; `sub_82578658` (ours) from `sub_825778D8`; `sub_825786B8` (ours) from `sub_825778D8`; `sub_825787D0` (ours) from `sub_825778D8`; `sub_825788C0` (ours) from `sub_82578AF0`; `sub_82578AF0` (ours) from `sub_8245A410`; `sub_82578C88` (ours) from ; `sub_82578CB8` (ours) from ; `sub_82578CD0` (ours) from `sub_82578D70`, `sub_82578F00`, `sub_825791B0`; `sub_82578D70` (ours) from `sub_825790F0`; `sub_82578F00` (ours) from `sub_825790F0`; `sub_825790F0` (ours) from ; `sub_82579108` (ours) from `sub_82579630`; `sub_825791B0` (ours) from ; `sub_82579280` (ours) from `sub_825796C0`; `sub_82579318` (ours) from ; `sub_82579448` (ours) from ; `sub_82579590` (ours) from `sub_8257F198`; `sub_82579630` (ours) from ; `sub_825796C0` (ours) from `sub_82579798`; `sub_82579798` (ours) from ; `sub_825797B0` (ours) from ; `sub_825797C0` (ours) from `sub_82579968`; `sub_82579830` (ours) from ; `sub_82579880` (ours) from ; `sub_82579898` (ours) from ; `sub_82579968` (ours) from ; `sub_82579A88` (ours) from ; `sub_82579BE8` (ours) from `sub_82579C48`; `sub_82579C48` (ours) from `sub_82579CD0`; `sub_82579CD0` (ours) from ; `sub_82579CF0` (ours) from ; `sub_82579D00` (ours) from ; `sub_82579D40` (ours) from ; `sub_82579D60` (ours) from ; `sub_82579D88` (ours) from ; `sub_82579DA0` (ours) from ; `sub_8257A3D0` (ours) from `sub_82579A88`; `sub_8257A3F0` (ours) from `sub_82579BE8`; `sub_8257B9A8` (ours) from `sub_82579630`, `sub_82579898`; `sub_8257B9D0` (ours) from `sub_82579108`, `sub_82579830`; `sub_8257B9F8` (ours) from ; `sub_8257BA88` (ours) from ; `sub_8257BAA0` (ours) from ; `sub_8257BAD0` (ours) from ; `sub_8257BB00` (ours) from ; `sub_8257BB28` (ours) from ; `sub_8257BB50` (ours) from ; `sub_8257BB60` (ours) from `sub_8257BBC8`, `sub_8257BE48`; `sub_8257BBC8` (ours) from ; `sub_8257BC48` (ours) from ; `sub_8257BE48` (ours) from ; `sub_8257BEC0` (ours) from ; `sub_8257C160` (ours) from `sub_82578CD0`, `sub_825797C0`, `sub_82579A88` ...; `sub_8257C2B0` (ours) from `sub_8257C3E0`; `sub_8257C3E0` (ours) from `sub_8257EDE8`; `sub_8257C570` (ours) from `sub_82578F00`, `sub_8257B9F8`, `sub_8257CCD8`; `sub_8257C590` (ours) from `sub_8257BC48`, `sub_8257BEC0`, `sub_8257C2B0`; `sub_8257C610` (ours) from `sub_8257C160`; `sub_8257C6B0` (ours) from `sub_8257C3E0`; `sub_8257C710` (ours) from `sub_82579280`, `sub_82579BE8`, `sub_8257CFA8`; `sub_8257C858` (ours) from `sub_825791B0`, `sub_82579A88`, `sub_8257D518`; `sub_8257C948` (ours) from `sub_8257C9A0`, `sub_8257D298`; `sub_8257C9A0` (ours) from `sub_8257D020`; `sub_8257CA10` (ours) from `sub_8257D298`; `sub_8257CB38` (ours) from `sub_8257D298`; `sub_8257CC08` (ours) from `sub_8257D298`; `sub_8257CCD8` (ours) from `sub_8257D298`; `sub_8257CF10` (ours) from `sub_8257D020`; `sub_8257CFA8` (ours) from `sub_82579280`, `sub_8257D518`; `sub_8257D020` (ours) from `sub_82578F00`; `sub_8257D298` (ours) from `sub_82579590`; `sub_8257D460` (ours) from `sub_82579630`; `sub_8257D518` (ours) from `sub_825791B0`; `sub_8257D6D8` (ours) from `sub_82579630`; `sub_8257D6F8` (ours) from `sub_82579108`; `sub_8257E8E8` (ours) from `sub_8257F070`, `sub_8257F1E8`; `sub_8257E9B8` (ours) from `sub_8257F070`; `sub_8257EAF0` (ours) from `sub_8257EF80`, `sub_8257EFF8`, `sub_8257F198`; `sub_8257EB68` (ours) from `sub_8257F070`; `sub_8257EDE8` (ours) from `sub_82579BE8`; `sub_8257EF80` (ours) from `sub_82579898`; `sub_8257EFF8` (ours) from `sub_82579830`; `sub_8257F070` (ours) from `sub_82579898`, `sub_82579A88`; `sub_8257F198` (ours) from `sub_82579968`; `sub_8257F1E8` (ours) from `sub_82579898`, `sub_82579A88`; `sub_8257F388` (ours) from `sub_82579A88`; `sub_8257F5E8` (ours) from `sub_8257BC48`, `sub_8257BEC0`; `sub_8257F970` (ours) from `sub_8257FA90`; `sub_8257FA90` (ours) from `sub_8257BC48`; `sub_8257FB38` (ours) from `sub_8257FBF8`, `sub_8257FC00`; `sub_8257FBF8` (ours) from `sub_8257BB00`, `sub_8257BBC8`; `sub_8257FC00` (ours) from `sub_8257FD18`; `sub_8257FD18` (ours) from `sub_8257BBC8`; `sub_8257FD48` (ours) from `sub_8257FEB8`; `sub_8257FEB8` (ours) from `sub_8257BEC0`; `sub_8257FF50` (ours) from `sub_8257BEC0`; `sub_8257FF68` (ours) from `sub_82580020`, `sub_82580028`.

## Our C++ naming replaced-library functions

Tables and vtables our C++ stores whose words land in the replaced libraries, and calls into `sub_822B8F10` (the HLSL compiler's entry, overridden by the stored shaders); not counted as the game's:

`sub_8209BE88` -> `sub_822B8F10`, `sub_8210ADD0` -> `sub_822B8F10`, `sub_8210AE60` -> `sub_824F65C0`, `sub_824600F0` -> `sub_824877D0`, `sub_824658C8` -> `sub_824877D0`, `sub_82465938` -> `sub_824877D0`, `sub_8259C6D0` -> `sub_824F65C0`, `sub_825A1438` -> `sub_822B8F10`, `sub_825D82E8` -> `sub_824892F0`, `sub_825D8660` -> `sub_824892F0`

## Replaced-library functions with direct callers in the game's code

In the excluded ranges but called directly by unported game code ("checked against its callers first"), so counted as the game's (ours or unported), with what they reach:

`sub_82481A70`, `sub_82481A80`, `sub_82481B88`, `sub_82481B98`, `sub_82482068`, `sub_82482730`, `sub_82482780`, `sub_82482968`, `sub_82482B18`, `sub_82482CA8`, `sub_82482DA8`, `sub_82483130`, `sub_82483208`, `sub_82483238`, `sub_82483388`, `sub_82483460`, `sub_82483700`, `sub_82483770`, `sub_82483DC0`, `sub_82483DD0`, `sub_82483F30`, `sub_824847D0`, `sub_82485350`, `sub_82485460`, `sub_82486710`, `sub_82486810`, `sub_82486F90`, `sub_824875A8`, `sub_824875B8`, `sub_82487660`, `sub_824877E0`, `sub_824878B0`, `sub_824878D0`, `sub_82487B28`, `sub_82487BE8`, `sub_82488F50`, `sub_82489048`, `sub_824892A8`, `sub_824892D0`, `sub_824892F8`, `sub_82489300`, `sub_824893B8`, `sub_8248CA80`, `sub_8248CB58`, `sub_8248CBD0`, `sub_8248EC58`, `sub_8248ED78`, `sub_8248F030`, `sub_8248F218`, `sub_8248F550`, `sub_82490960`, `sub_82490B08`, `sub_824936E8`, `sub_82493700`, `sub_824960B8`, `sub_824960E0`, `sub_82496148`
