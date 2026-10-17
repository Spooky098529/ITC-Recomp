name: pause-menu-polling-issue
description: Pause menu polls input outside render-only pass split, causing fast repeat in races
metadata:
  type: feedback

The pause menu polls input **outside** the normal draw step split (see [[frame-pacing-split]]), so during render-only passes (races) it sees controller presses multiple times per tick. The input manager update (sub_820990F0) is skipped on render-only passes to prevent this, but the pause menu handles input differently.

**Root cause**: In `frame_pacing/game_messages.cpp`, sub_820990F0 (input manager update via XInputGetState) returns early on render-only passes. However, the pause menu's input handling bypasses this check or polls input through a different path that doesn't respect the render-only pass flag.

**Fix**: The pause menu needs to either:
1. Respect the `GameMessages_IsRenderOnlyPass()` flag when polling input, OR
2. Have its input polling wrapped in a similar guard that skips on render-only passes

The pause menu state is likely managed in the game's UI subsystem, and its input handler needs to check the render-only pass flag before processing controller input.

**Related**: [[frame-pacing-split]]
