#!/bin/bash
# Plays a pad recording by tick (--pad_ticks_play, docs/pad-replay.md "By
# tick") with the state digest on (--state_digest), stops a few seconds after
# the recording ends, and compares the races' digests with a baseline: the
# same input on the same ticks, so the same game gives the same digest, tick
# for tick. Prints the log's path, error lines, and per race the first tick
# that differs (none: the runs match).
#
#   itc_recomp/tools/tick_replay.sh [ticks file] [baseline digest] [extra flags]
#
# Defaults: itc_recomp/pad_recordings/race_test.ticks and .digest. The run's
# digest is written next to the log (<log>.digest), to keep as a new
# baseline when a change is meant to change the game. Run from the repo
# root, after building; no virtual pad (unlike scripted_run.sh).
set -u
TICKS=${1:-itc_recomp/pad_recordings/race_test.ticks}; shift || true
BASELINE=${1:-itc_recomp/pad_recordings/race_test.digest}; shift || true
B=itc_recomp/out/build/linux-amd64-release; G="$PWD/itc/Import Tuner Challenge (USA)"

newest_log() { ls -t $B/logs/itc_*.log 2>/dev/null | head -1; }
before=$(newest_log)
SDL_GAMECONTROLLER_IGNORE_DEVICES=0x2E24/0x072F XDG_DATA_DIRS=/usr/local/share:/usr/share \
  timeout 300 "$B/itc" \
  --native_audio=true \
  --game_data_root="$G" --max_fps=144 --fullscreen=false --log_fps=true \
  --interpolate_draw_state=true --log_level=info --skip_intro \
  --state_digest=true --pad_ticks_play="$TICKS" "$@" >/dev/null 2>&1 &
GAME=$!
L=""
while kill -0 $GAME 2>/dev/null; do
  sleep 1
  if [ -z "$L" ] && [ "$(newest_log)" != "$before" ]; then L=$(newest_log); fi
  if [ -n "$L" ] && grep -q "pad ticks: the recording ended" "$L"; then
    sleep 3
    break
  fi
done
kill $GAME 2>/dev/null; wait $GAME 2>/dev/null
find /dev/shm -maxdepth 1 -user "$USER" -name 'xenia_memory_*' -delete
[ -z "$L" ] && L=$(newest_log)
echo "log: $L"
grep "state digest" "$L" | sed 's/.*state digest: //' > "$L.digest"
echo "error lines: $(grep -c '\[error\]\|\[critical\]' "$L")"
grep "pad ticks: race" "$L" | sed 's/.*pad ticks: /  /'
python3 -I - "$BASELINE" "$L.digest" <<'EOF'
import sys
def races(path):
    out = []
    for line in open(path):
        f = line.split()
        if len(f) < 8:
            continue
        if f[1] == "0" or not out:
            out.append([])
        out[-1].append(f)
    return out
base, run = races(sys.argv[1]), races(sys.argv[2])
if len(base) != len(run):
    print(f"races: {len(run)} in this run, {len(base)} in the baseline")
for i, (a, b) in enumerate(zip(base, run)):
    # Fields: tick N racers N hash H pad P player x y z.
    d = next((k for k, (p, q) in enumerate(zip(a, b)) if p[3:8] != q[3:8]), None)
    if d is None and len(a) != len(b):
        d = min(len(a), len(b))
    if d is None:
        print(f"race {i}: {len(b)} ticks, all match")
    else:
        print(f"race {i}: first differing tick {d} of {len(a)}")
        for k in range(max(0, d - 1), min(d + 2, len(a), len(b))):
            print("  baseline: " + " ".join(a[k]))
            print("  this run: " + " ".join(b[k]))
EOF
