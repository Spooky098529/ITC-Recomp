# B7 leads

Subsystems for the B7 loop to take (`b7-loop.md`). One lead a line: the
functions to start from, why, and who found it. The loop takes them from
the top and deletes a lead's line when it picks it; mapping agents add
new ones at the end, after checking the function isn't already in a file
in `docs/functions/` or listed here.

- `sub_821298C0`, `sub_8209C6E0`: the main thread's draw state, the next
  performance leads ("DrawState::Note and AfterDraw" in
  `docs/native-renderer.md`); both ready groups. Found by the user and
  Claude, 2026-10-22.
