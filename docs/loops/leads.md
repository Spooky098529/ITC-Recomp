# B7 leads

Subsystems for the B7 loop to take (`b7-loop.md`). One lead a line: the
functions to start from, why, and who found it. The loop takes them from
the top and deletes a lead's line when it picks it; mapping agents add
new ones at the end, after checking the function isn't already in a file
in `docs/functions/` or listed here.

- `sub_8258C9D0`, `sub_8258CA50`, `sub_8258CB08`, `sub_8258CB98`, `sub_8258CC30`: the graphics library's command list (open and close a chunk, vertex and constant payloads; `sub_8258CAA0` is in `functions.md`), called by every recorder (~40 callers seen); they block every recorder in `functions/sprites.md`. Found by the B7 loop's mapping agent (sprites), 2026-10-22.
- `sub_825A9934`, `sub_825A8170` (→ `sub_825A7FB0`): C runtime helpers seen in play, the stack probe for large frames (9 callers seen in play) and probably `acos`; they block `sub_82129A08`, the four particle-like overlays and `sub_821266C8` in `functions/sprites.md`. Found by the B7 loop's mapping agent (sprites), 2026-10-22.
- `sub_82126220`, `sub_82126158`, `sub_82123450`, `sub_82123A80`, `sub_82123E48`, `sub_82124728`: the race overlays' update side, the rest of `82121000`-`8212EF00` (~120 functions seen in play: creating the overlays, their vtable slot 4 updates, small accessors such as `sub_82126550` and `sub_82126498`), whose recordings are in `functions/sprites.md`; with `sub_8215DEA0` (`RaceOverlays::Run`, in `functions.md`). Found by the B7 loop's mapping agent (sprites), 2026-10-22.
