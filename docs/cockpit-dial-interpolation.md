# Cockpit dials with interpolation

Branch `cockpit-dial-interpolation` (from `main` at `3d09aa6`, 2026-10-18).

## The problem

In cockpit view above 30 fps, the dashboard needles went wrong on fast
moves. Blending a needle's whole world matrix between ticks moved its
origin in a straight line, while the needle turns about a spindle away
from that origin, so in-between frames cut across the dial. The turbo
gauge also fell to its rest angle between ticks: Draw's per-car update
lowers its value on every pass, and only logic ticks set it again.

## What we did: native dial updates

`src/hooks/cockpit_dials.cpp` replaces the game's two dial update
functions with native versions (`--native_cockpit_dials`, on by default):

- `sub_820E42E8`: the speedometer (car + 696) and the tachometer
  (car + 700), angles from `sub_820D3740`;
- `sub_820E45A8`: the dash's other needles, angles from `sub_820D3688`.
  In mode 5 (`[dash + 4]`) its second needle is the turbo gauge
  (car + 704).

Both are called from Draw's per-car update (`sub_820DAC68`) on every pass.
A needle's world matrix is `(B * RotZ(angle)) * A * car world (car + 16)`,
with A and B its local matrices. The angle maps and the matrix math follow
the game's operations; `--check_native_cockpit_dials` compares them with
the translated code on logic passes (angles exact, poses within a float
step).

On every pass:

- The angle comes from the logic pass's gauge values, kept per car, so
  the needles step once a tick, as on the 360.
- The game draws the needle with its tick pose, on the tick's car. The
  game culls it with the tick's camera. Posed on the blended car instead,
  it was culled at speed on logic passes, behind the cockpit camera, and
  the in-between frames then lost it.
- The pose on the car blended to the frame (the car's last two ticks, by
  the frame's amount) goes to the replay beside the draw, through
  `BlendState_SetLivePose`. `blend_state.cpp` keeps that world matrix for
  the needle's product and blends only its camera, like the dash around
  it.

The needle's turn is never blended; only the car under it is, so the
needle stays on its spindle while the dial moves with the car.
`--log_blend_state` counts these live poses (unpaired, camera rejected,
left as drawn).

## TODO

The needles step at 30 Hz. Two ways to make them move smoothly:

- Blend the angles: blend the kept gauge values between the last two ticks
  in `InputsFor` and work the frame pose's angle out from them (see the
  comment in `cockpit_dials.cpp`). The turn would still be about the
  spindle, so it wouldn't cut across the dial.
- Rework the game logic to run at a higher tick rate.
