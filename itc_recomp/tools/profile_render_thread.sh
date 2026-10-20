#!/bin/bash
# Profiles the render thread in an uncapped attract race and reports cycles
# per frame: the way to judge changes of a few percent, which fps runs can't
# show (see "Measuring small changes" in docs/native-renderer.md).
#
#   itc_recomp/tools/profile_render_thread.sh <dir holding itc> <name> [symbol pattern]
#
# Run from the repo root. Launches <dir>/itc (to compare builds, copy each
# into its own directory with the build's cache/; logs go to <dir>/logs),
# records the render thread 14-26 s after launch with
# `perf record -e cycles:u`, and counts the frames drawn meanwhile from the
# --log_fps lines. Prints the thread's million cycles a frame and, for each
# symbol matching the pattern (an extended regex, default
# BlendState_ReplayConstants), its share and thousand cycles a frame. The
# profile is kept as itc_recomp/out/profiles/<name>.data for perf report.
# ITC_ARGS adds flags to the game's command line (e.g. to compare one build
# with an option on and off); a default flag it names again is left out (the
# game rejects a flag given twice), e.g.
# ITC_ARGS=--native_renderer_mirror_by_tick=false for the old mirror.
# ITC_THREAD=main records the main thread (Main XThread, which records the
# game's command lists) instead; the figures are then that thread's.
set -e
D=$1; NAME=$2; PATTERN=${3:-BlendState_ReplayConstants}
B=itc_recomp/out/build/linux-amd64-release; G="$PWD/itc/Import Tuner Challenge (USA)"
mkdir -p itc_recomp/out/profiles; O=itc_recomp/out/profiles/$NAME.data
# As players get it: our audio, the mirrors updated once a frame, and a window, not
# fullscreen (the SDK's --fullscreen defaults to true).
defaults=(--native_audio=true
  --native_renderer_mirror_by_tick=true --native_renderer_mirror_to_both=true
  --game_data_root="$G" --max_fps=0 --log_fps=true --interpolate_draw_state=true
  --skip_intro --attract_after_s=1 --fullscreen=false)
read -ra extra <<< "${ITC_ARGS:-}"
flags=()
for flag in "${defaults[@]}"; do
  for arg in "${extra[@]}"; do
    [ "${arg%%=*}" = "${flag%%=*}" ] && continue 2
  done
  flags+=("$flag")
done
XDG_DATA_DIRS=/usr/local/share:/usr/share LD_LIBRARY_PATH=$B:rexglue-sdk/out/linux-amd64 \
  timeout 30 "$D/itc" "${flags[@]}" "${extra[@]}" > /dev/null 2>&1 &
sleep 14
P=$(pgrep -n -x itc)
# The render thread: the busiest but the main thread (its name changes from
# run to run; the main thread can be as busy since the render thread got
# lighter).
if [ "${ITC_THREAD:-}" = main ]; then
  T=$(ps -L -o tid=,comm= -p "$P" | grep "Main XThread" | awk '{print $1}'); WHAT="main thread"
else
  T=$(ps -L -o tid=,pcpu=,comm= -p "$P" | grep -v "Main XThread" | sort -k2 -nr | head -1 |
    awk '{print $1}'); WHAT="render thread"
fi
t0=$(date +%s); perf record -q -e cycles:u -t "$T" -o "$O" -- sleep 12 2>/dev/null; t1=$(date +%s)
wait || true
find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete 2>/dev/null || true
L=$(ls -t "$D"/logs/*.log | head -1)
frames=$(grep -E "\] fps " "$L" | while read -r day time rest; do
  e=$(date -d "${day#[} ${time%]}" +%s)
  if (( e > t0 && e <= t1 )); then echo "$rest" | sed -E 's/.* fps ([0-9]+) .*/\1/'; fi
done | awk '{s+=$1} END{print s+0}')
if (( frames == 0 )); then echo "$NAME: no frames counted (did the race start? see $L)"; exit 1; fi
total=$(perf report -i "$O" --stdio 2>/dev/null | grep -m1 "Event count" | awk '{print $NF}')
awk -v n="$NAME" -v f="$frames" -v t="$total" -v w="$WHAT" \
  'BEGIN{printf "%s: %d frames, %s %.3f Mcycles a frame\n", n, f, w, t/f/1e6}'
perf report -i "$O" --no-children --sort sym --stdio 2>/dev/null | grep -E "$PATTERN" | head -20 |
  awk -v f="$frames" -v t="$total" '{s=$1; sub(/%/,"",s); $1=$2=""; printf "  %6.2f%% %8.1f kcycles a frame %s\n", s, t*s/100/f/1e3, substr($0,1,90)}'
