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

## Findings from `--log_near_camera_turns` (2026-10-18)

`--log_near_camera_turns` (off by default): once a tick, paired products
within 3 m of their camera that turn relative to it, with how they blend
(blend / spinning / not blended), the turn, how far the origin moved and
the radius about the point it turns about, and the recorder's caller and
r31. Every 2 s a summary by blend kind.

- Attract race (chase camera): nothing near the camera turns, apart from
  an unknown part ~2.5 m away turning ~0.97 rad a tick (a wheel rolling
  past, caller `82596D9C` once, `0` in another run).
- Cockpit drive (`itc_071`): one part turns: caller `820D0564` (the model
  draw), r31 `055F06A0`, the tachometer's needle. Its origin is **0.80 m
  from the point it turns about** (in `itc_074`, 0.89-0.94 m), so its
  mesh's origin isn't on its spindle. Revving, it turns 0.11-0.34 rad a
  tick and gets the normal blend (origin moved straight: the spindle
  drifts ~1.1 cm mid-tick at 0.33 rad). Fast swings reach 0.51-0.95 rad a
  tick and get the wheel blend (turn taken from the newer tick, origin
  sliding).
- Each needle line comes twice, once with caller `0` (a second product with
  the same values).
- In `itc_074` tick 2306, **two** products with caller `820D0564` and r31
  `055F06A0` show up, with radii 0.51 m and 0.08 m against the usual
  0.89-0.94: likely two needles sharing one mesh (tachometer and
  speedometer?) paired with each other. Parts sharing a key are paired by
  nearest origin (`MatchTick`), and these needles' origins are ~0.9 m off,
  swinging up to 0.45 m a tick. Not confirmed.

## Tried and reverted: blending about the spindle

`05d2dd0` (`--blend_about_spindle`, reverted in `160ce3b`): for parts
within 3 m of the camera, solve the point both ticks place at the same
spot in camera space, require the same point as the previous tick, and
blend turning about it. The user: "it still jumps, now it's jumping out of
the dial across the screen". In the log, most of the fast swings didn't get
it (they failed the same-point check), which fits a wrong pairing.

## Pairing checked: the needles aren't swapped (2026-10-18)

`--log_near_camera_pairs` (off by default): once a tick, groups of the
per-object recorder's products sharing a key (caller, r31, world and camera
addresses) with a part within 3 m of their camera, each member's r24-r30,
position and the previous tick's product it was paired with (as an anchor
or in the "rest" loop of `MatchTick`).

Cockpit drive (`itc_075`, ~1,260 ticks of cockpit view, revving and
braking): five groups near the camera, all caller `820D0564`. The
needle's (r31 `055F06A0`) has **four** members, always at exactly the
same position (one needle drawn four times; r26 is 0-3, one per member,
the same every tick; r29/r30 alternate between two buffers from tick to
tick). The others (r31 `055F0870`, `05570BC0`, `05570480`, `028D01E0`) have
two each.

- 12,822 pairings logged, **none** swapped: each member always paired with
  the previous tick's member with the same occurrence and r26. 22 unpaired,
  all on a group's first tick.
- So the wrong-pairing suspicion is ruled out, and so is a second needle
  sharing the mesh. (Where `itc_074`'s 0.51 m and 0.08 m radii came from
  is unexplained.)
- The needle's radius is a steady 0.80 m, swings up to 0.45 rad a tick
  with the normal blend and 0.62 rad once with the wheel blend (tick 1331).
  The car moves ~0.88 m a tick in the world.

That leaves the blend itself (the first lead): straight-line origin and a
turn about the origin, with the origin 0.8 m from the spindle. At 0.45 rad a
tick the spindle drifts ~1.9 cm mid-tick; with the wheel blend, about
`0.8 * Δθ / 2`, ~25 cm at 0.62 rad.

## Next

1. ~~Log the pairing~~: done, not swapped (above).
2. Rather than special cases: screw interpolation (dual quaternions)
   for the normal blend, which doesn't depend on where a mesh's origin is.
   Fast swings over 0.5 rad, taken for wheels, are still to solve.
