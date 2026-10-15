#!/bin/bash
# Profiles the render thread in an uncapped attract race and reports cycles
# per frame: the way to judge changes of a few percent, which fps runs can't
# show (see "Measuring small changes" in docs/native-renderer.md).
#
#   itc_recomp/tools/profile_render_thread.sh <dir holding itc> <name> [symbol pattern]
#
# Run from the repo root. Launches <dir>/itc (to compare builds, copy each
# into its own directory with the build's cache/; logs go to <dir>/logs),
# records the busiest thread 14-26 s after launch with
# `perf record -e cycles:u`, and counts the frames drawn meanwhile from the
# --log_fps lines. Prints the thread's million cycles a frame and, for each
# symbol matching the pattern (an extended regex, default
# BlendState_ReplayConstants), its share and thousand cycles a frame. The
# profile is kept as itc_recomp/out/profiles/<name>.data for perf report.
set -e
D=$1; NAME=$2; PATTERN=${3:-BlendState_ReplayConstants}
B=itc_recomp/out/build/linux-amd64-release; G="$PWD/itc/Import Tuner Challenge (USA)"
mkdir -p itc_recomp/out/profiles; O=itc_recomp/out/profiles/$NAME.data
XDG_DATA_DIRS=/usr/local/share:/usr/share LD_LIBRARY_PATH=$B:rexglue-sdk/out/linux-amd64 \
  timeout 30 "$D/itc" --native_renderer=true --native_renderer_exclusive=true \
  --game_data_root="$G" --mnk_mode=true --max_fps=0 --log_fps=true --interpolate_draw_state=true \
  --skip_intro --attract_after_s=1 > /dev/null 2>&1 &
sleep 14
P=$(pgrep -n -x itc)
T=$(ps -L -o tid=,pcpu= -p "$P" | sort -k2 -nr | head -1 | awk '{print $1}')
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
awk -v n="$NAME" -v f="$frames" -v t="$total" \
  'BEGIN{printf "%s: %d frames, render thread %.3f Mcycles a frame\n", n, f, t/f/1e6}'
perf report -i "$O" --no-children --sort sym --stdio 2>/dev/null | grep -E "$PATTERN" | head -20 |
  awk -v f="$frames" -v t="$total" '{s=$1; sub(/%/,"",s); $1=$2=""; printf "  %6.2f%% %8.1f kcycles a frame %s\n", s, t*s/100/f/1e3, substr($0,1,90)}'
