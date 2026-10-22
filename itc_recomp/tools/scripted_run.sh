#!/bin/bash
# Runs the game with the standard flags and replays a recorded gamepad
# (pad_record.py) into it through a virtual pad (pad_play.py), so the game
# can be driven past the title screen without the user: menus, a race,
# saves. Prints the log's path, how many seconds ran at 30 ticks/s, errors
# in the log, and any harness results, as attract_race.sh does. See
# docs/pad-replay.md.
#
#   itc_recomp/tools/scripted_run.sh <recording> <seconds> [extra flags]
#
# Run from the repo root, after building. The replay starts PAD_DELAY
# seconds (3 by default) after the boot's first draw ("called from
# 8248466C"), with --skip_intro, so at the title screen. If the boot stalls
# (no such line in 30 s), the game is restarted once. The user's own pad
# can stay plugged in: SDL is told to ignore it (REAL_PAD, its USB ids), so
# the virtual pad is the game's only pad. Extra flags go on the game's
# command line; a flag given here can't be given again.
set -u
RECORDING=${1:?usage: scripted_run.sh <recording> <seconds> [extra flags]}
SECONDS_TO_RUN=${2:?usage: scripted_run.sh <recording> <seconds> [extra flags]}; shift 2
PAD_DELAY=${PAD_DELAY:-3}
REAL_PAD=${REAL_PAD:-0x2E24/0x072F}
B=itc_recomp/out/build/linux-amd64-release; G="$PWD/itc/Import Tuner Challenge (USA)"
TOOLS=$(dirname "$0")

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
python3 "$TOOLS/pad_play.py" "$RECORDING" --keep &
PAD=$!
wait $GAME
status=$?
kill $PAD 2>/dev/null; wait $PAD 2>/dev/null
find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
echo "log: $L"
echo "exit: $status (124: stopped by the timeout, as expected)"
# A second can log 31 ticks (the tick clock against the log's seconds).
echo "seconds at 30 ticks/s: $(grep -cE 'ticks/s 3[01]\b' "$L")"
echo "error lines: $(grep -ciE '\berror\b' "$L")"
grep 'harness:' "$L" | grep -v DIFFERS | sed 's/^.*harness: /harness: /'
