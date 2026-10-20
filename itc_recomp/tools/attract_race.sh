#!/bin/bash
# Runs an attract race (AI cars, no input) for a while with the standard
# flags, the way to check the game still works without the user, and prints
# the log's path, how many seconds ran at 30 ticks/s (the race starts ~2 s
# after launch), errors in the log, and any harness results.
#
#   itc_recomp/tools/attract_race.sh <seconds> [extra flags]
#
# Run from the repo root, after building. Extra flags go on the game's
# command line, e.g. --run_harnesses=all (see "Checking with a harness" in
# docs/rexglue-removal.md) or --native_renderer_dump=<dir>. A flag the
# standard ones already give can't be given again (the game rejects a flag
# given twice).
set -u
SECONDS_TO_RUN=${1:?usage: attract_race.sh <seconds> [extra flags]}; shift
B=itc_recomp/out/build/linux-amd64-release; G="$PWD/itc/Import Tuner Challenge (USA)"
XDG_DATA_DIRS=/usr/local/share:/usr/share LD_LIBRARY_PATH=$B:rexglue-sdk/out/linux-amd64 \
  timeout "$SECONDS_TO_RUN" "$B/itc" \
  --native_audio=true --native_renderer_mirror_by_tick=true --native_renderer_mirror_to_both=true \
  --game_data_root="$G" --max_fps=144 --fullscreen=false --log_fps=true \
  --interpolate_draw_state=true --log_level=info --skip_intro --attract_after_s=1 "$@" \
  >/dev/null 2>&1
status=$?
find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
L=$(ls -t $B/logs/itc_*.log | head -1)
echo "log: $L"
echo "exit: $status (124: stopped by the timeout, as expected)"
echo "seconds at 30 ticks/s: $(grep -c 'ticks/s 30' "$L")"
echo "error lines: $(grep -ciE '\berror\b' "$L")"
grep 'harness:' "$L" | grep -v DIFFERS | sed 's/^.*harness: /harness: /'
