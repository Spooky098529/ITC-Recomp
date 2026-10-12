#!/bin/bash
# Launch like run.sh, with logging on: the log goes to logs/itc_<n>.log, with
# the frame rate and where each frame's time goes, every second or two.
# Extra flags are passed on, as with run.sh.
dir="$(cd "$(dirname "$0")" && pwd)"
exec "$dir/run.sh" --log_level=info --log_fps=true --log_frame_times=true "$@"
