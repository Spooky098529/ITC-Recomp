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
#   ./run.sh --width=1920 --height=1080   a bigger window, drawn sharper
#   ./run.sh --fullscreen        fill the screen, drawn at its resolution
#   ./run.sh --motion_blur=false  no motion blur
#
# Logging is off; ./run-debug.sh runs the same with logs (in logs/), for
# reporting problems.
#
# Frame interpolation (experimental): game logic stays at 30 Hz and races
# draw up to --max_fps (144 here; pass --max_fps=N to change it, 0 for
# unlimited), each frame blended between the last two ticks. Menus stay at
# 30 fps.
#
# Resolution (none of these is on by default: out of the box the game runs
# in a 1280x720 window at its own resolution, its 3D view drawn at 1024x576
# and upscaled, as on the console):
#   --width=W --height=H  the window's size in pixels; the game is drawn at
#                         the largest 16:9 resolution that fits it. Give
#                         one alone and the other follows at 16:9.
#   --fullscreen          fill the screen (borderless, no display mode
#                         change), drawn at the screen's resolution; with
#                         --width/--height, drawn at that resolution and
#                         scaled to the screen (e.g. to go easier on a weak
#                         GPU).
# At any size but 1280x720 the 3D view is drawn at the full resolution too,
# not upscaled. Screens and windows that aren't 16:9 get black bars. Higher
# resolutions cost GPU time only.
#
# Motion blur (on by default): the game's blur of the picture at speed.
#   --motion_blur=false   leave it out, for a sharp picture. The glow
#                         around lights is always drawn.
#
# Xbox 360 gamma (off by default): the console sends its picture through a
# gamma table made for the kind of display it's connected to, so on a TV it
# looks darker than here.
#   --xbox_gamma=off    no change, as made for a PC monitor (the default)
#   --xbox_gamma=hdtv   as the 360 sends it to an HDTV: darker mid-tones and
#                       shadows
#   --xbox_gamma=power  the 360's table for a display with a set gamma
#                       power: lighter shadows
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
flags=(--native_renderer=true --native_renderer_exclusive=true --native_audio=true --game_data_root="$dir" --native_renderer_mirror_by_tick=true --native_renderer_mirror_to_both=true --fullscreen=false --xbox_gamma=off "${defaults[@]}" --log_level=off)
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
