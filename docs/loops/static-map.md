# B7's static map

Made by `itc_recomp/tools/static_map.py` on every run; never edit it by hand (run the tool again). What it reads, how edges are found and its gaps: the tool's docstring and "The static map" in `docs/coverage.md`.

Made 2026-11-14 at commit `b13fe9e9` (`b7-loop-13-11-26`).

## Totals

- Recompiled `sub_` functions: 16552; 144 jump-table cases folded into their parents, leaving 16408.
- Reachable: 15910 (15528 from `main`, 382 more from 189 extra roots: functions seen in play that `main` doesn't reach, 1 of them not under another).
- Ours: 6539. Excluded (replaced libraries): 5201.
- **Unported: 4170** (3828 under `main`): seen in play 652, never seen 3518; 323890 generated lines.
- Of the unported, 626 hang under unported code (or a root); **3544 are reached only through ours** (415 seen in play): the original's callees of native functions. Native code may still call them (a guest call or fallback: 687 are named in `src/` by name or address; a guest vtable call names nothing) or may have left the call out (plugged-in data), in which case they're no work; the TSV's `behind_ours` and `named` columns list them.
- Frontier (ready now): 2175 functions.
- Reachable functions calling imports: 394 (199 unported).
- Image read: 13492 code-built data addresses scanned; 181 function pointers in data that no code-built address reaches (136 functions; not edges).

A `*` after a function: never seen in play. Sizes are generated lines.

## Blockers, by what they unblock

Unported functions ranked by how many unported functions above them (through unported callers) wait on them; "frees" is how many would join the frontier if this one alone became ours.

| Function | Unblocks | Frees | Lines | Seen | Ready | File |
|---|---|---|---|---|---|---|
| `sub_82294E20` | 379 | 5 | 24 | yes | yes | functions.md |
| `sub_825AA400` | 370 | 1 | 27 | yes | yes | unmapped |
| `sub_825AA4C0` | 369 | 2 | 68 | yes | no | unmapped |
| `sub_825A81E0` | 362 | 1 | 23 | no | no | unmapped |
| `sub_825B68B8` | 356 | 1 | 7 | no | yes | unmapped |
| `sub_825AFCC8` | 354 | 0 | 27 | no | no | unmapped |
| `sub_825A8218` | 325 | 0 | 23 | no | no | unmapped |
| `sub_825B7A58` | 324 | 5 | 15 | no | yes | unmapped |
| `sub_825B3570` | 324 | 1 | 16 | yes | yes | unmapped |
| `sub_825B35A0` | 323 | 1 | 35 | yes | no | unmapped |
| `sub_825A8178` | 323 | 0 | 36 | no | yes | unmapped |
| `sub_825B7880` | 323 | 0 | 68 | no | no | unmapped |
| `sub_825A8250` | 322 | 0 | 40 | no | no | unmapped |
| `sub_825B7A20` | 320 | 0 | 20 | no | yes | unmapped |
| `sub_825B6500` | 319 | 0 | 64 | no | no | files-and-saves.md |
| `sub_825B7958` | 319 | 0 | 68 | no | no | unmapped |
| `sub_825B0E88` | 316 | 1 | 32 | no | no | unmapped |
| `sub_825B0F48` | 312 | 0 | 195 | no | no | files-and-saves.md |
| `sub_825B1304` | 312 | 0 | 21 | no | no | unmapped |
| `sub_825B11A8` | 311 | 0 | 106 | no | no | unmapped |
| `sub_825ACC78` | 277 | 1 | 53 | no | no | unmapped |
| `sub_825AD688` | 273 | 3 | 20 | no | yes | unmapped |
| `sub_825AD6C0` | 273 | 2 | 12 | no | yes | unmapped |
| `sub_825AEDA0` | 271 | 1 | 23 | no | yes | unmapped |
| `sub_825AD670` | 271 | 0 | 12 | no | yes | unmapped |
| `sub_825B4978` | 271 | 0 | 21 | no | no | unmapped |
| `sub_825B77E8` | 271 | 0 | 50 | no | no | unmapped |
| `sub_825BA510` | 271 | 0 | 25 | no | yes | files-and-saves.md |
| `sub_825ACEB4` | 270 | 0 | 16 | no | yes | unmapped |
| `sub_825ACF04` | 270 | 0 | 32 | no | no | unmapped |
| `sub_825AD638` | 270 | 0 | 20 | no | yes | unmapped |
| `sub_825B0C28` | 270 | 0 | 81 | no | no | unmapped |
| `sub_825B0E50` | 270 | 0 | 21 | no | no | unmapped |
| `sub_825B4828` | 270 | 0 | 105 | no | no | files-and-saves.md |
| `sub_82297870` | 269 | 1 | 8 | yes | yes | unmapped |
| `sub_825ACD10` | 269 | 1 | 49 | no | no | unmapped |
| `sub_825ACD90` | 269 | 1 | 96 | no | no | unmapped |
| `sub_825B0D10` | 269 | 0 | 98 | no | no | unmapped |
| `sub_825B0EE0` | 269 | 0 | 36 | no | yes | unmapped |
| `sub_825A94C0` | 268 | 0 | 65 | no | no | unmapped |

## Frontier clusters, by what they unblock

773 clusters (frontier functions sharing an unported caller within 0x4000 bytes, neighbors in address order, or a call cycle). Each: what it unblocks, its functions with their sizes, its function files.

1. **sub_82294E20**-82295728: unblocks 393; 4 functions, 214 lines, 1 seen; functions.md, system-services.md. `sub_82294E20` 24, `sub_82294E58`* 48, `sub_82295700`* 15, `sub_82295728`* 127
2. **sub_825AA3F8**-825AA5C0: unblocks 373; 3 functions, 98 lines, 3 seen; unmapped. `sub_825AA3F8` 5, `sub_825AA400` 27, `sub_825AA5C0` 66
3. **sub_825B68B8**-825B68B8: unblocks 356; 1 functions, 7 lines, 0 seen; unmapped. `sub_825B68B8`* 7
4. **sub_825B7A20**-825B7A58: unblocks 325; 2 functions, 35 lines, 0 seen; unmapped. `sub_825B7A20`* 20, `sub_825B7A58`* 15
5. **sub_825B3570**-825B3570: unblocks 324; 1 functions, 16 lines, 1 seen; unmapped. `sub_825B3570` 16
6. **sub_825A8178**-825A8178: unblocks 323; 1 functions, 36 lines, 0 seen; unmapped. `sub_825A8178`* 36
7. **sub_825ACEB4**-825AD6C0: unblocks 279; 5 functions, 80 lines, 0 seen; unmapped. `sub_825ACEB4`* 16, `sub_825AD638`* 20, `sub_825AD670`* 12, `sub_825AD688`* 20, `sub_825AD6C0`* 12
8. **sub_82297870**-82297B98: unblocks 271; 5 functions, 138 lines, 3 seen; unmapped. `sub_82297870` 8, `sub_82297940`* 48, `sub_822979D0`* 6, `sub_82297B38` 37, `sub_82297B98` 39
9. **sub_825AD120**-825AEDA0: unblocks 271; 2 functions, 34 lines, 0 seen; unmapped. `sub_825AD120`* 11, `sub_825AEDA0`* 23
10. **sub_825BA510**-825BA510: unblocks 271; 1 functions, 25 lines, 0 seen; files-and-saves.md. `sub_825BA510`* 25
11. **sub_825B0EE0**-825B0EE0: unblocks 269; 1 functions, 36 lines, 0 seen; unmapped. `sub_825B0EE0`* 36
12. **sub_825B4F38**-825B4F38: unblocks 267; 1 functions, 16 lines, 0 seen; unmapped. `sub_825B4F38`* 16
13. **sub_825AA1B8**-825AA338: unblocks 265; 2 functions, 56 lines, 0 seen; c-runtime.md. `sub_825AA1B8`* 32, `sub_825AA338`* 24
14. **sub_825B4C8C**-825B4C8C: unblocks 262; 1 functions, 24 lines, 0 seen; unmapped. `sub_825B4C8C`* 24
15. **sub_825B49B0**-825B49B0: unblocks 261; 1 functions, 6 lines, 0 seen; unmapped. `sub_825B49B0`* 6
16. **sub_82090380**-82094A70: unblocks 163; 19 functions, 325 lines, 4 seen; frame-driver.md, single-commands.md. `sub_82090380`* 28, `sub_820903D0`* 28, `sub_820911F8`* 5, `sub_82091648`* 12, `sub_82091910`* 30, `sub_82092A70`* 28, `sub_82092AC0`* 28, `sub_82094380` 9, `sub_82094398`* 9, `sub_820943E0` 8, `sub_820943F8` 8, `sub_82094410`* 5, ... 7 more
17. **sub_825B4790**-825B4790: unblocks 134; 1 functions, 44 lines, 0 seen; unmapped. `sub_825B4790`* 44
18. **sub_825BFEA0**-825C6918: unblocks 120; 24 functions, 1068 lines, 6 seen; xact.md. `sub_825BFEA0` 65, `sub_825C05C8`* 70, `sub_825C0698`* 36, `sub_825C1548`* 11, `sub_825C1568` 40, `sub_825C1738`* 23, `sub_825C1788`* 43, `sub_825C38A8` 108, `sub_825C39F8`* 117, `sub_825C4940`* 89, `sub_825C4A58`* 12, `sub_825C4A78`* 12, ... 12 more
19. **sub_821AA500**-821AFA08: unblocks 119; 61 functions, 5075 lines, 0 seen; car-settings.md. `sub_821AA500`* 15, `sub_821AA530`* 16, `sub_821AA560`* 182, `sub_821AA7B8`* 134, `sub_821AA970`* 103, `sub_821AAAB8`* 130, `sub_821AAC60`* 171, `sub_821AAE90`* 40, `sub_821AAF08`* 35, `sub_821AB038`* 161, `sub_821AB250`* 33, `sub_821AB2C0`* 182, ... 49 more
20. **sub_82148768**-8214A488: unblocks 106; 7 functions, 514 lines, 0 seen; unmapped. `sub_82148768`* 118, `sub_82148F88`* 24, `sub_82149AA8`* 33, `sub_82149B08`* 34, `sub_8214A198`* 73, `sub_8214A280`* 99, `sub_8214A488`* 133
21. **sub_82575FE0**-825767A0: unblocks 105; 12 functions, 745 lines, 0 seen; system-services.md. `sub_82575FE0`* 62, `sub_82576080`* 63, `sub_82576130`* 64, `sub_825761D8`* 64, `sub_82576280`* 63, `sub_82576328`* 63, `sub_825763D0`* 77, `sub_825764A8`* 60, `sub_82576540`* 63, `sub_825765F0`* 79, `sub_825766E0`* 72, `sub_825767A0`* 15
22. **sub_82459C20**-82459D28: unblocks 97; 13 functions, 114 lines, 0 seen; system-services.md. `sub_82459C20`* 9, `sub_82459C38`* 7, `sub_82459C48`* 9, `sub_82459C60`* 9, `sub_82459C78`* 9, `sub_82459C90`* 8, `sub_82459CA0`* 9, `sub_82459CB8`* 11, `sub_82459CD8`* 12, `sub_82459CF8`* 12, `sub_82459D18`* 5, `sub_82459D20`* 5, ... 1 more
23. **sub_82292648**-82295208: unblocks 87; 4 functions, 233 lines, 0 seen; system-services.md. `sub_82292648`* 82, `sub_822950C0`* 81, `sub_822951A8`* 35, `sub_82295208`* 35
24. **sub_825C07C8**-825C1090: unblocks 86; 5 functions, 204 lines, 1 seen; xact.md. `sub_825C07C8`* 14, `sub_825C07F0`* 30, `sub_825C0848`* 43, `sub_825C0958`* 28, `sub_825C1090` 89
25. **sub_825CA860**-825CD388: unblocks 83; 26 functions, 638 lines, 2 seen; xact.md. `sub_825CA860` 29, `sub_825CAC58`* 33, `sub_825CACB8`* 37, `sub_825CAD18`* 53, `sub_825CADC0`* 15, `sub_825CADF0`* 10, `sub_825CAE08`* 44, `sub_825CAFD8` 34, `sub_825CB660`* 18, `sub_825CB6A0`* 31, `sub_825CC0E0`* 40, `sub_825CC148`* 56, ... 14 more
26. **sub_82583FD0**-825892B8: unblocks 77; 45 functions, 3190 lines, 5 seen; unmapped. `sub_82583FD0` 61, `sub_82584080`* 28, `sub_825840D0` 63, `sub_82584178`* 38, `sub_825841E8`* 55, `sub_82584970`* 9, `sub_82584988` 44, `sub_82584A00` 38, `sub_82584A70`* 83, `sub_82584B90`* 68, `sub_82584E48`* 24, `sub_82584E78`* 24, ... 33 more
27. **sub_825D60C0**-825D8F18: unblocks 77; 9 functions, 388 lines, 0 seen; unmapped. `sub_825D60C0`* 14, `sub_825D6180`* 63, `sub_825D6A88`* 25, `sub_825D8B28`* 21, `sub_825D8C78`* 62, `sub_825D8D50`* 29, `sub_825D8DA0`* 90, `sub_825D8E98`* 53, `sub_825D8F18`* 31
28. **sub_8225FB88**-82263A98: unblocks 74; 23 functions, 1401 lines, 0 seen; unmapped. `sub_8225FB88`* 94, `sub_8225FCB8`* 29, `sub_8225FD18`* 29, `sub_8225FD78`* 27, `sub_8225FDD0`* 29, `sub_8225FE30`* 30, `sub_8225FE98`* 30, `sub_8225FF00`* 30, `sub_8225FF68`* 29, `sub_8225FFC8`* 134, `sub_82260170`* 21, `sub_822601B0`* 22, ... 11 more
29. **sub_82294F78**-82294F78: unblocks 74; 1 functions, 55 lines, 0 seen; system-services.md. `sub_82294F78`* 55
30. **sub_822AD690**-822AF3D0: unblocks 71; 16 functions, 1311 lines, 7 seen; functions.md, texture-compression.md. `sub_822AD690`* 52, `sub_822AD740`* 23, `sub_822AD790`* 63, `sub_822AD850`* 58, `sub_822AD8F8`* 43, `sub_822AD978`* 33, `sub_822ADA28` 37, `sub_822ADA98` 55, `sub_822ADB50` 71, `sub_822ADC08` 205, `sub_822AE340` 439, `sub_822AE918`* 62, ... 4 more
31. **sub_8214F790**-8214F790: unblocks 71; 1 functions, 43 lines, 0 seen; system-services.md. `sub_8214F790`* 43
32. **sub_825D5698**-825D5758: unblocks 69; 7 functions, 79 lines, 0 seen; xact-tracks.md. `sub_825D5698`* 11, `sub_825D56B8`* 11, `sub_825D56D8`* 11, `sub_825D56F8`* 11, `sub_825D5718`* 11, `sub_825D5738`* 11, `sub_825D5758`* 13
33. **sub_82094CD0**-8258E578: unblocks 66; 17 functions, 623 lines, 5 seen; video-output.md. `sub_82094CD0`* 29, `sub_8258BE58`* 121, `sub_8258BFA0`* 66, `sub_8258C060`* 30, `sub_8258D030`* 33, `sub_8258D090` 25, `sub_8258D0D8` 50, `sub_8258D170` 37, `sub_8258D1E0`* 35, `sub_8258D248`* 21, `sub_8258D290`* 45, `sub_8258D318`* 47, ... 5 more
34. **sub_823EA0C0**-823EB408: unblocks 66; 5 functions, 1210 lines, 0 seen; texture-compression.md. `sub_823EA0C0`* 609, `sub_823EADB0`* 59, `sub_823EAE40`* 269, `sub_823EB180`* 215, `sub_823EB408`* 58
35. **sub_82579D00**-8257FF50: unblocks 58; 16 functions, 745 lines, 4 seen; app-start-up.md, system-services.md. `sub_82579D00` 10, `sub_82579D18`* 26, `sub_82579D40` 12, `sub_82579D60`* 14, `sub_82579D88`* 10, `sub_8257A3D0` 11, `sub_8257A3F0`* 51, `sub_8257A498`* 26, `sub_8257C570`* 13, `sub_8257C590`* 40, `sub_8257C610` 53, `sub_8257C6B0`* 34, ... 4 more
36. **sub_825DADF0**-825DB028: unblocks 54; 60 functions, 273 lines, 0 seen; unmapped. `sub_825DADF0`* 4, `sub_825DADF8`* 4, `sub_825DAE00`* 4, `sub_825DAE08`* 4, `sub_825DAE10`* 4, `sub_825DAE18`* 4, `sub_825DAE20`* 4, `sub_825DAE28`* 34, `sub_825DAE88`* 4, `sub_825DAE90`* 4, `sub_825DAE98`* 4, `sub_825DAEA0`* 4, ... 48 more
37. **sub_82587F98**-825881E0: unblocks 54; 3 functions, 499 lines, 0 seen; unmapped. `sub_82587F98`* 107, `sub_825880B8`* 108, `sub_825881E0`* 284
38. **sub_8259C550**-8259C550: unblocks 51; 1 functions, 116 lines, 0 seen; unmapped. `sub_8259C550`* 116
39. **sub_82576EE0**-82576FE8: unblocks 48; 3 functions, 135 lines, 0 seen; system-services.md. `sub_82576EE0`* 27, `sub_82576F30`* 56, `sub_82576FE8`* 52
40. **sub_825CA6F8**-825CA6F8: unblocks 46; 1 functions, 88 lines, 1 seen; xact.md. `sub_825CA6F8` 88

## Unmapped areas

Clusters of unported reachable functions with no rows in any function file: 305 (906 functions, 72830 lines). Largest first; the callers named are outside the cluster.

- **sub_8246F270**-82480368: 93 functions, 21232 lines, 0 seen, unblocks 0; called from `sub_8246F140` (xaudio-objects.md, ours).
- **sub_820A70A8**-820A8B38: 74 functions, 2549 lines, 10 seen, unblocks 4; called from `sub_82090910` (unmapped, unported), `sub_820A75E8` (cameras.md, ours), `sub_820A8B88` (cameras.md, ours), `sub_82593E30` (movies.md, unported) ....
- **sub_822820A8**-822872A8: 53 functions, 6182 lines, 0 seen, unblocks 0; called from `sub_822873A0` (network-mode.md, ours).
- **sub_82272968**-82276330: 44 functions, 4799 lines, 0 seen, unblocks 0; called from `sub_82276510` (network-mode.md, ours).
- **sub_82265930**-822681D0: 26 functions, 3219 lines, 0 seen, unblocks 1; called from `sub_82268410` (network-mode.md, ours), `sub_82268720` (unmapped, unported), `sub_82278C20` (network-mode.md, ours).
- **sub_820B95F8**-820BE0D8: 24 functions, 1995 lines, 0 seen, unblocks 13; called from `sub_820A75E8` (cameras.md, ours), `sub_820BAB48` (collision.md, ours), `sub_820BB020` (collision.md, ours), `sub_820BB140` (collision.md, ours) ....
- **sub_822609F0**-82262820: 22 functions, 2235 lines, 0 seen, unblocks 2; called from `sub_82262A88` (network-mode.md, ours), `sub_82262E10` (unmapped, unported), `sub_82262F20` (unmapped, unported), `sub_8227A000` (network-mode.md, ours).
- **sub_822802C8**-82281800: 18 functions, 1699 lines, 0 seen, unblocks 0; called from `sub_82281858` (network-mode.md, ours).
- **sub_820ACCF0**-820ADB80: 17 functions, 521 lines, 0 seen, unblocks 3; called from `sub_820AD118` (app-states.md, ours), `sub_820ADBD8` (app-states.md, ours), `sub_820AE198` (unmapped, unported), `sub_820AE1C8` (app-states.md, ours) ....
- **sub_8225E268**-8225F100: 17 functions, 1292 lines, 0 seen, unblocks 0; called from `sub_8225F330` (network-mode.md, ours).
- **sub_8259A010**-8259BE40: 17 functions, 2919 lines, 0 seen, unblocks 5; called from `sub_820EFFD8` (accessories.md, ours), `sub_820F0120` (accessories.md, unported), `sub_820F03F0` (accessories.md, ours), `sub_820F0628` (accessories.md, unported) ....
- **sub_8227E1D8**-8227F4E8: 16 functions, 970 lines, 0 seen, unblocks 3; called from `sub_822785E0` (network-mode.md, unported), `sub_8227A9F0` (network-mode.md, ours), `sub_8227AEE8` (network-mode.md, ours), `sub_8227E858` (unmapped, unported) ....
- **sub_82133B10**-82138520: 14 functions, 1452 lines, 0 seen, unblocks 1; called from `sub_82135008` (post-effects.md, ours), `sub_821351A8` (post-effects.md, ours), `sub_82135AA8` (post-effects.md, ours), `sub_82136130` (post-effects.md, ours) ....
- **sub_82136F50**-82136FA0: 11 functions, 88 lines, 0 seen, unblocks 0; called from `sub_82136E30` (post-effects.md, ours), `sub_82138610` (post-effects.md, ours).
- **sub_822A2390**-822A2420: 10 functions, 60 lines, 0 seen, unblocks 0; called from `sub_822ACB90` (device-state.md, ours).
- **sub_820AA010**-820AA198: 9 functions, 145 lines, 0 seen, unblocks 0; called from `sub_820A9E28` (cameras.md, ours).
- **sub_82290AB8**-822910F0: 9 functions, 665 lines, 0 seen, unblocks 7; called from `sub_8209E540` (unmapped, unported), `sub_8209E610` (unmapped, unported), `sub_8209E698` (unmapped, unported), `sub_8209E730` (unmapped, unported) ....
- **sub_825B0068**-825B0A40: 9 functions, 934 lines, 0 seen, unblocks 0; called from `sub_825A8A50` (c-runtime.md, ours), `sub_825AAD20` (c-runtime.md, ours), `sub_825AB650` (unmapped, excluded), `sub_825AB888` (unmapped, excluded) ....
- **sub_82271D50**-822726C8: 8 functions, 670 lines, 0 seen, unblocks 3; called from `sub_82274B60` (unmapped, unported), `sub_82276510` (network-mode.md, ours).
- **sub_82287CE0**-82288408: 8 functions, 397 lines, 0 seen, unblocks 2; called from `sub_82279610` (network-mode.md, ours), `sub_82288438` (unmapped, unported), `sub_82288550` (unmapped, unported).
- **sub_82277178**-82277928: 7 functions, 628 lines, 0 seen, unblocks 2; called from `sub_82277950` (unmapped, unported), `sub_822785E0` (network-mode.md, unported), `sub_82279B08` (network-mode.md, ours).
- **sub_82297870**-82297B98: 7 functions, 250 lines, 5 seen, unblocks 270; called from `xstart` (unmapped, unported), `sub_825AA708` (unmapped, unported), `sub_825AD070` (unmapped, unported), `sub_825AFD10` (unmapped, unported) ....
- **sub_8258AB18**-8258B368: 6 functions, 850 lines, 0 seen, unblocks 4; called from `sub_820EF468` (accessories.md, unported), `sub_8258BCD8` (unmapped, unported).
- **sub_8227F9D8**-8227FC08: 5 functions, 229 lines, 0 seen, unblocks 8; called from `sub_822800C0` (unmapped, unported), `sub_82281858` (network-mode.md, ours), `sub_82282BC8` (unmapped, unported).
- **sub_82591898**-825920A8: 5 functions, 104 lines, 0 seen, unblocks 7; called from `sub_82098F30` (unmapped, unported), `sub_820A7720` (unmapped, unported), `sub_82591B38` (input.md, ours), `sub_82591B68` (input.md, ours).
- **sub_8259C6F0**-8259C910: 5 functions, 133 lines, 0 seen, unblocks 6; called from `sub_8210AE60` (action-mappers.md, ours), `sub_8210AEA0` (unmapped, unported), `sub_8210B090` (unmapped, unported), `sub_8259C6D0` (action-mappers.md, ours) ....
- **sub_820A6420**-820A9CF8: 4 functions, 404 lines, 0 seen, unblocks 13; called from `sub_820A7218` (unmapped, unported), `sub_820A7C50` (unmapped, unported), `sub_820A9E28` (cameras.md, ours), `sub_820AA088` (unmapped, unported) ....
- **sub_820BE4F0**-820BEBB0: 4 functions, 604 lines, 0 seen, unblocks 0; called from `sub_820BCE80` (collision.md, ours).
- **sub_820F6C88**-820F7EE8: 4 functions, 410 lines, 0 seen, unblocks 0; called from `sub_820F8090` (collision-world.md, ours).
- **sub_82216850**-82216A40: 4 functions, 190 lines, 0 seen, unblocks 0; called from `sub_8216C210` (race-hud-machine.md, ours), `sub_8216C530` (race-hud-machine.md, ours), `sub_8216CB98` (race-hud-machine.md, ours), `sub_8216D918` (race-hud-machine.md, ours) ....
- **sub_82282630**-822827F0: 4 functions, 161 lines, 0 seen, unblocks 0; called from `sub_822873A0` (network-mode.md, ours).
- **sub_825A6948**-825A6B18: 4 functions, 129 lines, 0 seen, unblocks 78; called from `sub_82091200` (unmapped, unported), `sub_82091260` (unmapped, unported), `sub_82091398` (unmapped, unported), `sub_82091540` (std-string.md, ours) ....
- **sub_825DB428**-825DB498: 4 functions, 49 lines, 0 seen, unblocks 6; called from `sub_825DABC8` (unmapped, unported), `sub_825DAC00` (unmapped, unported), `sub_825DAC10` (unmapped, unported), `sub_825DAC48` (unmapped, unported).
- **sub_82098F30**-82099048: 3 functions, 152 lines, 0 seen, unblocks 0; called from `sub_820991E0` (input.md, ours).
- **sub_820A44C0**-820A5660: 3 functions, 115 lines, 0 seen, unblocks 0; called from `sub_820A56E8` (save-interface.md, ours), `sub_820A6160` (save-interface.md, ours).
- **sub_820AE198**-820AE2F8: 3 functions, 60 lines, 0 seen, unblocks 0; called from `sub_820AE348` (app-states.md, ours).
- **sub_820B0098**-820B00B8: 3 functions, 18 lines, 0 seen, unblocks 0; called from `sub_820BFD30` (vehicle.md, ours).
- **sub_820B0340**-820B0428: 3 functions, 126 lines, 0 seen, unblocks 0; called from `sub_820B2100` (car-physics.md, ours), `sub_820BFD30` (vehicle.md, ours).
- **sub_820CC480**-820CC778: 3 functions, 447 lines, 0 seen, unblocks 2; called from `sub_82115550` (career.md, unported), `sub_82115708` (career.md, unported).
- **sub_820FF5A0**-820FF680: 3 functions, 119 lines, 0 seen, unblocks 6; called from `sub_820FF790` (race-states.md, unported), `sub_82214C88` (race-hud-object.md, unported).
- ... and 265 smaller ones (323 functions).

## Extra roots

Seen in play but not reachable from `main` (static edges miss them, or they run outside `main`: start-up, threads, callbacks), not under another extra root:

`xstart` (unported)
