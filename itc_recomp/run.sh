#!/bin/bash
# Launch the recompiled Import Tuner Challenge.
#
# Put this script, itc and librexruntime.so in the game folder next to
# default.xex, then run ./run.sh. Extra flags are passed on.
#
# The game draws with its own Vulkan renderer (the native renderer, in its
# exclusive mode), not an emulated Xbox 360 GPU, and plays sound through
# its own audio output (--native_audio), not the SDK's.
#
#   ./run.sh                     frame interpolation on (the default)
#   ./run.sh --no-interpolation  the original 30 fps
#
# Logging is off; ./run-debug.sh runs the same with logs (in logs/), for
# reporting problems.
#
# Frame interpolation (experimental): game logic stays at 30 Hz and races
# draw up to --max_fps (144 here; pass --max_fps=N to change it, 0 for
# unlimited), each frame blended between the last two ticks. Menus stay at
# 30 fps. Motion blur and glow are always drawn.
#
# The window is 1280x720 and draws at the game's own resolution. --width
# and --height size the window and draw as sharp as it allows (e.g.
# --width=1920 --height=1080); --fullscreen fills the screen at its own
# resolution (with --width/--height, draws at that resolution instead).
# Screens and windows that aren't 16:9 get black bars.
#
# The renderer's copies of guest memory are updated once a frame instead of
# checked draw by draw (--native_renderer_mirror_by_tick and
# --native_renderer_mirror_to_both; pass
# --native_renderer_mirror_by_tick=false for the old way).
dir="$(cd "$(dirname "$0")" && pwd)"

export LD_LIBRARY_PATH="$dir${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

defaults=(--interpolate_draw_state=true --max_fps=144)
args=()
for arg in "$@"; do
  if [ "$arg" = "--no-interpolation" ]; then
    defaults=(--interpolate_draw_state=false --max_fps=30)
  else
    args+=("$arg")
  fi
done

# The flag parser rejects a flag given twice, so a default is left out when
# the same flag is passed in.
flags=(--native_renderer=true --native_renderer_exclusive=true --native_audio=true --game_data_root="$dir" --mnk_mode=true --native_renderer_mirror_by_tick=true --native_renderer_mirror_to_both=true --fullscreen=false "${defaults[@]}" --log_level=off)
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
