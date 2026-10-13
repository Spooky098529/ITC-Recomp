# Native renderer

A renderer that draws the game's render commands with Vulkan directly,
replacing the emulated GPU (`librexgpu-xenos.so`) and moving the project
away from the ReXGlue SDK. Why: the emulated GPU limits the frame rate
(see "Performance" in `functions.md`), and the game's command list is a
narrow boundary to draw from (see `gpu-functions.md`, which holds what's
known about it; this file is the plan and its progress).

## Decisions

- **Our own window and swapchain**, in `itc_recomp/src/native/`, not the
  SDK's presenter. SDL is used for the window: it's third-party, and the
  process has one copy of it (in `librexruntime.so`), whose event loop the
  SDK runs on the main thread. Window calls go to the main thread through
  `SDL_RunOnMainThread`; events for our window are ignored by the SDK's
  loop (it dispatches by window id).
- **Vulkan loaded at runtime** through SDL (`VK_NO_PROTOTYPES`, our own
  function table), so the build needs no Vulkan library. Headers come from
  the SDK's third-party copy of the Khronos headers.
- **Shaders: reuse the SDK's Xenos-to-SPIR-V translator** for now, to be
  replaced with our own later. This is the one part that leans on the SDK.
- **Development mode first**: with `--native_renderer`, the replay hooks
  draw natively *and* still call the graphics library, so the emulated GPU
  keeps rendering the game's own window and the two can be compared side by
  side; the game stays playable throughout. Input goes to the game's
  window, not ours. An exclusive mode, which stops calling the library,
  comes at milestone 5.

## Milestones

| # | Milestone | Done when | Status |
|---|---|---|---|
| 1 | **Skeleton**: `--native_renderer` creates our own Vulkan device, window and swapchain; the replay hooks pass each frame's commands to it. | A window clears and presents every frame. | Done (2026-10-13): presents every frame in menus (30 fps) and races (113-128 fps, ~26,800 commands and ~2,900 draws a frame), the game unaffected. |
| 2 | **Main view geometry**: vertex and index buffers read from guest memory, shaders translated, render states mapped to pipelines. | Race geometry is visible in our window, untextured. | In progress |
| 3 | **Textures**: untile and upload (mostly BC1-BC3), samplers from the fetch constants. | The main view looks right without effects. | |
| 4 | **The other passes**: each pass renders into a Vulkan image of its resolve's size and format; resolves become copies. | Shadows, reflections and the mirror work. | |
| 5 | **Post-processing, HUD, present**, then exclusive mode. | A race is fully native; measure the frame rate. | |
| 6 | **Menus**. | The emulated GPU is no longer needed. | |

## Known issues

- **Resizing the window doesn't resize the swapchain** on Wayland: the
  compositor leaves the size to us and never reports the swapchain out of
  date, so the image stays 1280x720. Fix later by watching the window's
  size-change events (on the main thread) and recreating the swapchain.
- No Vulkan validation layer here yet (`--native_renderer_validation` falls
  back without it); install `vulkan-validation-layers` before drawing gets
  complicated.
