#!/bin/bash
# Launch the recompiled Import Tuner Challenge.
#
# Put this script, itc, librexruntime.so and librexgpu-xenos.so in the game
# folder next to default.xex, then run ./run.sh. Extra flags are passed on.
#
#   ./run.sh                     frame interpolation on (the default)
#   ./run.sh --no-interpolation  the original 30 fps, with motion blur
#
# Frame interpolation (experimental): game logic stays at 30 Hz and races
# draw up to --max_fps (144 here; pass --max_fps=N to change it, 0 for
# unlimited), each frame blended between the last two ticks. Menus stay at
# 30 fps. Motion blur is off with it, because it smears a whole tick of
# motion at high frame rates.
dir="$(cd "$(dirname "$0")" && pwd)"

export LD_LIBRARY_PATH="$dir${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

defaults=(--interpolate_draw_state=true --max_fps=144 --motion_blur=false)
args=()
for arg in "$@"; do
  if [ "$arg" = "--no-interpolation" ]; then
    defaults=(--interpolate_draw_state=false --max_fps=30 --motion_blur=true)
  else
    args+=("$arg")
  fi
done

# The flag parser rejects a flag given twice, so a default is left out when
# the same flag is passed in.
flags=(--gpu_plugin=xenos --game_data_root="$dir" --mnk_mode=true "${defaults[@]}" --log_fps=true)
final=()
for flag in "${flags[@]}"; do
  name="${flag%%=*}"
  for arg in "${args[@]}"; do
    if [ "${arg%%=*}" = "$name" ] || [ "$arg" = "--no-${name#--}" ]; then
      continue 2
    fi
  done
  final+=("$flag")
done

exec "$dir/itc" "${final[@]}" "${args[@]}"
