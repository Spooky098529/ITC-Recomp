#!/bin/bash
# Runs the game with the standard flags and replays a recorded gamepad
# (pad_record.py) into it through a virtual pad (pad_play.py), so the game
# can be driven past the title screen without the user: menus, a race,
# saves. Prints the log's path, how many seconds ran at 30 ticks/s, errors
# in the log, and any harness results, as attract_race.sh does. See
# docs/pad-replay.md.
#
#   itc_recomp/tools/scripted_run.sh <recording> [extra flags]
#
# Run from the repo root, after building. The replay starts PAD_DELAY
# seconds (3 by default) after the boot's first draw ("called from
# 8248466C"), with --skip_intro, so at the title screen, and the game is
# closed 5 s after the last input (or, if it hangs, 2 minutes after the
# recording's length from launch). If the boot stalls
# (no such line in 30 s), the game is restarted once. The user's own pad
# can stay plugged in: SDL is told to ignore it (REAL_PAD, its USB ids), so
# the virtual pad is the game's only pad. Extra flags go on the game's
# command line; a flag given here can't be given again.
set -u
RECORDING=${1:?usage: scripted_run.sh <recording> [extra flags]}; shift
PAD_DELAY=${PAD_DELAY:-3}
REAL_PAD=${REAL_PAD:-0x2E24/0x072F}
B=itc_recomp/out/build/linux-amd64-release; G="$PWD/itc/Import Tuner Challenge (USA)"
TOOLS=$(dirname "$0")
# A limit in case the game hangs: the recording's length plus 2 minutes.
SECONDS_TO_RUN=$(python3 -I -c 'import json, sys
lines = open(sys.argv[1]).read().splitlines()
print(int(json.loads(lines[-1]).get("t", 0)) + 120)' "$RECORDING")

newest_log() { ls -t $B/logs/itc_*.log 2>/dev/null | head -1; }

# Starts the game in the background; sets GAME (its pid) and L (its log), or
# returns 1 if the boot doesn't reach its first draw in 30 s.
start_game() {
  local before; before=$(newest_log)
  SDL_GAMECONTROLLER_IGNORE_DEVICES=$REAL_PAD SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS=1 \
  XDG_DATA_DIRS=/usr/local/share:/usr/share LD_LIBRARY_PATH=$B:rexglue-sdk/out/linux-amd64 \
    timeout "$SECONDS_TO_RUN" "$B/itc" \
    --native_audio=true --native_renderer_mirror_by_tick=true --native_renderer_mirror_to_both=true \
    --game_data_root="$G" --max_fps=144 --fullscreen=false --log_fps=true \
    --interpolate_draw_state=true --log_level=info --skip_intro "$@" \
    >/dev/null 2>&1 &
  GAME=$!
  L=""
  for _ in $(seq 300); do
    sleep 0.1
    if [ -z "$L" ] && [ "$(newest_log)" != "$before" ]; then L=$(newest_log); fi
    if [ -n "$L" ] && grep -q 'called from 8248466C' "$L"; then return 0; fi
    kill -0 $GAME 2>/dev/null || return 1
  done
  return 1
}

if ! start_game "$@"; then
  echo "boot stalled (no \"called from 8248466C\" in 30 s); restarting once"
  kill $GAME 2>/dev/null; wait $GAME 2>/dev/null
  find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
  if ! start_game "$@"; then
    echo "boot stalled again; giving up"
    kill $GAME 2>/dev/null; wait $GAME 2>/dev/null
    find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
    exit 1
  fi
fi
sleep "$PAD_DELAY"
python3 "$TOOLS/pad_play.py" "$RECORDING" --exit-after 5 &
PAD=$!
# Whichever ends first: the replay (then the game is closed) or the game.
while kill -0 $PAD 2>/dev/null && kill -0 $GAME 2>/dev/null; do sleep 0.2; done
if kill -0 $GAME 2>/dev/null; then
  kill $GAME; wait $GAME
  ending="closed 5 s after the last input"
else
  wait $GAME
  status=$?
  ending="the game ended first, exit $status (124: the hang limit, ${SECONDS_TO_RUN} s)"
  kill $PAD 2>/dev/null
fi
wait $PAD 2>/dev/null
find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
echo "log: $L"
echo "end: $ending"
# A second can log 31 ticks (the tick clock against the log's seconds).
echo "seconds at 30 ticks/s: $(grep -cE 'ticks/s 3[01]\b' "$L")"
echo "error lines: $(grep -ciE '\berror\b' "$L")"
grep 'harness:' "$L" | grep -v DIFFERS | sed 's/^.*harness: /harness: /'
