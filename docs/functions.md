# Known functions

Names for the generated `sub_XXXXXXXX` functions, worked out so far. The
address is the function's location in the original `default.xex`, so it also
matches Ghidra or IDA. Names are our own guesses; the game shipped without
symbols.

"Rewritten" means a readable replacement lives in `itc_recomp/src/hooks/` and
overrides the generated code. "Patched" means the generated code was edited in
place (search for `PATCH:`).

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_82092358` | `SetPresentInterval` (guess) | | Passes the vblank interval (2 = 30 fps) to `sub_8258D568`. Patched to 1 on the `unlock-fps` branch. |
| `sub_8258D568` | | | Receives the frame interval from `sub_82092358`. |
| `sub_8258DAA8` | `WaitForFrame` (guess) | | Frame pacing wait. |
| `sub_820C0128` | `Vehicle::GetSpeedKmh` | Rewritten | Returns the float at vehicle+48 (m/s) times 3.6. |
| `sub_8210EDB0` | | | Wind sound; scales with speed. |
| `sub_82152CD8` | | | Camera shake; scales with speed. |
| `sub_8213F530` | | | Parent of the speed trail object (switches mode at about 88 km/h). Calls `sub_8213F2E0`, `sub_8213F140` and `sub_8213F4B0`. |
| `sub_8213D438` | | | Resets the trail history; runs on a camera switch. |
| `sub_8218E420` | | Patched | Post-race check; the comparison against 60 was changed to 1 to fix the black screen after quest races. |
