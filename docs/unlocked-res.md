# Unlocked resolution

Drawing at resolutions other than the game's own 1280x720, on the branch
`unlocked-resolution` (2026-10-16). This is a first pass: it works and the
user is happy with it, but it keeps the game's own way of building the
frame, including its upscale draw (see "Later: a more native approach").
The details of how it was built and checked are in "Resolution" at the
end of `native-renderer.md`; this file is the summary and what's next.

## What it does

- `--width`/`--height`: the window's size in real pixels (one alone keeps
  16:9; neither gives 1280x720). The window is high-DPI
  (`SDL_WINDOW_HIGH_PIXEL_DENSITY`), so desktop scaling doesn't shrink it.
- `--fullscreen` (the SDK's flag, default true; `run.sh` and the test
  command in `CLAUDE.md` pass `--fullscreen=false`): borderless at the
  desktop's resolution. With `--width`/`--height` the game draws at that
  resolution and the frame is scaled to the screen.
- Screens and windows that aren't 16:9 get black bars (`FitOffsets` in
  `renderer.cpp`). The swapchain is made again when the window's size
  changes.
- The drawing scale is the largest that fits the window's pixels at start
  (`ResolutionScale` in `renderer.cpp`); it doesn't follow later resizes.

## How

Everything is in our renderer; shaders are translated unscaled and the
SDK isn't patched. That relies on no shader reading the pixel position or
fetching in texels, or writing memory (checked by the "analyzed ... shader"
log lines in `shaders.cpp`: none in boot, menus, attract races or the
cockpit view).

- Each target is drawn at its guest size times a scale
  (`Target::scale`); viewports, scissors, resolve rectangles and the depth
  bias slope are scaled by it.
- The 3D view (1024x576 in the game, 80% of the frame) gets 1.25 times the
  frame's scale at any size other than the default, so it's drawn at the
  frame's resolution and the game's upscale draw becomes a one-to-one
  copy. At the default size it's the console's 1024x576, upscaled.
- Resolve destinations are made at the scale of the target resolved into
  them, one image per scale (the upscale's source `1ECB9000` is also where
  the menus resolve their frame, at the frame's scale); draws sample the
  one written last.

## Checked

- Attract races at 1920x1080 (dumped frames right, including the cockpit
  view with the rear-view mirror) and at the default size (unchanged, the
  144 cap held).
- The user: fullscreen at 3x on a 4K screen at 145% desktop scaling (a
  race, 150 fps at the cap, before the 3D view was drawn at full size),
  and a 1400x1050 window (black bars above and below, resizing and
  maximizing).
- The user: fullscreen at 4K with the 3D view at full size (2026-10-16,
  `itc_795.log`): 3D view and frame 3840x2160, ~3 minutes of racing at
  150-151 fps at the 144 cap (two seconds at 138-139), the render thread at
  ~48-57% of a core (waiting on the cap); "looks sick".

## Still open

- The bloom and motion blur at high scales, judged while driving (nothing
  wrong seen yet).
- The Steam Deck (fullscreen on its 1280x800 screen: 1x with bars).
- The drawing scale following a resized window; toggling fullscreen while
  running.
- The README and release notes for players.

## Later: a more native approach

The game builds its frame in two images: the 3D view (HDR, 2x MSAA, where
the post-processing runs) and the 1280x720 frame (8-bit, where the HUD is
drawn). Its "upscale" draw (the first draw of the last pass, after the
last resolve; see "The end of a frame" in `gpu-functions.md`) samples the
3D view's resolved copy across the frame, and the HUD goes on top. At
custom sizes this pass makes it a one-to-one copy, but the frame is still
built the game's way: a resolve of the 3D view into `1ECB9000`, then a
full-screen draw.

To investigate: skipping the game's upscale step altogether and
composing the frame natively, e.g. drawing the 3D view straight into the
frame's image, or resolving it into the frame at the right place, and
drawing the HUD over it. First find out:

- What the upscale draw's pixel shader does besides sampling (tone
  mapping or gamma going from HDR to 8-bit?). Identify it in a dump's
  `draws.txt` (`--native_renderer_dump`) and read its microcode with
  `--native_renderer_log_shader_ucode=<hash>`. If it changes colours,
  whatever replaces it must do the same.
- Whether anything else reads `1ECB9000` or the frame between the upscale
  and the HUD, in races and in menus (which resolve their own frame into
  the same address).
- What it would gain: at 4K the copy is one full-screen draw, likely well
  under a millisecond, so the case for it is a simpler frame and the
  resolve and its second image per scale gone, not speed. Measure before
  and after.
