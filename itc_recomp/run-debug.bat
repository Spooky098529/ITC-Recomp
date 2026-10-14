@echo off
rem run.bat with logging on (written to logs\), for reporting problems.
"%~dp0run.bat" --log_level=info --log_fps=true --log_frame_times=true %*
