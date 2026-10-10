#!/bin/bash
# Launch the recompiled Import Tuner Challenge.
cd "$(dirname "$0")"

# VS Code (AppImage) overrides XDG_DATA_DIRS, which hides the system Vulkan
# drivers in /usr/share/vulkan/icd.d. Restore the default search path.
export XDG_DATA_DIRS=/usr/local/share:/usr/share

# The SDK builds librexruntime.so into its own output dir, not next to the game.
export LD_LIBRARY_PATH=../rexglue-sdk/out/linux-amd64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}

# direct_host_resolve=false: the SDK's direct resolve path leaves the motion
# blur's afterimage texture stale, so blur is weak and light trails from the
# previous camera stay on screen after switching view.

exec ./out/build/linux-amd64-release/itc \
  --gpu_plugin=xenos \
  --game_data_root="../itc/Import Tuner Challenge (USA)" \
  --mnk_mode=true \
  --direct_host_resolve=false \
  "$@"
