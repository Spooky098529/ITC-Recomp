@echo off
rem Launch the recompiled Import Tuner Challenge (Windows).
rem
rem Put this script, itc.exe, rexruntime.dll and SDL3.dll in the game folder
rem next to default.xex, then run run.bat. Extra options are passed on.
rem
rem   run.bat                     frame interpolation on (the default)
rem   run.bat --no-interpolation  the original 30 fps
rem
rem Logging is off; run-debug.bat runs the same with logs (in logs\), for
rem reporting problems.
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
set "REX_MNK_MODE=true"
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
