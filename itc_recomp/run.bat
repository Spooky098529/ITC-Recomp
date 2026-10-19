@echo off
rem Launch the recompiled Import Tuner Challenge (Windows).
rem
rem Put this script, itc.exe, rexruntime.dll and SDL3.dll in the game folder
rem next to default.xex, then run run.bat. Extra options are passed on.
rem
rem The game draws with its own Vulkan renderer (the native renderer, in its
rem exclusive mode), not an emulated Xbox 360 GPU, and plays sound through
rem its own audio output (--native_audio), not the SDK's.
rem
rem   run.bat                     frame interpolation on (the default)
rem   run.bat --no-interpolation  the original 30 fps
rem   run.bat --width=1920 --height=1080   a bigger window, drawn sharper
rem   run.bat --fullscreen        fill the screen, drawn at its resolution
rem
rem Logging is off; run-debug.bat runs the same with logs (in logs\), for
rem reporting problems.
rem
rem Frame interpolation (experimental): game logic stays at 30 Hz and races
rem draw up to --max_fps (144 here; pass --max_fps=N to change it, 0 for
rem unlimited), each frame blended between the last two ticks. Menus stay at
rem 30 fps. Motion blur and glow are always drawn.
rem
rem Resolution (none of these is on by default: out of the box the game runs
rem in a 1280x720 window at its own resolution, its 3D view drawn at 1024x576
rem and upscaled, as on the console):
rem   --width=W --height=H  the window's size in pixels; the game is drawn at
rem                         the largest 16:9 resolution that fits it. Give
rem                         one alone and the other follows at 16:9.
rem   --fullscreen          fill the screen (borderless, no display mode
rem                         change), drawn at the screen's resolution; with
rem                         --width/--height, drawn at that resolution and
rem                         scaled to the screen (e.g. to go easier on a weak
rem                         GPU).
rem At any size but 1280x720 the 3D view is drawn at the full resolution too,
rem not upscaled. Screens and windows that aren't 16:9 get black bars. Higher
rem resolutions cost GPU time only.
rem
rem The renderer's copies of guest memory are updated once a frame instead of
rem checked draw by draw (--native_renderer_mirror_by_tick and
rem --native_renderer_mirror_to_both; pass
rem --native_renderer_mirror_by_tick=false for the old way).
rem
rem The defaults are set as environment variables (REX_<OPTION>), which the
rem game reads after its command line: an option given on the command line
rem overrides its default. (The game rejects an option given twice.)
setlocal
set "dir=%~dp0"
set "dir=%dir:~0,-1%"

set "REX_NATIVE_RENDERER=true"
set "REX_NATIVE_RENDERER_EXCLUSIVE=true"
set "REX_NATIVE_AUDIO=true"
set "REX_GAME_DATA_ROOT=%dir%"
set "REX_NATIVE_RENDERER_MIRROR_BY_TICK=true"
set "REX_NATIVE_RENDERER_MIRROR_TO_BOTH=true"
set "REX_FULLSCREEN=false"
set "REX_LOG_LEVEL=off"
set "REX_INTERPOLATE_DRAW_STATE=true"
set "REX_MAX_FPS=144"
for %%a in (%*) do (
  if /i "%%~a"=="--no-interpolation" (
    set "REX_INTERPOLATE_DRAW_STATE=false"
    set "REX_MAX_FPS=30"
  )
)

"%dir%\itc.exe" %*
