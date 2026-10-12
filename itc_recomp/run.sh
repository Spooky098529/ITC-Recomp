#!/bin/bash
# Launch the recompiled Import Tuner Challenge.
#
# Put this script, itc, librexruntime.so and librexgpu-xenos.so in the game
# folder next to default.xex, then run ./run.sh. Extra flags are passed on.
dir="$(cd "$(dirname "$0")" && pwd)"

export LD_LIBRARY_PATH="$dir${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Frame interpolation (experimental): game logic stays at 30 Hz and races
# draw up to --max_fps, each frame blended between the last two ticks. Turn
# it off with --interpolate_draw_state=false, or use --max_fps=30 for the
# original 30 fps. Motion blur is off because it smears a whole tick of
# motion at high frame rates.
exec "$dir/itc" \
  --gpu_plugin=xenos \
  --game_data_root="$dir" \
  --mnk_mode=true \
  --interpolate_draw_state=true \
  --max_fps=144 \
  --motion_blur=false \
  --log_fps=true \
  "$@"
