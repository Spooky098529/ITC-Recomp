# B7 leads

Subsystems for the B7 loop to take (`b7-loop.md`). One lead a line: the
functions to start from, why, and who found it. The loop takes them from
the top and deletes a lead's line when it picks it; mapping agents add
new ones at the end, after checking the function isn't already in a file
in `docs/functions/` or listed here.

- `sub_82126220`, `sub_82126158`, `sub_82123450`, `sub_82123A80`, `sub_82123E48`, `sub_82124728`: the race overlays' update side, the rest of `82121000`-`8212EF00` (~120 functions seen in play: creating the overlays, their vtable slot 4 updates, small accessors such as `sub_82126550` and `sub_82126498`), whose recordings are in `functions/sprites.md`; with `sub_8215DEA0` (`RaceOverlays::Run`, in `functions.md`). Found by the B7 loop's mapping agent (sprites), 2026-10-22.
- `sub_825A6940`, `sub_825A71C0` (and `sub_825A70E8`): the C runtime heap, probably `malloc` (or operator new) and `free` (and operator delete), seen in play; `CommandList::Init` (`sub_8258CD28`, `functions/command-list.md`) and `sub_82090CC0` call them. Made native, they allocate guest memory without `ctx`: the user decides how first. Found by the B7 loop's mapping agent (command list), 2026-10-22.
