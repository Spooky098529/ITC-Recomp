# B7's static map

Made by `itc_recomp/tools/static_map.py` on every run; never edit it by hand (run the tool again). What it reads, how edges are found and its gaps: the tool's docstring and "The static map" in `docs/coverage.md`.

Made 2026-11-14 at commit `c66d07b1` (`b7-loop-13-11-26`).

## Totals

- Recompiled `sub_` functions: 16552; 144 jump-table cases folded into their parents, leaving 16408.
- Reachable: 10865 (10321 from `main`, 544 more from 189 extra roots: functions seen in play that `main` doesn't reach, 1 of them not under another).
- Ours: 6034. Excluded (replaced libraries): 1599.
- **Unported: 3232** (2727 under `main`): seen in play 644, never seen 2588; 245074 generated lines.
- Of the unported, 672 hang under unported code (or a root); 2560 are reached only through ours (394 seen in play), 1220 of them called by our C++ itself (a guest call by name or address, a fallback for a branch, a table or vtable it stores) and the rest under those; the TSV's `behind_ours` column lists them.
- Ours functions' edges come from our C++ (`src_edges.py`): 27194 definitions in 1131 files of `src/` (harnesses left out), 6587 functions with C++ (an override or `ITC_NATIVE_RUNNING`), 5960 of them reachable. Ours functions without C++ (Merged, or whose print names no address) keep the original's edges where something still reaches them.
- Ours functions whose original body is still reachable: 73 (69 from our C++: an `__imp__` fallback when the native C++ declines, or a guest call to a function with neither override nor C++); listed below.
- Host code no native function's C++ reaches but that names guest functions (constants defined but not resolved to a user, a few hooks' helpers): 35 functions named, taken as roots; they add 2 reachable functions.
- Before 2026-11-14's fix the graph followed the original's edges through ours too: 15908 reachable. **Originals no longer called: 1493** (945 not ours, 548 ours whose work our C++ took in; 124542 lines, 170 seen in play): reachable through the original's graph (outside the replaced libraries), not through ours; below, by the native function that left the call out.
- Frontier (ready now): 1663 functions.
- Reachable functions calling imports: 328 (170 unported).
- Image read: 13492 code-built data addresses scanned; 181 function pointers in data that no code-built address reaches (136 functions; not edges).

A `*` after a function: never seen in play. Sizes are generated lines.

## Blockers, by what they unblock

Unported functions ranked by how many unported functions above them (through unported callers) wait on them; "frees" is how many would join the frontier if this one alone became ours.

| Function | Unblocks | Frees | Lines | Seen | Ready | File |
|---|---|---|---|---|---|---|
| `sub_825AA400` | 305 | 1 | 27 | yes | yes | unmapped |
| `sub_825AA4C0` | 304 | 2 | 68 | yes | no | unmapped |
| `sub_82294E20` | 298 | 5 | 24 | yes | yes | functions.md |
| `sub_825B68B8` | 298 | 1 | 7 | no | yes | unmapped |
| `sub_825A81E0` | 297 | 0 | 23 | no | no | unmapped |
| `sub_825AFCC8` | 296 | 0 | 27 | no | no | unmapped |
| `sub_825A8218` | 283 | 0 | 23 | no | no | unmapped |
| `sub_825B7A58` | 282 | 4 | 15 | no | yes | unmapped |
| `sub_825B3570` | 282 | 1 | 16 | yes | yes | unmapped |
| `sub_825B35A0` | 281 | 1 | 35 | yes | no | unmapped |
| `sub_825A8178` | 281 | 0 | 36 | no | yes | unmapped |
| `sub_825B7880` | 281 | 0 | 68 | no | no | unmapped |
| `sub_825A8250` | 280 | 0 | 40 | no | no | unmapped |
| `sub_825B7A20` | 279 | 0 | 20 | no | yes | unmapped |
| `sub_825B7958` | 278 | 0 | 68 | no | no | unmapped |
| `sub_825B6500` | 277 | 0 | 64 | no | no | files-and-saves.md |
| `sub_825B0E88` | 275 | 1 | 32 | no | no | unmapped |
| `sub_825B0F48` | 275 | 0 | 195 | no | no | files-and-saves.md |
| `sub_825B1304` | 275 | 0 | 21 | no | no | unmapped |
| `sub_825B11A8` | 274 | 0 | 106 | no | no | unmapped |
| `sub_825ACC78` | 243 | 1 | 53 | no | no | unmapped |
| `sub_825AD688` | 240 | 3 | 20 | no | yes | unmapped |
| `sub_825AD6C0` | 238 | 1 | 12 | no | yes | unmapped |
| `sub_825B4978` | 238 | 0 | 21 | no | no | unmapped |
| `sub_825B77E8` | 238 | 0 | 50 | no | no | unmapped |
| `sub_825BA510` | 238 | 0 | 25 | no | yes | files-and-saves.md |
| `sub_825ACEB4` | 237 | 0 | 16 | no | yes | unmapped |
| `sub_825ACF04` | 237 | 0 | 32 | no | no | unmapped |
| `sub_825AD638` | 237 | 0 | 20 | no | yes | unmapped |
| `sub_825AD670` | 237 | 0 | 12 | no | yes | unmapped |
| `sub_825B0C28` | 237 | 0 | 81 | no | no | unmapped |
| `sub_825B0E50` | 237 | 0 | 21 | no | no | unmapped |
| `sub_825B4828` | 237 | 0 | 105 | no | no | files-and-saves.md |
| `sub_82297870` | 236 | 1 | 8 | yes | yes | unmapped |
| `sub_825ACD10` | 236 | 1 | 49 | no | no | unmapped |
| `sub_825ACD90` | 236 | 1 | 96 | no | no | unmapped |
| `sub_825B0D10` | 236 | 0 | 98 | no | no | unmapped |
| `sub_825B0EE0` | 236 | 0 | 36 | no | yes | unmapped |
| `sub_825AEDA0` | 235 | 1 | 23 | no | yes | unmapped |
| `sub_825A94C0` | 235 | 0 | 65 | no | no | unmapped |

## Frontier clusters, by what they unblock

582 clusters (frontier functions sharing an unported caller within 0x4000 bytes, neighbors in address order, or a call cycle). Each: what it unblocks, its functions with their sizes, its function files.

1. **sub_825AA3F8**-825AA5C0: unblocks 308; 3 functions, 98 lines, 3 seen; unmapped. `sub_825AA3F8` 5, `sub_825AA400` 27, `sub_825AA5C0` 66
2. **sub_82294E20**-82295700: unblocks 301; 3 functions, 87 lines, 1 seen; functions.md, system-services.md. `sub_82294E20` 24, `sub_82294E58`* 48, `sub_82295700`* 15
3. **sub_825B68B8**-825B68B8: unblocks 298; 1 functions, 7 lines, 0 seen; unmapped. `sub_825B68B8`* 7
4. **sub_825B7A20**-825B7A58: unblocks 283; 2 functions, 35 lines, 0 seen; unmapped. `sub_825B7A20`* 20, `sub_825B7A58`* 15
5. **sub_825B3570**-825B3570: unblocks 282; 1 functions, 16 lines, 1 seen; unmapped. `sub_825B3570` 16
6. **sub_825A8178**-825A8178: unblocks 281; 1 functions, 36 lines, 0 seen; unmapped. `sub_825A8178`* 36
7. **sub_825ACEB4**-825AD6C0: unblocks 244; 5 functions, 80 lines, 0 seen; unmapped. `sub_825ACEB4`* 16, `sub_825AD638`* 20, `sub_825AD670`* 12, `sub_825AD688`* 20, `sub_825AD6C0`* 12
8. **sub_82297870**-82297B98: unblocks 238; 5 functions, 138 lines, 3 seen; unmapped. `sub_82297870` 8, `sub_82297940`* 48, `sub_822979D0`* 6, `sub_82297B38` 37, `sub_82297B98` 39
9. **sub_825BA510**-825BA510: unblocks 238; 1 functions, 25 lines, 0 seen; files-and-saves.md. `sub_825BA510`* 25
10. **sub_825B0EE0**-825B0EE0: unblocks 236; 1 functions, 36 lines, 0 seen; unmapped. `sub_825B0EE0`* 36
11. **sub_825AD120**-825AEDA0: unblocks 235; 2 functions, 34 lines, 0 seen; unmapped. `sub_825AD120`* 11, `sub_825AEDA0`* 23
12. **sub_825B4F38**-825B4F38: unblocks 234; 1 functions, 16 lines, 0 seen; unmapped. `sub_825B4F38`* 16
13. **sub_825AA1B8**-825AA338: unblocks 232; 2 functions, 56 lines, 0 seen; c-runtime.md. `sub_825AA1B8`* 32, `sub_825AA338`* 24
14. **sub_825B4C8C**-825B4C8C: unblocks 229; 1 functions, 24 lines, 0 seen; unmapped. `sub_825B4C8C`* 24
15. **sub_825B49B0**-825B49B0: unblocks 228; 1 functions, 6 lines, 0 seen; unmapped. `sub_825B49B0`* 6
16. **sub_82090380**-82094A70: unblocks 134; 19 functions, 325 lines, 4 seen; frame-driver.md, single-commands.md. `sub_82090380`* 28, `sub_820903D0`* 28, `sub_820911F8`* 5, `sub_82091648`* 12, `sub_82091910`* 30, `sub_82092A70`* 28, `sub_82092AC0`* 28, `sub_82094380` 9, `sub_82094398`* 9, `sub_820943E0` 8, `sub_820943F8` 8, `sub_82094410`* 5, ... 7 more
17. **sub_821AA500**-821AFA08: unblocks 119; 61 functions, 5075 lines, 0 seen; car-settings.md. `sub_821AA500`* 15, `sub_821AA530`* 16, `sub_821AA560`* 182, `sub_821AA7B8`* 134, `sub_821AA970`* 103, `sub_821AAAB8`* 130, `sub_821AAC60`* 171, `sub_821AAE90`* 40, `sub_821AAF08`* 35, `sub_821AB038`* 161, `sub_821AB250`* 33, `sub_821AB2C0`* 182, ... 49 more
18. **sub_825BFEA0**-825C6918: unblocks 108; 23 functions, 1050 lines, 6 seen; xact.md. `sub_825BFEA0` 65, `sub_825C05C8`* 70, `sub_825C0698`* 36, `sub_825C1548`* 11, `sub_825C1568` 40, `sub_825C1738`* 23, `sub_825C1788`* 43, `sub_825C38A8` 108, `sub_825C39F8`* 117, `sub_825C4940`* 89, `sub_825C4A58`* 12, `sub_825C4A78`* 12, ... 11 more
19. **sub_825B4790**-825B4790: unblocks 102; 1 functions, 44 lines, 0 seen; unmapped. `sub_825B4790`* 44
20. **sub_825C07F0**-825C1090: unblocks 73; 4 functions, 190 lines, 1 seen; xact.md. `sub_825C07F0`* 30, `sub_825C0848`* 43, `sub_825C0958`* 28, `sub_825C1090` 89
21. **sub_822AD790**-822AF3D0: unblocks 69; 11 functions, 1102 lines, 7 seen; functions.md, texture-compression.md. `sub_822AD790`* 63, `sub_822ADA28` 37, `sub_822ADA98` 55, `sub_822ADB50` 71, `sub_822ADC08` 205, `sub_822AE340` 439, `sub_822AE918`* 62, `sub_822AE9E8`* 61, `sub_822AF2D8` 23, `sub_822AF328`* 49, `sub_822AF3D0` 37
22. **sub_82094CD0**-8258E578: unblocks 66; 17 functions, 623 lines, 5 seen; video-output.md. `sub_82094CD0`* 29, `sub_8258BE58`* 121, `sub_8258BFA0`* 66, `sub_8258C060`* 30, `sub_8258D030`* 33, `sub_8258D090` 25, `sub_8258D0D8` 50, `sub_8258D170` 37, `sub_8258D1E0`* 35, `sub_8258D248`* 21, `sub_8258D290`* 45, `sub_8258D318`* 47, ... 5 more
23. **sub_823EA0C0**-823EB408: unblocks 65; 5 functions, 1210 lines, 0 seen; texture-compression.md. `sub_823EA0C0`* 609, `sub_823EADB0`* 59, `sub_823EAE40`* 269, `sub_823EB180`* 215, `sub_823EB408`* 58
24. **sub_825CA860**-825CD388: unblocks 59; 24 functions, 534 lines, 2 seen; xact.md. `sub_825CA860` 29, `sub_825CAC58`* 33, `sub_825CACB8`* 37, `sub_825CAD18`* 53, `sub_825CADC0`* 15, `sub_825CADF0`* 10, `sub_825CAE08`* 44, `sub_825CAFD8` 34, `sub_825CB660`* 18, `sub_825CB6A0`* 31, `sub_825CC0E0`* 40, `sub_825CC8F0`* 6, ... 12 more
25. **sub_82583FD0**-825892B8: unblocks 58; 45 functions, 3190 lines, 5 seen; unmapped. `sub_82583FD0` 61, `sub_82584080`* 28, `sub_825840D0` 63, `sub_82584178`* 38, `sub_825841E8`* 55, `sub_82584970`* 9, `sub_82584988` 44, `sub_82584A00` 38, `sub_82584A70`* 83, `sub_82584B90`* 68, `sub_82584E48`* 24, `sub_82584E78`* 24, ... 33 more
26. **sub_825DADF0**-825DB028: unblocks 54; 60 functions, 273 lines, 0 seen; unmapped. `sub_825DADF0`* 4, `sub_825DADF8`* 4, `sub_825DAE00`* 4, `sub_825DAE08`* 4, `sub_825DAE10`* 4, `sub_825DAE18`* 4, `sub_825DAE20`* 4, `sub_825DAE28`* 34, `sub_825DAE88`* 4, `sub_825DAE90`* 4, `sub_825DAE98`* 4, `sub_825DAEA0`* 4, ... 48 more
27. **sub_825D8F18**-825D8F18: unblocks 44; 1 functions, 31 lines, 0 seen; unmapped. `sub_825D8F18`* 31
28. **sub_825CA6F8**-825CA6F8: unblocks 41; 1 functions, 88 lines, 1 seen; xact.md. `sub_825CA6F8` 88
29. **sub_82579D00**-8257FF50: unblocks 39; 16 functions, 745 lines, 4 seen; app-start-up.md, system-services.md. `sub_82579D00` 10, `sub_82579D18`* 26, `sub_82579D40` 12, `sub_82579D60`* 14, `sub_82579D88`* 10, `sub_8257A3D0` 11, `sub_8257A3F0`* 51, `sub_8257A498`* 26, `sub_8257C570`* 13, `sub_8257C590`* 40, `sub_8257C610` 53, `sub_8257C6B0`* 34, ... 4 more
30. **sub_825CEE50**-825D5258: unblocks 37; 17 functions, 677 lines, 0 seen; xact-waves.md. `sub_825CEE50`* 6, `sub_825CEE60`* 6, `sub_825CEEE8`* 32, `sub_825CF220`* 33, `sub_825CF4D8`* 26, `sub_825CF528`* 35, `sub_825CFAE0`* 24, `sub_825D1C50`* 25, `sub_825D24B0`* 17, `sub_825D4128`* 61, `sub_825D4300`* 80, `sub_825D43F0`* 16, ... 5 more
31. **sub_8245EF28**-8245EF28: unblocks 36; 1 functions, 30 lines, 0 seen; xaudio-engine.md. `sub_8245EF28`* 30
32. **sub_825C1E08**-825C1E08: unblocks 36; 1 functions, 90 lines, 1 seen; xact.md. `sub_825C1E08` 90
33. **sub_8259CB48**-8259D1C0: unblocks 35; 13 functions, 271 lines, 11 seen; sound-cues.md. `sub_8259CB48` 49, `sub_8259CBE8` 27, `sub_8259CC38` 11, `sub_8259CC58` 12, `sub_8259CCA0` 30, `sub_8259CCF8` 30, `sub_8259D0C8` 20, `sub_8259D100` 15, `sub_8259D120` 12, `sub_8259D140` 12, `sub_8259D160`* 29, `sub_8259D1A8` 9, ... 1 more
34. **sub_82587F98**-825881E0: unblocks 35; 3 functions, 499 lines, 0 seen; unmapped. `sub_82587F98`* 107, `sub_825880B8`* 108, `sub_825881E0`* 284
35. **sub_825D5738**-825D5758: unblocks 35; 2 functions, 24 lines, 0 seen; xact-tracks.md. `sub_825D5738`* 11, `sub_825D5758`* 13
36. **sub_82576080**-825767A0: unblocks 33; 8 functions, 492 lines, 0 seen; system-services.md. `sub_82576080`* 63, `sub_82576328`* 63, `sub_825763D0`* 77, `sub_825764A8`* 60, `sub_82576540`* 63, `sub_825765F0`* 79, `sub_825766E0`* 72, `sub_825767A0`* 15
37. **sub_824936E8**-824936E8: unblocks 33; 1 functions, 5 lines, 1 seen; files-and-saves.md. `sub_824936E8` 5
38. **sub_82461F10**-8246D748: unblocks 32; 58 functions, 2122 lines, 4 seen; audio.md, xaudio-objects.md. `sub_82461F10`* 7, `sub_82461F20`* 178, `sub_82465418`* 7, `sub_82465850`* 9, `sub_82465868`* 5, `sub_82466990`* 73, `sub_82466A80`* 81, `sub_82466C48`* 7, `sub_82466C58`* 5, `sub_82466C60`* 28, `sub_82466CB0`* 27, `sub_82466D00`* 28, ... 46 more
39. **sub_825A6D20**-825A6D30: unblocks 32; 3 functions, 51 lines, 2 seen; video-output.md. `sub_825A6D20` 5, `sub_825A6D28` 5, `sub_825A6D30`* 41
40. **sub_82297D38**-82297D38: unblocks 32; 1 functions, 12 lines, 1 seen; video-output.md. `sub_82297D38` 12

## Unmapped areas

Clusters of unported reachable functions with no rows in any function file: 177 (542 functions, 44924 lines). Largest first; the callers named are outside the cluster.

- **sub_820A70A8**-820A8B38: 74 functions, 2549 lines, 10 seen, unblocks 4; called from `sub_82090910` (unmapped, unported), `sub_820A75E8` (cameras.md, ours), `sub_820A8B88` (cameras.md, ours), `sub_82593E30` (movies.md, unported) ....
- **sub_82478FB8**-8247E0B0: 36 functions, 6609 lines, 0 seen, unblocks 5; called from `sub_8246F140` (xaudio-objects.md, ours), `sub_8247F7A8` (unmapped, unported), `sub_8247FB28` (unmapped, unported), `sub_8247FE18` (unmapped, unported) ....
- **sub_82577088**-82578AF0: 33 functions, 2212 lines, 2 seen, unblocks 8; called from `sub_8245A410` (sound-cues.md, ours), `sub_82575BF0` (unmapped, unported), `sub_82575C90` (system-services.md, unported), `sub_82575D78` (system-services.md, unported).
- **sub_820B95F8**-820BE0D8: 24 functions, 1995 lines, 0 seen, unblocks 18; called from `sub_820A75E8` (cameras.md, ours), `sub_820B0E70` (car-physics.md, ours), `sub_820B5670` (car-physics.md, ours), `sub_820B5B08` (car-physics.md, ours) ....
- **sub_82474578**-82478D30: 19 functions, 5833 lines, 0 seen, unblocks 14; called from `sub_8246F140` (xaudio-objects.md, ours), `sub_8247E338` (unmapped, unported), `sub_8247E410` (unmapped, unported), `sub_8247E4E8` (unmapped, unported) ....
- **sub_8246F300**-824739B0: 18 functions, 6011 lines, 0 seen, unblocks 12; called from `sub_8246F140` (xaudio-objects.md, ours), `sub_8247DE28` (unmapped, unported), `sub_8247DF00` (unmapped, unported), `sub_8247DFD8` (unmapped, unported) ....
- **sub_820ACCF0**-820ADB80: 17 functions, 521 lines, 0 seen, unblocks 3; called from `sub_820AD118` (app-states.md, ours), `sub_820ADBD8` (app-states.md, ours), `sub_820AE198` (unmapped, unported), `sub_821067F8` (game-modes.md, ours).
- **sub_82133B10**-82138520: 14 functions, 1452 lines, 0 seen, unblocks 1; called from `sub_820962B8` (frame-passes.md, ours), `sub_82135008` (post-effects.md, ours), `sub_821351A8` (post-effects.md, ours), `sub_82135AA8` (post-effects.md, ours) ....
- **sub_8247E338**-8247F320: 14 functions, 1475 lines, 0 seen, unblocks 0; called from `sub_8246F140` (xaudio-objects.md, ours).
- **sub_82136F50**-82136FA0: 11 functions, 88 lines, 0 seen, unblocks 0; called from `sub_82136E30` (post-effects.md, ours), `sub_82138610` (post-effects.md, ours).
- **sub_820AA010**-820AA198: 9 functions, 145 lines, 0 seen, unblocks 0; called from `sub_820A9E28` (cameras.md, ours).
- **sub_82489048**-8248ED78: 8 functions, 185 lines, 0 seen, unblocks 23; called from `sub_82482780` (unmapped, unported), `sub_82482968` (unmapped, unported), `sub_824831B0` (unmapped, excluded), `sub_82483238` (unmapped, unported) ....
- **sub_82297870**-82297B98: 7 functions, 250 lines, 5 seen, unblocks 237; called from `xstart` (unmapped, unported), `sub_825AA708` (unmapped, unported), `sub_825AD070` (unmapped, unported), `sub_825AFD10` (unmapped, unported) ....
- **sub_82487660**-82488F50: 7 functions, 143 lines, 0 seen, unblocks 5; called from `sub_82482068` (unmapped, unported), `sub_824820C0` (unmapped, excluded), `sub_8248F2B8` (unmapped, excluded).
- **sub_8258AB18**-8258B368: 6 functions, 850 lines, 0 seen, unblocks 4; called from `sub_820EF468` (accessories.md, unported), `sub_8258BCD8` (unmapped, unported).
- **sub_821497A8**-8214BF20: 5 functions, 453 lines, 0 seen, unblocks 6; called from `sub_8214C828` (unmapped, unported).
- **sub_8247F7A8**-82480368: 5 functions, 1265 lines, 0 seen, unblocks 0; called from `sub_8246F140` (xaudio-objects.md, ours).
- **sub_82481B88**-82482780: 5 functions, 1028 lines, 0 seen, unblocks 6; called from `sub_824820C0` (unmapped, excluded), `sub_82482968` (unmapped, unported), `sub_82483DD0` (unmapped, unported), `sub_82593E78` (movies.md, unported).
- **sub_82591898**-825920A8: 5 functions, 104 lines, 0 seen, unblocks 7; called from `sub_82098F30` (unmapped, unported), `sub_820A7720` (unmapped, unported), `sub_82591B38` (input.md, ours), `sub_82591B68` (input.md, ours).
- **sub_8259C6F0**-8259C910: 5 functions, 133 lines, 0 seen, unblocks 6; called from `sub_8210AE60` (action-mappers.md, ours), `sub_8210AEA0` (unmapped, unported), `sub_8210B090` (unmapped, unported), `sub_8259C6D0` (action-mappers.md, ours) ....
- **sub_820A6420**-820A9CF8: 4 functions, 404 lines, 0 seen, unblocks 13; called from `sub_820A7218` (unmapped, unported), `sub_820A7C50` (unmapped, unported), `sub_820A9E28` (cameras.md, ours), `sub_820AA088` (unmapped, unported) ....
- **sub_820BE4F0**-820BEBB0: 4 functions, 604 lines, 0 seen, unblocks 0; called from `sub_820BCE80` (collision.md, ours).
- **sub_820F6C88**-820F7EE8: 4 functions, 410 lines, 0 seen, unblocks 0; called from `sub_820F8090` (collision-world.md, ours).
- **sub_8259A2B0**-8259B660: 4 functions, 387 lines, 0 seen, unblocks 6; called from `sub_820F0120` (accessories.md, unported), `sub_8259A140` (model-animation.md, ours), `sub_8259A9D0` (model-animation.md, ours), `sub_8259BB98` (unmapped, unported).
- **sub_825DB428**-825DB498: 4 functions, 49 lines, 0 seen, unblocks 6; called from `sub_825DABC8` (unmapped, unported), `sub_825DAC00` (unmapped, unported), `sub_825DAC10` (unmapped, unported), `sub_825DAC48` (unmapped, unported).
- **sub_82098F30**-82099048: 3 functions, 152 lines, 0 seen, unblocks 0; called from `sub_820991E0` (input.md, ours).
- **sub_820A44C0**-820A5660: 3 functions, 115 lines, 0 seen, unblocks 0; called from `sub_820A56E8` (save-interface.md, ours), `sub_820A6160` (save-interface.md, ours).
- **sub_820AE198**-820AE2F8: 3 functions, 60 lines, 0 seen, unblocks 0; called from `sub_820AE348` (app-states.md, ours).
- **sub_820B0098**-820B00B8: 3 functions, 18 lines, 0 seen, unblocks 0; called from `sub_820BFD30` (vehicle.md, ours).
- **sub_820B0340**-820B0428: 3 functions, 126 lines, 0 seen, unblocks 0; called from `sub_820B2100` (car-physics.md, ours), `sub_820BFD30` (vehicle.md, ours).
- **sub_820FF5A0**-820FF680: 3 functions, 119 lines, 0 seen, unblocks 6; called from `sub_820FF790` (race-states.md, unported), `sub_82214C88` (race-hud-object.md, unported).
- **sub_82149A30**-82149B08: 3 functions, 105 lines, 1 seen, unblocks 31; called from `sub_82149C48` (unmapped, unported), `sub_82149D00` (unmapped, unported), `sub_8214B188` (unmapped, unported).
- **sub_822A8418**-822A8518: 3 functions, 77 lines, 0 seen, unblocks 3; called from `sub_8258F0F0` (single-commands.md, ours), `sub_8258FD48` (functions.md, unported).
- **sub_825B31D8**-825B3650: 3 functions, 120 lines, 2 seen, unblocks 3; called from `sub_825AA448` (unmapped, unported), `sub_825AA708` (unmapped, unported).
- **sub_825B4C8C**-825B4D90: 3 functions, 92 lines, 0 seen, unblocks 230; called from `sub_825A9BB8` (c-runtime.md, ours), `sub_825AA398` (c-runtime.md, ours), `sub_825B49C8` (unmapped, unported), `sub_825DAC58` (unmapped, unported).
- **sub_820BB410**-820BB450: 2 functions, 155 lines, 0 seen, unblocks 0; called from `sub_820B0E70` (car-physics.md, ours), `sub_820B5670` (car-physics.md, ours), `sub_820B5B08` (car-physics.md, ours), `sub_820B7998` (car-physics.md, ours) ....
- **sub_820BC560**-820BC828: 2 functions, 266 lines, 0 seen, unblocks 0; called from `sub_820B8D30` (car-physics.md, ours).
- **sub_820BFBA8**-820BFBB0: 2 functions, 12 lines, 0 seen, unblocks 0; called from `sub_820BCDD0` (collision.md, ours).
- **sub_820F1098**-820F1100: 2 functions, 54 lines, 0 seen, unblocks 0; called from `sub_820F2638` (course-scenery.md, ours).
- **sub_82108A08**-82108A70: 2 functions, 98 lines, 0 seen, unblocks 15; called from `sub_82109400` (unmapped, unported), `sub_8210A990` (strings-and-font.md, ours), `sub_8210ABC0` (strings-and-font.md, ours), `sub_8211C2C8` (ui-movies-game.md, unported).
- ... and 137 smaller ones (160 functions).

## Extra roots

Seen in play but not reachable from `main` (static edges miss them, or they run outside `main`: start-up, threads, callbacks), not under another extra root:

`xstart` (unported)

## Originals no longer called

Reachable through the original's graph but not through ours: the native function's C++ left the call out (plugged-in data, a dropped call, work merged into the C++). Grouped by that native function: its original's callees no longer reached, then how many functions hang only under them (a function under several groups counts in each; "not ours" leaves out those ours already, mostly Merged). Candidates for Dropped or "Not called" rows, for the user's review; the TSV's `lost` column lists them all.

| Native caller | File | Status | Lost callees | Under them | Not ours | Lines | Seen |
|---|---|---|---|---|---|---|---|
| `sub_821077A8` | game-modes.md | Overridden | `sub_82106F88`, `sub_82107670` | 810 | 476 | 77363 | 108 |
| `sub_822ACB90` | device-state.md | Native | `sub_822A1280`, `sub_822A12B0`, `sub_822A12F0`, `sub_822A1380`, `sub_822A1410`, `sub_822A14A0`, `sub_822A1530`, `sub_822A15A0`, ... 90 more | 98 | 98 | 739 | 0 |
| `sub_8214E268` | system-services.md | Overridden | `sub_82149080`, `sub_8214AC68`, `sub_8214ADC0`, `sub_8214AEA8`, `sub_8214D7A0`, `sub_8214F160`, `sub_8214F290`, `sub_8214F810`, ... 5 more | 85 | 77 | 7197 | 0 |
| `sub_8209EEC8` | mode-tick.md | Overridden | `sub_8209E4F0`, `sub_8209E540`, `sub_8209E610`, `sub_8209E698`, `sub_8209E730`, `sub_8209E790`, `sub_8209E860`, `sub_8209E9A8`, ... 3 more | 83 | 81 | 7036 | 0 |
| `sub_825D0F00` | xact-tracks.md | Native | `sub_825D0F4C`, `sub_825D1A80`, `sub_825D5F40`, `sub_825D5FE8`, `sub_825D6308`, `sub_825D67E8`, `sub_825D6CF0`, `sub_825D70C8`, ... 3 more | 68 | 68 | 3487 | 0 |
| `sub_8216DD88` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_8221F1E8` | 55 | 16 | 4204 | 5 |
| `sub_8235AAC0` | texture-compression.md | Native | `sub_82365D88`, `sub_82366458`, `sub_82368170`, `sub_8236D380` | 55 | 53 | 5481 | 0 |
| `sub_8221BD20` | race-screens.md | Native | `sub_8221A4A8`, `sub_8221A610`, `sub_8221A680`, `sub_8221A6C8`, `sub_8221A720`, `sub_8221B298`, `sub_8221B860`, `sub_8221B8A8` | 28 | 15 | 1869 | 10 |
| `sub_8220F638` | race-screens.md | Native | `sub_8220C578`, `sub_8220C5D0`, `sub_8220D898`, `sub_8220DB98`, `sub_8220E988`, `sub_8220EBD8` | 26 | 10 | 2289 | 15 |
| `sub_825CB470` | xact.md | Overridden | `sub_825C07C8`, `sub_825C95F8`, `sub_825C9A80`, `sub_825CABC8` | 25 | 25 | 1467 | 0 |
| `sub_825D06B8` | xact-tracks.md | Overridden | `sub_825C07C8`, `sub_825C95F8`, `sub_825C9788`, `sub_825CB3D8` | 24 | 24 | 1421 | 0 |
| `sub_8216DB70` | race-hud-machine.md | Overridden | `sub_82211458` | 22 | 10 | 1721 | 5 |
| `sub_825C2910` | xact.md | Overridden | `sub_825C95F8`, `sub_825C9878` | 22 | 22 | 1352 | 0 |
| `sub_8216C6D8` | race-hud-machine.md | Overridden | `sub_8221EF60` | 20 | 0 | 1703 | 0 |
| `sub_821159B0` | career.md | Overridden | `sub_82110CA8`, `sub_82111140`, `sub_821115C8`, `sub_82112260`, `sub_82115550`, `sub_82115708` | 19 | 19 | 2616 | 0 |
| `sub_822900E0` | save-interface.md | Native | `sub_825A9EC0`, `sub_825AA058` | 17 | 15 | 2961 | 0 |
| `sub_8216CB98` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_82216A40` | 14 | 14 | 1029 | 0 |
| `sub_8216D918` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_82216A40` | 14 | 14 | 1029 | 0 |
| `sub_825A8A50` | c-runtime.md | Overridden | `sub_825AFEC0`, `sub_825AFEF8`, `sub_825B0860`, `sub_825B0968`, `sub_825B0B80` | 14 | 11 | 974 | 0 |
| `sub_825AAD20` | c-runtime.md | Overridden | `sub_825AB1C8`, `sub_825AFEF8`, `sub_825B08E8`, `sub_825B0A40`, `sub_825B0B80` | 14 | 10 | 976 | 0 |
| `sub_8259C3A8` | models.md | Native | `sub_8259B748`, `sub_8259BE40` | 11 | 11 | 1838 | 0 |
| `sub_8216DFD0` | race-hud-machine.md | Overridden | `sub_82216850`, `sub_8221F930`, `sub_8221FFF0` | 10 | 6 | 614 | 0 |
| `sub_825B13D0` | c-runtime.md | Native | `sub_825AC108`, `sub_825B1368`, `sub_825B6898`, `sub_825B7EB8`, `sub_825BA4A0` | 10 | 6 | 723 | 0 |
| `sub_8216D6D0` | race-hud-machine.md | Overridden | `sub_8221F7D0`, `sub_822200E0` | 8 | 0 | 777 | 0 |
| `sub_825AF130` | c-runtime.md | Native | `sub_825AB010`, `sub_825AC108`, `sub_825AF000`, `sub_825AF098`, `sub_825B6890`, `sub_825B6898` | 8 | 3 | 316 | 2 |
| `sub_820990F0` | input.md | Overridden | `sub_82591918`, `sub_82591C18` | 7 | 2 | 617 | 0 |
| `sub_822A08A0` | functions.md | Overridden | `sub_822A05A8`, `sub_822B1F68`, `sub_822B20D8`, `sub_822B2388`, `sub_822B3FE0`, `sub_822B5C58` | 7 | 2 | 758 | 0 |
| `sub_8211E500` | ui-movies-game.md | Overridden | `sub_8211A6C0`, `sub_8259D5C8`, `sub_825A0CD0`, `sub_825A0CF8`, `sub_825A0EB0`, `sub_825A0EF0` | 6 | 0 | 245 | 0 |
| `sub_8216C210` | race-hud-machine.md | Overridden | `sub_82216850` | 6 | 6 | 366 | 0 |
| `sub_8216C530` | race-hud-machine.md | Overridden | `sub_82216850` | 6 | 6 | 366 | 0 |
| `sub_822968B8` | functions.md | Overridden | `sub_82297678` | 6 | 0 | 613 | 0 |
| `sub_820CDC18` | race-setup.md | Native | `sub_820CC450`, `sub_820CC458`, `sub_820CC460`, `sub_820CC468`, `sub_820CC478` | 5 | 1 | 37 | 4 |
| `sub_8221A090` | race-screens.md | Native | `sub_822191D0`, `sub_82219A78`, `sub_82219B60`, `sub_82219C30` | 4 | 2 | 249 | 2 |
| `sub_82299D60` | heap.md | Overridden | `sub_82299208`, `sub_8229A5F4` | 4 | 4 | 704 | 4 |
| `sub_8229A930` | heap.md | Overridden | `sub_82298A88`, `sub_8229B16C` | 4 | 4 | 938 | 4 |
| `sub_82593088` | std-string.md | Overridden | `sub_82592E18` | 4 | 3 | 622 | 0 |
| `sub_825A6880` | heap.md | Overridden | `sub_820911E8`, `sub_825A67A8`, `sub_825AD150`, `sub_825DB3F8` | 4 | 4 | 99 | 0 |
| `sub_82091FF0` | frame-driver.md | Overridden | `sub_82091F58` | 3 | 3 | 121 | 0 |
| `sub_82093BE0` | app-states.md | Overridden | `sub_82091F58` | 3 | 3 | 121 | 0 |
| `sub_8209A458` | framework.md | Overridden | `sub_82091F58` | 3 | 3 | 121 | 0 |
| `sub_820A6F60` | std-string.md | Overridden | `sub_820A6E60`, `sub_825A6A20`, `sub_825A7270` | 3 | 2 | 192 | 0 |
| `sub_820EF168` | car-models.md | Native | `sub_820EF0B8` | 3 | 3 | 174 | 0 |
| `sub_8211C680` | build-screen.md | Overridden | `sub_8211B720`, `sub_8211B770` | 3 | 0 | 85 | 0 |
| `sub_8211CD90` | ui-movies-game.md | Overridden | `sub_8211B720`, `sub_8211B770` | 3 | 0 | 85 | 0 |
| `sub_8214E190` | system-services.md | Overridden | `sub_82148958`, `sub_82459D40`, `sub_82459D50` | 3 | 0 | 55 | 0 |
| `sub_8216D450` | race-hud-machine.md | Overridden | `sub_822157E8`, `sub_82215870`, `sub_822158E8` | 3 | 3 | 145 | 0 |
| `sub_8216D600` | race-hud-machine.md | Overridden | `sub_8216C798`, `sub_82211390` | 3 | 1 | 142 | 0 |
| `sub_8216DA40` | race-hud-machine.md | Overridden | `sub_8216C798`, `sub_82211390` | 3 | 1 | 142 | 0 |
| `sub_82229238` | location-screen.md | Overridden | `sub_8211AB28`, `sub_8211BE98` | 3 | 0 | 69 | 0 |
| `sub_8229A648` | heap.md | Overridden | `sub_82298548`, `sub_82299448`, `sub_8229A8DC` | 3 | 3 | 581 | 3 |
| `sub_822A6210` | graphics-resources.md | Overridden | `sub_822AD9D8` | 3 | 3 | 86 | 0 |
| `sub_822B0AB0` | functions.md | Overridden | `sub_822B0760`, `sub_822B07B0`, `sub_822B0860` | 3 | 0 | 166 | 0 |
| `sub_8259F348` | ui-movies.md | Overridden | `sub_8211A948`, `sub_8259D618`, `sub_8259D628` | 3 | 0 | 83 | 0 |
| `sub_825A1A38` | model-animation.md | Overridden | `sub_825A1938`, `sub_825A6608` | 3 | 3 | 123 | 0 |
| `sub_825A6F60` | c-runtime.md | Native | `sub_825A7044`, `sub_825AA1A8` | 3 | 0 | 27 | 3 |
| `sub_82091720` | std-string.md | Overridden | `sub_82091398`, `sub_825A7270` | 2 | 1 | 142 | 0 |
| `sub_820988C8` | frame-driver.md | Overridden | `sub_8258C998`, `sub_8258C9C0` | 2 | 0 | 20 | 0 |
| `sub_8209D130` | mode-tick.md | Overridden | `sub_8217C348` | 2 | 1 | 281 | 1 |
| `sub_8209E118` | files-and-saves.md | Overridden | `sub_8209DD00`, `sub_822954C8` | 2 | 1 | 63 | 0 |
| `sub_820A4610` | system-services.md | Overridden | `sub_820A45C0` | 2 | 0 | 33 | 0 |
| `sub_820A4C58` | files-and-saves.md | Overridden | `sub_82295BC8`, `sub_82295C10` | 2 | 0 | 18 | 0 |
| `sub_820B2808` | car-physics.md | Native | `sub_820B6938`, `sub_820B6940` | 2 | 2 | 11 | 0 |
| `sub_820BD508` | collision.md | Native | `sub_820BACB8`, `sub_820BCF28` | 2 | 1 | 162 | 0 |
| `sub_820BD6E0` | collision.md | Native | `sub_820BACB8`, `sub_820BCF28` | 2 | 1 | 162 | 0 |
| `sub_820D3E98` | scene-loaders.md | Native | `__savevmx_123`, `__restvmx_123` | 2 | 0 | 40 | 2 |
| `sub_820D9F50` | second-set.md | Native | `__savevmx_123`, `__restvmx_123` | 2 | 0 | 40 | 2 |
| `sub_82106158` | game-modes.md | Overridden | `sub_82105FE0`, `sub_82106100` | 2 | 0 | 57 | 2 |
| `sub_82109EF0` | strings-and-font.md | Overridden | `sub_82109D80` | 2 | 2 | 330 | 0 |
| `sub_8210A680` | strings-and-font.md | Native | `sub_825A7EF8`, `sub_825A7F30` | 2 | 0 | 32 | 0 |
| `sub_8211B800` | ui-movies-game.md | Native | `sub_8211B310`, `sub_8211B720` | 2 | 0 | 59 | 0 |
| `sub_8211BEE8` | ui-movies-game.md | Overridden | `sub_8211A948`, `sub_8259D628` | 2 | 0 | 76 | 0 |
| `sub_82131DD8` | screen-objects.md | Native | `sub_8212F030`, `sub_821318B8` | 2 | 2 | 358 | 0 |
| `sub_82132BD8` | screen-objects.md | Native | `sub_8212EFA8`, `sub_82132318` | 2 | 2 | 361 | 0 |
| `sub_82147D88` | scenery-lists.md | Native | `sub_82146BD0` | 2 | 1 | 1052 | 1 |
| `sub_82148658` | sprites.md | Native | `sub_8209F5A0`, `sub_8209F5C8` | 2 | 0 | 21 | 0 |
| `sub_821D8978` | tuning-screen.md | Native | `sub_8211A630`, `sub_8211BA00` | 2 | 0 | 107 | 0 |
| `sub_821E3AD0` | tuning-screen.md | Native | `sub_8211A630`, `sub_8211BA00` | 2 | 0 | 107 | 0 |
| `sub_82211AB0` | camera-modes.md | Native | `sub_8211AB90`, `sub_8211BE98` | 2 | 0 | 35 | 0 |
| `sub_82293998` | matrix-math.md | Native | `sub_822936D8` | 2 | 0 | 118 | 0 |
| `sub_82296270` | functions.md | Overridden | `sub_822972D0` | 2 | 2 | 12 | 0 |
| `sub_8229E308` | command-buffer.md | Overridden | `sub_822AD690`, `sub_822AD8F8` | 2 | 2 | 95 | 0 |
| `sub_8229EC00` | command-buffer.md | Native | `sub_822AD690`, `sub_822AD8F8` | 2 | 2 | 95 | 0 |
| `sub_822AD298` | functions.md | Overridden | `sub_822AD190`, `sub_822B2388` | 2 | 0 | 128 | 0 |
| `sub_822B0910` | functions.md | Overridden | `sub_822B07B0`, `sub_822B0860` | 2 | 0 | 142 | 0 |
| `sub_823F6C60` | texture-compression.md | Native | `sub_823EAB58`, `sub_823F55B0` | 2 | 2 | 197 | 0 |
| `sub_82481880` | car-sounds.md | Native | `sub_82480780`, `sub_82481708` | 2 | 2 | 196 | 0 |
| `sub_82575B08` | system-services.md | Native | `sub_82459C10` | 2 | 0 | 67 | 1 |
| `sub_82590EF0` | models.md | Overridden | `sub_82590D18`, `sub_825981C8` | 2 | 2 | 91 | 0 |
| `sub_825963F0` | models.md | Native | `sub_825ABFE8` | 2 | 2 | 101 | 0 |
| `sub_8259ED40` | ui-movies.md | Overridden | `sub_8211A948`, `sub_8259D648` | 2 | 0 | 73 | 0 |
| `sub_825A8ED0` | c-runtime.md | Overridden | `sub_825A8D50`, `sub_825AB1C8` | 2 | 1 | 144 | 0 |
| `sub_825B3430` | c-runtime.md | Overridden | `sub_825B32F0` | 2 | 2 | 107 | 0 |
| `sub_825C27C0` | xact.md | Overridden | `sub_825C35C0` | 2 | 2 | 213 | 0 |
| `sub_825CA980` | xact.md | Overridden | `sub_825D0540` | 2 | 2 | 19 | 0 |
| `sub_82090CC0` | files-and-saves.md | Overridden | `sub_8258BE48` | 1 | 0 | 6 | 1 |
| `sub_82091328` | std-string.md | Overridden | `sub_825A7270` | 1 | 0 | 77 | 0 |
| `sub_82091458` | std-string.md | Native | `sub_825A7270` | 1 | 0 | 77 | 0 |
| `sub_82091540` | std-string.md | Native | `sub_825A6A20` | 1 | 1 | 28 | 0 |
| `sub_82091808` | std-string.md | Overridden | `sub_825A7270` | 1 | 0 | 77 | 0 |
| `sub_82098A98` | frame-driver.md | Overridden | `sub_8258C960` | 1 | 0 | 16 | 0 |
| `sub_8209A0E8` | framework.md | Native | `sub_82091CD0` | 1 | 1 | 25 | 0 |
| `sub_8209CC38` | sprites.md | Overridden | `sub_8209C6E0` | 1 | 0 | 475 | 0 |
| `sub_8209F340` | racer-controllers.md | Native | `sub_82591C08` | 1 | 0 | 6 | 0 |
| `sub_820A48B8` | files-and-saves.md | Overridden | `sub_82295C10` | 1 | 0 | 5 | 0 |
| `sub_820A4960` | files-and-saves.md | Overridden | `sub_822952E0` | 1 | 0 | 5 | 0 |
| `sub_820A49D0` | files-and-saves.md | Overridden | `sub_82295C08` | 1 | 0 | 5 | 0 |
| `sub_820A4B18` | files-and-saves.md | Overridden | `sub_82295BC8` | 1 | 0 | 13 | 0 |
| `sub_820A5110` | files-and-saves.md | Overridden | `sub_82295BF8` | 1 | 0 | 5 | 0 |
| `sub_820A51E0` | files-and-saves.md | Overridden | `sub_82295BE8` | 1 | 0 | 5 | 0 |
| `sub_820AEC10` | movies.md | Overridden | `sub_820AE7F8` | 1 | 1 | 8 | 1 |
| `sub_820BCDD0` | collision.md | Native | `sub_820BACB8` | 1 | 0 | 9 | 0 |
| `sub_820BCE80` | collision.md | Native | `sub_820BACB8` | 1 | 0 | 9 | 0 |
| `sub_820BE150` | collision.md | Native | `sub_820BC8A8` | 1 | 1 | 39 | 0 |
| `sub_820F2D28` | minimap.md | Native | `sub_820F2CA8` | 1 | 1 | 42 | 0 |
| `sub_82107B90` | boot-mode.md | Overridden | `sub_825894C8` | 1 | 0 | 75 | 0 |
| `sub_82107BF8` | boot-mode.md | Overridden | `sub_825895A0` | 1 | 0 | 61 | 0 |
| `sub_82109560` | strings-and-font.md | Native | `sub_82108F68` | 1 | 1 | 53 | 0 |
| `sub_82109AA8` | strings-and-font.md | Native | `sub_821097F0` | 1 | 1 | 206 | 0 |
| `sub_8210A490` | strings-and-font.md | Overridden | `sub_825A9A80` | 1 | 0 | 7 | 0 |
| `sub_8210A5E8` | strings-and-font.md | Overridden | `sub_825A7EF8` | 1 | 0 | 20 | 0 |
| `sub_8210A990` | strings-and-font.md | Overridden | `sub_825A7EF8` | 1 | 0 | 20 | 0 |
| `sub_8210ABC0` | strings-and-font.md | Overridden | `sub_825A7EF8` | 1 | 0 | 20 | 0 |
| `sub_8210ADD0` | strings-and-font.md | Overridden | `sub_82109A00` | 1 | 0 | 23 | 1 |
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
| `sub_8222DC08` | location-screen.md | Overridden | `sub_8211A1E0` | 1 | 0 | 270 | 0 |
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
| `sub_82297CE0` | system-services.md | Overridden | `sub_82297C08` | 1 | 0 | 66 | 0 |
| `sub_8229E100` | command-buffer.md | Native | `sub_8229DC28` | 1 | 0 | 55 | 1 |
| `sub_8229E848` | command-buffer.md | Overridden | `sub_8229DFF8` | 1 | 1 | 48 | 0 |
| `sub_8229EDA8` | command-buffer.md | Native | `sub_822AD850` | 1 | 1 | 58 | 0 |
| `sub_8229EF48` | command-buffer.md | Overridden | `sub_8229E5D0` | 1 | 0 | 119 | 1 |
| `sub_822A3A20` | device-state.md | Native | `sub_822B0C38` | 1 | 0 | 92 | 0 |
| `sub_822A63F8` | graphics-resources.md | Native | `sub_8229DDD8` | 1 | 0 | 55 | 1 |
| `sub_822B6BF8` | predicated-tiling.md | Overridden | `sub_8229E5D0` | 1 | 0 | 119 | 1 |
| `sub_8235A990` | models.md | Native | `sub_822A5B68` | 1 | 0 | 7 | 1 |
| `sub_82459D60` | xaudio-engine.md | Overridden | `sub_8245C638` | 1 | 0 | 9 | 0 |
| `sub_8245C650` | audio.md | Overridden | `sub_82295728` | 1 | 1 | 127 | 0 |
| `sub_8245C958` | audio.md | Overridden | `sub_8245D118` | 1 | 1 | 10 | 0 |
| `sub_82465258` | xaudio-objects.md | Native | `sub_82467A00` | 1 | 0 | 12 | 0 |
| `sub_824696F0` | xaudio-objects.md | Native | `sub_82469728` | 1 | 1 | 48 | 0 |
| `sub_8246F140` | xaudio-objects.md | Overridden | `sub_8246F270` | 1 | 1 | 39 | 0 |
| `sub_8258BD28` | second-set.md | Native | `sub_8258B7E0` | 1 | 1 | 147 | 0 |
| `sub_82594570` | models.md | Native | `sub_825981C8` | 1 | 1 | 62 | 0 |
| `sub_82595550` | models.md | Overridden | `sub_825A1148` | 1 | 1 | 54 | 0 |
| `sub_82598288` | car-scene-draw.md | Native | `sub_825981C8` | 1 | 1 | 62 | 0 |
| `sub_8259F700` | ui-movies.md | Native | `sub_8259EC58` | 1 | 1 | 84 | 0 |
| `sub_825A82C8` | c-runtime.md | Native | `sub_825AFD98` | 1 | 1 | 16 | 0 |
| `sub_825A9A90` | heap.md | Overridden | `sub_825AD150` | 1 | 1 | 26 | 0 |
| `sub_825AC248` | heap.md | Native | `sub_825AD150` | 1 | 1 | 26 | 0 |
| `sub_825AEBC8` | heap.md | Overridden | `sub_825AD150` | 1 | 1 | 26 | 0 |
| `sub_825C4D90` | xact.md | Overridden | `sub_825C4DC8` | 1 | 1 | 18 | 0 |
| `sub_825C7F98` | xact.md | Overridden | `sub_825C7FE8` | 1 | 1 | 53 | 0 |
| `sub_825C8230` | xact.md | Overridden | `sub_825C7B10` | 1 | 1 | 32 | 0 |
| `sub_825CC1D0` | xact.md | Overridden | `sub_825CC148` | 1 | 1 | 56 | 0 |
| `sub_825CC9C8` | xact.md | Overridden | `sub_825CC858` | 1 | 1 | 48 | 0 |

## Ours functions whose original body is still reachable

Ours by their rows, but something reaches their recompiled body: an `__imp__` fallback in our C++ (taken when the native C++ declines: a block not in our heap, a flag of the callee's off), a guest call to a function with no override and no C++, or an unported caller of one without an override. Each with its callers in that variant (native ones marked `n`):

`sub_82099B18` (Overridden; from sub_82090108 n, sub_82091A98 n, sub_82091C48 n ...), `sub_8209A080` (Overridden; from sub_82090108 n), `sub_8209BD50` (Overridden; from sub_820E42E8 n, sub_820E45A8 n), `sub_8209F738` (Native; from sub_8209F9B8 n), `sub_820A2D30` (Overridden; from sub_820E42E8 n, sub_820E45A8 n), `sub_820A8F80` (Merged; from sub_820A9198), `sub_820A9198` (Merged; from sub_820A9200), `sub_820AC7F0` (Overridden; from sub_820AC8A0 n), `sub_820AC8A0` (Overridden; from sub_820AC7F0 n), `sub_820B18B8` (Native; from sub_820B6498, sub_820B6498 n), `sub_820B2100` (Native; from sub_820B6498, sub_820B6498 n), `sub_820B5670` (Overridden; from sub_820C3150 n, sub_820C33B8 n), `sub_820B5B08` (Overridden; from sub_820C3150 n, sub_820C33B8 n), `sub_820B5FD8` (Overridden; from sub_820BFD30, sub_820BFD30 n, sub_820C3020 n), `sub_820B6498` (Overridden; from sub_820C3020 n, sub_821706E8, sub_821724F8 n), `sub_820B7998` (Overridden; from sub_820C3608, sub_820C3608 n, sub_821046F8 n ...), `sub_820B81C8` (Native; from sub_820B8548 n), `sub_820BB758` (Overridden; from sub_820B7998, sub_820B7998 n, sub_820B92C8 n), `sub_820BBE30` (Overridden; from sub_820B87B0 n), `sub_820BFD30` (Native; from sub_820C3020 n), `sub_820C0118` (Overridden; from sub_82171560 n), `sub_820C1D08` (Overridden; from sub_820C3150 n, sub_820C33B8 n, sub_821046F8 n ...), `sub_820C20E0` (Overridden; from sub_82170668 n, sub_82171978 n, sub_82171E50 n ...), `sub_820C23E8` (Overridden; from sub_82165E20 n, sub_821662E0 n, sub_82166E50 n ...), `sub_820C27E8` (Overridden; from sub_820C3150 n, sub_820C33B8 n, sub_820CD5C8 n ...), `sub_820C3608` (Overridden; from sub_821046F8 n, sub_82104898 n, sub_82104D40 n ...), `sub_820C3B78` (Overridden; from sub_82171250 n, sub_82171308), `sub_820C3BD0` (Overridden; from sub_82173A90 n), `sub_820C5C88` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820DC448 n ...), `sub_820CC470` (Merged; from sub_8224B660 n, sub_8224B708 n), `sub_820D09A0` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820C9608 n ...), `sub_820D3318` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_820D3688` (Native; from sub_820E45A8 n), `sub_820D3740` (Native; from sub_820E42E8 n), `sub_820D47A8` (Overridden; from sub_820D4F80 n), `sub_820D4F80` (Overridden; from sub_820D47A8 n), `sub_820D5890` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D6108` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_820D63A0` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D6A70` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D7428` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D78B8` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D7D38` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_820D8908` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820D9570` (Native; from sub_820C8DE8 n, sub_82180B00 n), `sub_820F0070` (Native; from sub_820F0820 n), `sub_820F4128` (Native; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D06F0 n ...), `sub_820FEFB0` (Overridden; from sub_82171250 n), `sub_820FFD88` (Overridden; from sub_820C5DE8 n, sub_820F5740 n, sub_82104F68 n ...), `sub_8211AED0` (Overridden; from sub_8211CD90 n, sub_8211E258 n, sub_8211E370 n ...), `sub_8211BD10` (Overridden; from sub_8211E500 n), `sub_82128E70` (Overridden; from sub_82129280 n), `sub_8213B1E0` (Overridden; from sub_82173A90 n), `sub_8213B2B8` (Overridden; from sub_82171250 n, sub_82171308), `sub_82150350` (Native; from sub_821561F8 n), `sub_821715B8` (Native; from sub_8213B1E0 n, sub_82173A90 n), `sub_8217C368` (Merged; from sub_82096418 n), `sub_82294330` (Merged; from sub_820B81C8), `sub_82295330` (Overridden; from sub_8209DD98 n, sub_8209E118 n), `sub_822A5B78` (Overridden; from sub_8211E500 n), `sub_822A6008` (Overridden; from sub_82098940 n, sub_822A8A60 n, sub_822AD298 n), `sub_822A8740` (Native; from sub_820C52A0 n, sub_820D6528 n, sub_820D7578 n ...), `sub_82594D10` (Native; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_82596920` (Overridden; from sub_820F0820 n), `sub_825986B8` (Native; from sub_8209CC38 n, sub_820C5DE8 n, sub_820F5740 n ...), `sub_825998A0` (Overridden; from sub_8209CC38 n, sub_820C5DE8 n, sub_820D8EC0 n ...), `sub_8259F770` (Native; from sub_8259FA00 n), `sub_8259FB30` (Native; from sub_825A0548 n), `sub_8259FC18` (Native; from sub_825A0548 n), `sub_825A7068` (Overridden; from sub_820AB358 n, sub_82152338 n, sub_82155288 n ...), `sub_825A70E8` (Overridden; from sub_82090148, sub_82090190, sub_820901D8 ...), `sub_825A7410` (Overridden; from sub_8209CC38 n, sub_820AC7F0 n, sub_820AC8A0 n ...), `sub_825B6458` (Merged; from sub_825AE1A8, sub_825AE9D0)

## Reached past the movie library's end (82575B08-8257FFFF)

`b7-loop.md` ends the movie library's first range at 82575B07; `movies.md` at 8257FFFF. Reachable there: 148 functions (6 ours, 142 unported, 0 reached only through the replaced libraries). Entered from 20 functions outside it:

- `sub_82148868` (ours, system-services.md): `sub_82575B08` (ours).
- `sub_82148D00` (unported, unmapped): `sub_825765F0` (unported).
- `sub_82149178` (unported, system-services.md): `sub_82576E78` (ours).
- `sub_8214E678` (unported, unmapped): `sub_82576080` (unported).
- `sub_8214E6C8` (unported, unmapped): `sub_82575E50` (unported).
- `sub_8214EC80` (unported, unmapped): `sub_825763D0` (unported).
- `sub_8214ED30` (unported, unmapped): `sub_82576080` (unported), `sub_825764A8` (unported), `sub_82576540` (unported).
- `sub_822917B8` (unported, unmapped): `sub_82575E50` (unported).
- `sub_82291858` (unported, unmapped): `sub_825761D8` (unported).
- `sub_82291920` (unported, unmapped): `sub_825764A8` (unported).
- `sub_822919A0` (unported, unmapped): `sub_82576540` (unported).
- `sub_82291A40` (unported, unmapped): `sub_82576328` (unported).
- `sub_82291AD0` (unported, unmapped): `sub_82576080` (unported).
- `sub_82292B28` (unported, unmapped): `sub_825766E0` (unported).
- `sub_82292E98` (unported, unmapped): `sub_82576080` (unported).
- `sub_82293028` (unported, unmapped): `sub_82576BE0` (unported), `sub_82576F30` (unported).
- `sub_82293190` (unported, unmapped): `sub_82576080` (unported).
- `sub_8245A410` (ours, sound-cues.md): `sub_82578AF0` (unported).
- `sub_82580020` (unported, unmapped): `sub_8257FF68` (unported).
- `sub_82580028` (unported, unmapped): `sub_8257FF68` (unported).

Each function there, with its callers: `sub_82575B08` (ours) from `sub_82148868`; `sub_82575B68` (unported) from `sub_82576BE0`; `sub_82575BF0` (unported) from `sub_82576BE0`; `sub_82575C90` (unported) from `sub_82576BE0`; `sub_82575D78` (unported) from `sub_82576BE0`; `sub_82575E50` (unported) from `sub_8214E6C8`, `sub_822917B8`; `sub_82576080` (unported) from `sub_8214E678`, `sub_8214ED30`, `sub_82291AD0` ...; `sub_825761D8` (unported) from `sub_82291858`; `sub_82576328` (unported) from `sub_82291A40`; `sub_825763D0` (unported) from `sub_8214EC80`; `sub_825764A8` (unported) from `sub_8214ED30`, `sub_82291920`; `sub_82576540` (unported) from `sub_8214ED30`, `sub_822919A0`; `sub_825765F0` (unported) from `sub_82148D00`; `sub_825766E0` (unported) from `sub_82292B28`; `sub_825767A0` (unported) from `sub_82576BE0`; `sub_82576BE0` (unported) from `sub_82293028`; `sub_82576E78` (ours) from `sub_82149178`; `sub_82576F30` (unported) from `sub_82293028`; `sub_82577088` (unported) from `sub_82577850`, `sub_825778D8`; `sub_825770B8` (unported) from `sub_82577190`, `sub_825778D8`; `sub_82577110` (unported) from `sub_825786B8`; `sub_82577140` (unported) from `sub_82577190`, `sub_825778D8`; `sub_82577190` (unported) from `sub_825778D8`; `sub_825774D0` (unported) from `sub_82575BF0`, `sub_82577630`; `sub_82577528` (unported) from `sub_825778D8`, `sub_825786B8`; `sub_82577560` (ours) from `sub_82575B08`; `sub_82577630` (unported) from `sub_82577788`; `sub_825776A8` (unported) from `sub_82575C90`; `sub_82577788` (unported) from `sub_82575D78`; `sub_82577850` (unported) from `sub_825778D8`; `sub_825778D8` (unported) from `sub_825780A0`; `sub_825780A0` (unported) from `sub_82575D78`; `sub_825780D0` (unported) from `sub_82577630`, `sub_825776A8`, `sub_82577788` ...; `sub_825780F0` (unported) from `sub_82577190`, `sub_825778D8`; `sub_82578118` (unported) from `sub_82578168`, `sub_825784A8`, `sub_82578530` ...; `sub_82578168` (unported) from `sub_82578500`; `sub_82578248` (unported) from `sub_825778D8`, `sub_825787D0`; `sub_825782C0` (unported) from `sub_825778D8`, `sub_825787D0`; `sub_82578340` (unported) from `sub_825778D8`, `sub_825787D0`; `sub_825783C8` (unported) from `sub_825778D8`, `sub_825787D0`; `sub_82578470` (unported) from `sub_825778D8`; `sub_825784A8` (unported) from `sub_82577190`, `sub_825778D8`; `sub_82578500` (unported) from `sub_825770B8`, `sub_82577140`, `sub_82577190` ...; `sub_82578530` (unported) from `sub_82577088`, `sub_825770B8`, `sub_82577110` ...; `sub_82578580` (unported) from `sub_82577190`, `sub_825778D8`; `sub_82578588` (unported) from `sub_825778D8`; `sub_82578608` (unported) from `sub_825778D8`, `sub_825786B8`; `sub_82578658` (unported) from `sub_825778D8`; `sub_825786B8` (unported) from `sub_825778D8`; `sub_825787D0` (unported) from `sub_825778D8`; `sub_825788C0` (unported) from `sub_82578AF0`; `sub_82578AF0` (unported) from `sub_8245A410`; `sub_82578C88` (unported) from `sub_825796C0`; `sub_82578CB8` (unported) from `sub_82579448`; `sub_82578CD0` (unported) from `sub_82578D70`, `sub_82578F00`, `sub_825791B0`; `sub_82578D70` (unported) from `sub_825790F0`; `sub_82578F00` (unported) from `sub_825790F0`; `sub_825790F0` (unported) from `sub_825796C0`; `sub_82579108` (unported) from `sub_82579630`, `sub_825796C0`; `sub_825791B0` (unported) from `sub_825796C0`; `sub_82579280` (unported) from `sub_825796C0`; `sub_82579318` (unported) from `sub_825796C0`; `sub_82579448` (unported) from `sub_825796C0`; `sub_82579590` (unported) from `sub_8257F198`; `sub_82579630` (unported) from `sub_825796C0`; `sub_825796C0` (unported) from `sub_82579798`; `sub_82579798` (unported) from `sub_825797B0`, `sub_82579830`, `sub_82579898` ...; `sub_825797B0` (unported) from `sub_825796C0`; `sub_825797C0` (unported) from `sub_82579968`; `sub_82579830` (unported) from `sub_82579C48`; `sub_82579880` (unported) from `sub_82579C48`; `sub_82579898` (unported) from `sub_82579C48`; `sub_82579968` (unported) from `sub_82579C48`; `sub_82579A88` (unported) from `sub_82579C48`; `sub_82579BE8` (unported) from `sub_82579C48`; `sub_82579C48` (unported) from `sub_82579CD0`; `sub_82579CD0` (unported) from `sub_82579CF0`; `sub_82579CF0` (unported) from `sub_82579C48`; `sub_82579D00` (unported) from `sub_825788C0`; `sub_82579D18` (unported) from `sub_82579DA0`; `sub_82579D40` (unported) from `sub_82579DA0`; `sub_82579D60` (unported) from `sub_82579DA0`; `sub_82579D88` (unported) from `sub_82579DA0`; `sub_82579DA0` (unported) from `sub_825788C0`; `sub_82579E18` (unported) from `sub_82579DA0`; `sub_8257A3D0` (unported) from `sub_82579A88`; `sub_8257A3F0` (unported) from `sub_82579BE8`; `sub_8257A498` (unported) from `sub_82579DA0`; `sub_8257B9A8` (unported) from `sub_82579630`, `sub_82579898`; `sub_8257B9D0` (unported) from `sub_82579108`, `sub_82579830`; `sub_8257B9F8` (unported) from `sub_825796C0`, `sub_82579C48`; `sub_8257BA88` (unported) from `sub_8257BB60`; `sub_8257BAA0` (unported) from `sub_8257BB60`; `sub_8257BAD0` (unported) from `sub_8257BB60`; `sub_8257BB00` (unported) from `sub_82578C88`, `sub_82579880`; `sub_8257BB28` (unported) from `sub_82578C88`; `sub_8257BB50` (unported) from `sub_8257BB60`; `sub_8257BB60` (unported) from `sub_8257BBC8`, `sub_8257BE48`; `sub_8257BBC8` (unported) from `sub_82578C88`, `sub_82579880`; `sub_8257BC48` (unported) from `sub_82578C88`, `sub_82579880`; `sub_8257BE48` (unported) from `sub_82578C88`; `sub_8257BEC0` (unported) from `sub_82578C88`; `sub_8257C160` (unported) from `sub_82578CD0`, `sub_825797C0`, `sub_82579A88` ...; `sub_8257C2B0` (unported) from `sub_8257C3E0`; `sub_8257C3E0` (unported) from `sub_8257EDE8`; `sub_8257C570` (unported) from `sub_82578F00`, `sub_8257B9F8`, `sub_8257CCD8`; `sub_8257C590` (unported) from `sub_8257BC48`, `sub_8257BEC0`, `sub_8257C2B0`; `sub_8257C610` (unported) from `sub_8257C160`; `sub_8257C6B0` (unported) from `sub_8257C3E0`; `sub_8257C710` (unported) from `sub_82579280`, `sub_82579BE8`, `sub_8257CFA8`; `sub_8257C858` (ours) from `sub_825791B0`, `sub_82579A88`, `sub_8257D518`; `sub_8257C948` (unported) from `sub_8257C9A0`, `sub_8257D298`; `sub_8257C9A0` (unported) from `sub_8257D020`; `sub_8257CA10` (unported) from `sub_8257D298`; `sub_8257CB38` (unported) from `sub_8257D298`; `sub_8257CC08` (unported) from `sub_8257D298`; `sub_8257CCD8` (unported) from `sub_8257D298`; `sub_8257CF10` (unported) from `sub_8257D020`; `sub_8257CFA8` (unported) from `sub_82579280`, `sub_8257D518`; `sub_8257D020` (unported) from `sub_82578F00`; `sub_8257D298` (unported) from `sub_82579590`; `sub_8257D460` (unported) from `sub_82579630`; `sub_8257D518` (unported) from `sub_825791B0`; `sub_8257D6D8` (unported) from `sub_82579630`; `sub_8257D6F8` (unported) from `sub_82579108`; `sub_8257DB08` (unported) from `sub_82579E18`; `sub_8257E8E8` (unported) from `sub_8257F070`, `sub_8257F1E8`; `sub_8257E9B8` (unported) from `sub_8257F070`; `sub_8257EAF0` (unported) from `sub_8257EF80`, `sub_8257EFF8`, `sub_8257F198`; `sub_8257EB68` (ours) from `sub_8257F070`; `sub_8257EDE8` (unported) from `sub_82579BE8`; `sub_8257EF80` (unported) from `sub_82579898`; `sub_8257EFF8` (unported) from `sub_82579830`; `sub_8257F070` (unported) from `sub_82579898`, `sub_82579A88`; `sub_8257F198` (unported) from `sub_82579968`; `sub_8257F1E8` (unported) from `sub_82579898`, `sub_82579A88`; `sub_8257F388` (ours) from `sub_82579A88`; `sub_8257F5E8` (unported) from `sub_82579E18`, `sub_8257BC48`, `sub_8257BEC0`; `sub_8257F970` (unported) from `sub_8257FA90`; `sub_8257FA90` (unported) from `sub_8257BC48`; `sub_8257FB38` (unported) from `sub_8257FBF8`, `sub_8257FC00`; `sub_8257FBF8` (unported) from `sub_8257BB00`, `sub_8257BBC8`; `sub_8257FC00` (unported) from `sub_8257FD18`; `sub_8257FD18` (unported) from `sub_8257BBC8`; `sub_8257FD48` (unported) from `sub_8257FEB8`; `sub_8257FEB8` (unported) from `sub_8257BEC0`; `sub_8257FF50` (unported) from `sub_8257BEC0`; `sub_8257FF68` (unported) from `sub_82580020`, `sub_82580028`.

## Our C++ naming replaced-library functions

Tables and vtables our C++ stores whose words land in the replaced libraries, and calls into `sub_822B8F10` (the HLSL compiler's entry, overridden by the stored shaders); not counted as the game's:

`sub_8209BE88` -> `sub_822B8F10`, `sub_8210ADD0` -> `sub_822B8F10`, `sub_8210AE60` -> `sub_824F65C0`, `sub_824600F0` -> `sub_824877D0`, `sub_82465938` -> `sub_824877D0`, `sub_8259C6D0` -> `sub_824F65C0`, `sub_825D82E8` -> `sub_824892F0`, `sub_825D8660` -> `sub_824892F0`

## Replaced-library functions with direct callers in the game's code

In the excluded ranges but called directly by unported game code ("checked against its callers first"), so counted as the game's (ours or unported), with what they reach:

`sub_82481B88`, `sub_82481B98`, `sub_82482068`, `sub_82482730`, `sub_82482780`, `sub_82482968`, `sub_82482B18`, `sub_82483130`, `sub_82483208`, `sub_82483238`, `sub_82483388`, `sub_82483460`, `sub_82483700`, `sub_82483770`, `sub_82483DC0`, `sub_82483DD0`, `sub_82483F30`, `sub_824847D0`, `sub_82485350`, `sub_82485460`, `sub_82486710`, `sub_82486810`, `sub_82486F90`, `sub_824875A8`, `sub_824875B8`, `sub_82487660`, `sub_824877E0`, `sub_824878B0`, `sub_824878D0`, `sub_82487B28`, `sub_82487BE8`, `sub_82488F50`, `sub_82489048`, `sub_824892A8`, `sub_824892D0`, `sub_824892F8`, `sub_82489300`, `sub_824893B8`, `sub_8248CA80`, `sub_8248CB58`, `sub_8248CBD0`, `sub_8248EC58`, `sub_8248ED78`, `sub_8248F030`, `sub_8248F218`, `sub_8248F550`, `sub_82490960`, `sub_82490B08`, `sub_824936E8`, `sub_82493700`, `sub_824960B8`, `sub_824960E0`, `sub_82496148`
