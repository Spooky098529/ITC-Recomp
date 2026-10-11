#!/bin/bash
# Launch the recompiled Import Tuner Challenge.
#
# Put this script, itc, librexruntime.so and librexgpu-xenos.so in the game
# folder next to default.xex, then run ./run.sh. Extra flags are passed on.
dir="$(cd "$(dirname "$0")" && pwd)"

export LD_LIBRARY_PATH="$dir${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

exec "$dir/itc" \
  --gpu_plugin=xenos \
  --game_data_root="$dir" \
  --mnk_mode=true \
  --max_fps=144 \
  --log_fps=true \
  "$@"
