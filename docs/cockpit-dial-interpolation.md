# Cockpit dials with interpolation

Branch `cockpit-dial-interpolation` (from `main` at `3d09aa6`), started
2026-10-18. It follows `cockpit-anim-fix` (not merged), whose handover is
`docs/cockpit-anim-fix.md` on that branch.

## The problem

In cockpit view (the 3D dashboard), the dials' needles move erratically when
they have to move fast with interpolation on, for example the tachometer
under hard braking. At 30 fps (`--no-interpolation`) the needle also jumps,
because the revs really do drop that fast, but it stays on its spindle. With
interpolation on, it looks like it moves off its center. Slow needle
movement looks fine.

## Rechecking `cockpit-anim-fix`

That branch concluded "the vertex positions of the dials are not
interpolated": the needle's geometry would be rewritten by the CPU every
frame, at `0x8209637C`. That doesn't hold up:

- `0x8209637C` is a return address in `sub_82096360` after a call to
  `sub_8258CAA0`. `sub_8258CAA0` is the command list's slot allocation
  (`CommandList::AllocateSlots`, done natively since `allocate-slots`), not
  a vertex write. `--log_upload_owners` names the code that records
  **constant uploads**, not vertex buffers, so seeing `0x8209637C` there
  says nothing about the needle's vertices.
- The rest of that section (assembly, "copies the needle's vertices") was
  written without the code and doesn't match it. `cockpit-anim-fix-plan.md`
  (blending vertex buffer writes) builds on it.
- If the needle were simply not interpolated, it would step at 30 Hz but
  stay on its spindle, like at 30 fps. Moving **off center** points to
  something being blended, but wrongly.

Still standing from that branch: the jump itself is the game's. Nothing in
the cockpit or gauge models changes on render-only passes
(`--log_cockpit_changes`). `[car + 4916]` (the gauge models' material
value) is constant.

## Lead: how blending handles big turns

Everything drawn in 3D goes through world * view-projection products,
blended per frame by `BlendProduct` (`src/interpolation/blend_state.cpp`).
`PlanProduct` decides once a tick how each world matrix blends:

- **Turn under 0.5 rad in a tick (`kCutAngle`)**: `kBlend`. The rotation
  goes along the shortest arc and the **position (the matrix's origin)
  in a straight line**.
- **Turn over 0.5 rad, mostly about one local axis, moved under 8 m**:
  `kSpinning`, treated as a wheel. The position blends in a straight
  line, but the twist about that axis is **taken whole from the newer
  tick** (`BlendSpinning`).
- Otherwise: drawn unblended (the newer tick's matrix).

Both blends assume the object turns about its own origin. A wheel does. A
needle whose mesh origin isn't on its spindle doesn't: its translation is
`p - p * R` for spindle `p`, which follows an arc as it turns.

- With `kBlend`, the straight-line position cuts across that arc. Mid-tick
  error ≈ `|p| * (1 - cos(Δθ / 2))`: small for slow movement, growing with
  the square of the turn per tick.
- With `kSpinning` (fast swings, such as hard braking), the turn snaps to
  the new tick while the origin still slides from the old position. The
  needle is drawn at its new angle around a point that isn't its spindle,
  an error of up to about `|p| * Δθ`.

That fits what the user sees. The faster the needle, the further off
center it gets, and the worst case comes only in fast swings. It's also
the user's idea: a needle moving too far between ticks for the
interpolation to cope. The 0.5 rad threshold switches it from a slightly
wrong blend to a much wrong one.

**Not proven yet.** One thing argues against it: `cockpit-anim-fix`'s
`--log_parented_products` found the 16 multiplies in the cockpit's draw
(`sub_820DBD38`) all placed with exactly the car's matrix (largest turn in
the car's frame 0.000 rad). Either the needles are drawn from another call
path, or that log missed them, or the turn really isn't in a world matrix.
Its `--log_moving_products` "spinning" summary showed only caller
`0x820D0564`, but that caller is `sub_820D04E8`, the model draw, which the
cockpit uses as well as the cars' wheels. So it doesn't rule the needle out.

## Next step: a log, then a drive

Per the repo's rules, confirm with a log before changing the blend. The
plan is a diagnostic in `PlanProduct` (once a tick, off by default) that,
for each paired product near the camera (within ~3 m of the eye, which in
cockpit view is the dashboard):

- works out the world matrix relative to the camera (the cockpit moves
  with the car, so the needle's own turn is what's left),
- logs those that turn more than ~0.02 rad in a tick: the turn, the
  plan (`kBlend` / `kSpinning` / not blended), how far the origin moved,
  the implied radius from the spindle (`|Δt| / (2 sin(Δθ / 2))`), and the
  recorder's caller and r31.

The user drives in cockpit view and brakes hard. If needles show up with a
radius of centimetres or more and `kSpinning` during the swings, the
lead is confirmed. If nothing shows up, the turn isn't in a world matrix
and the lead is wrong.

## Possible fix, if confirmed

Blend such parts about their spindle instead of their origin: solve the
fixed point of the two ticks' transforms (the spindle, `p * Ra + ta = p *
Rb + tb` for the parts relative to the camera or car), slerp the turn
about it, and never treat them as wheels (a needle's swing interpolates
fine, as it isn't the many-turns-a-tick spin of a wheel). Over 0.5 rad, the
turn can still be blended about the spindle. `cockpit-anim-fix`'s
`--blend_about_pivot` has pivot code that could be reused, but it cost every
product 68 bytes on the main thread. It would be better to key it to the
products the log identifies.
