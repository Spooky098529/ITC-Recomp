# The title screen

The boot sequence and the attract race, from launch with no input.
Moved out of `docs/functions/functions.md` on 2026-10-20.

From launch, with no input (times as measured here):

| Time | What | Driven by |
|---|---|---|
| 0-9.5 s | Ubisoft logo movie (`UBI_LOGO.wmv`) | The boot sequence's message handlers `sub_82107B90` (start), `sub_82107C58` (wait, every tick), `sub_82107BF8` (end). The movie decodes on a worker thread (`sub_820AE5D0`) and plays at its own rate. |
| 9.5-15.7 s | Genki logo (an image) | UI animation counters, ~190 ticks. |
| 15.7-18.4 s | The title logo animating in | UI animation counters, ~80 ticks. |
| 18.4-38.4 s | Title screen, "press start" | `sub_822221A0`: idle ticks at +36 of `[0x82814E34]`, attract race at 600. |

`sub_822221A0` (the title screen's update) checks four controllers for a
button (152) and otherwise adds one to +36 of the object at `[0x82814E34]`.
At 600 ticks (20 s) it sets the state at +4 to 5 (via 8), which starts an
attract-mode race (AI cars, no input). After the race (~60 s) the game goes
back to the title screen, and the next race follows ~22 s later. The front
end's state objects are at `[0x82814E2C]`, `[0x82814E30]` and
`[0x82814E34]`, created when the movie ends.

Options for automated runs (both off by default):

- `--skip_intro`: the movie step reports the movie finished at once
  (`sub_820A4450` returns 1 inside `sub_82107C58`), and menus run without
  the 30 Hz tick wait until the first race, so the tick-counted Genki logo
  and title animation pass in a fraction of a second.
  Failing the movie file's open instead makes the game show the dirty disc
  error.
- `--attract_after_s=<n>`: the title screen's wait becomes n seconds: after
  a tick that counted, the counter jumps to 599.

With `--skip_intro --attract_after_s=1` the attract race starts about 1 s
after launch (21 s with only `--attract_after_s=1`, 39 s with neither).

How these were found: `--find_idle_timer` (`src/debug/idle_timer_scan.cpp`)
compares guest memory every 30 logic passes outside races, logs words that
step steadily like a timer (an integer by 30 or 60 ticks or about 1000 ms,
a float by about 1 s or 30 or 60), and then logs which tick handler or
message handler changes them; the counter at `01A70024` counted to 600.
`--log_boot_modes` logs the front end's state objects as they change and
each tick handler and message handler the first time it runs, which showed
the boot sequence's handlers. `--log_file_opens` and screenshots
(`spectacle -b -n -f -o <file>`) named the phases.
