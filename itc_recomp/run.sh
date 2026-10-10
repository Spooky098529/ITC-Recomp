#!/bin/bash
# Launch the recompiled Import Tuner Challenge.
cd "$(dirname "$0")"

# VS Code (AppImage) overrides XDG_DATA_DIRS, which hides the system Vulkan
# drivers in /usr/share/vulkan/icd.d. Restore the default search path.
export XDG_DATA_DIRS=/usr/local/share:/usr/share

# The SDK builds librexruntime.so into its own output dir, not next to the game.
export LD_LIBRARY_PATH=../rexglue-sdk/out/linux-amd64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}

exec ./out/build/linux-amd64-release/itc \
  --gpu_plugin=xenos \
  --game_data_root="../itc/Import Tuner Challenge (USA)" \
  "$@"
