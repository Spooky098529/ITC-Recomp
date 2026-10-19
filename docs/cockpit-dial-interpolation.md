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

## Frame by frame: where the spindle is drawn (2026-10-18)

`--log_near_camera_frames=<file>` (off by default): for each product of
`--log_near_camera_turns`, the point it turns about (found once a tick, in
the camera's frame) and, every frame, where that point is drawn on a
1280x720 screen, where it should be (fixed relative to the camera, with the
frame's blended camera) and where a screw blend (`BlendScrew` in
`transform.cpp`: the world matrix's motion between ticks as a turn about a
line plus a slide along it, blended along that) would put it. Records are
kept in memory and appended to the file every 10 s, so the frame rate isn't
affected (the run held 150 fps).

Cockpit drive (`itc_076`, 7,198 frames, one needle record a frame; one
frame drew no needle blended): pixels between where the spindle is drawn
and where it should be, by the turn a tick:

| turn a tick | blend | frames | now: median / max | screw: median / max |
|---|---|---|---|---|
| < 0.1 rad | normal | 6,009 | 0.5 / 4.4 px | 0.8 / 8.8 px |
| 0.1-0.3 | normal | 999 | 3.6 / 31 px | 0.9 / 7.9 px |
| 0.3-0.5 | normal | 175 | 31 / 84 px | 1.6 / 5.9 px |
| >= 0.5 | normal | 5 | 57 / 84 px | 1.6 / 1.8 px |
| >= 0.5 | wheel | 10 | 997 / 1,799 px | 1.2 / 4.4 px |

- The off-axis jumps are the blend's: tens of pixels with the normal
  blend on fast swings, and the wheel blend (over `kCutAngle`) puts the
  spindle across the screen, as the user saw with the reverted spindle
  blend and now.
- A screw blend keeps it within a few pixels on every swing. On slow
  movement it's slightly worse than now (median 0.8 against 0.5 px, max
  8.8): the world matrix also carries the car's movement (~0.9 m a tick),
  which a screw turns into an arc.
- Caveat: the point measured is the one on the turning line nearest the
  mesh's origin, which draws below the screen (y ~930), not the dial
  itself. The errors at the dial will differ in size, not in kind.
- The standalone check (a part turning about a point 0.8 m from its origin,
  no other movement): the screw blend keeps the point within 0.2 mm on
  turns of 0.1-2.0 rad a tick; the normal blend misses by 2.0 cm at 0.45
  rad and 36 cm at 2 rad.

## `--screw_blend` (2026-10-18, off by default, to test)

With the user's agreement: products planned `kBlend` (turning under
`kCutAngle` a tick) blend as a screw. `PlanProduct` works out the screw
once a tick (`ScrewBetween`: the turn, a point on its line relative to the
older tick's origin, and the slide along it; kept in `ProductPlan`), and
`BlendProduct` blends along it each frame (`BlendScrew`). Under ~0.001 rad
a tick it's the old blend. The wheel case (`kSpinning`) is unchanged, so
the needle's swings over 0.5 rad a tick still jump; that needs a better
wheel test (next).

- Standalone check: ends exactly at the newer tick (within 6e-5), keeps the
  point turned about within 0.2 mm at amounts 0.3 and 0.5.
- Cost: none measurable. Uncapped attract races, alternating: 486 and 479
  fps on, 484 and 485 off.

## Uncommitted work (2026-10-19): camera screw blend, the second needle

Not committed at the user's request until it looks right.

**Camera screw blend.** With the parts' screw blend alone, the car looked
juddery in fast turns in cockpit view: the camera still blended straight
while the car followed an arc. Under `--screw_blend`, each `CameraPair`
now also gets a `ScrewMotion` once a tick (`PlanProduct`), and
`BlendCamera` (used by `CameraBlends::Get` and the frame log) blends the
camera's placement along it. Things attached to each other share one
motion between ticks, so with the same blend they stay locked. The look-at
vectors (billboards, lighting) and the pause menu's cameras still blend
straight. An attract race ran fine, and the user confirmed it fixed the
car's judder in fast cockpit turns.

**One car fixed, another not.** The user: the needle stays on its spindle
in one car, but in another it still jumps. In this car (`itc_084`,
`itc_085`), two needles turn: r31 `055F06A0` (0.79 m from its spindle,
blended on all 5 frames of each tick) and r31 `055F0130` (~1.03 m;
another car in `itc_083` had the same pair as `041706A0` / `04170130`).

- `055F0130` is blended only on the logic pass's frame (1,780 of 1,793
  ticks). On the four render-only passes, its multiply's inputs differ from
  the logic pass's (`Note`: render-only mismatch), so it's not published
  and is drawn as the game recorded it.
- Its render-only matrix is **always the same pose** relative to the
  camera (10,519 of 10,519; mesh origin at view 1.058 0.244 -0.057), ~1.9 m
  and ~2.3 rad from the tick's. The game seems to pose this needle only on
  logic passes, so render-only passes draw a default pose. That's the
  game's, at any frame rate above 30, not the blend's.
- Leads (not decided): for parts keyed by r31, use the tick's product
  (blended) when a render-only pass draws it differently; or find the
  update that poses it and run it on render-only passes
  (`--render_only_update_handlers`, found with `--log_recording_handlers`),
  which would leave it stepping at 30 Hz.

**The tachometer flipping.** The user: in the second car, the tachometer
"inverts randomly when it has to move really fast so it points down for a
fraction of a second". Not seen in the logs yet: in `itc_085`, the
`055F06A0` needle never turned 0.5 rad or more a tick. Suspected (not
proven): a swing over half a turn in a tick, which any blend takes the
short way round (through "down"), the same ambiguity as a wheel's spin.
It could also be the default pose above. The frame log now records the
needle's angle on the dial to tell (below).

**Diagnostics added (uncommitted):**

- `--log_near_camera_frames=<file>`: records now carry r31 and the
  needle's angle on the dial (`AngleOnDial`: the mesh origin's angle about
  the turn's axis, from the camera's up), drawn each frame, plus both
  ticks' angles. An upload that differs from the tick's product is written
  as "upload differs" (none seen so far).
- `<file>.mismatch`, written alongside it: every render-only mismatch of an
  r31-keyed product within 3 m of its camera, with the pass's and the
  tick's origins in view space and the turn between them.
- Records also carry the mesh origin in view space (`origin x y z (ticks
  ... to ...)`), drawn and at both ticks, since the angles turned out
  unreliable (next).

**First angle run (`itc_086`, second car, 2026-10-19).** 7,503 frames of
the main needle (`055F06A0` r26 6) and 4,958 of its other draw (r26 0):
no tick turned it more than 0.5 rad (all but 23 under 0.25 rad), so a
swing over half a turn taken the short way doesn't explain the flip. The
angle jumps there are (8, up to ~2.3 rad, e.g. ticks 3607-3609: -0.87,
-3.12, -0.72) all fall at tick boundaries: the newer angle of one tick
differs from the older angle of the next, though they come from the same
matrix. That's the measurement: the centre and axis are found again each
tick from that tick's screw, and on small turns the camera's own movement
in the car tips the axis. So the angles can't show a flip, hence the
origins. `055F0130` was again blended only on logic passes (1,396
frames), drawn at the fixed default pose (view 1.058 0.244 -0.057) on
render-only ones, 1.0-2.6 rad from the tick's pose; around ticks
2979-2989 the render-only pose varied instead (e.g. 0.999 0.192 0.384).

## Test harness: `itc_recomp/tests/needle_blend.cpp` (2026-10-19)

A standalone program (build line at its top) that runs the blending on a
cockpit needle and checks every frame against the needle turning evenly
between ticks. It uses the real `transform.cpp`, with copies of
`PlanProduct`, `BlendCamera` and `BlendProduct` (keep them in step). The
scene: a car carrying the camera and a dial, the needle's mesh origin 0.78 m
from its spindle. The needle turns at 0.05 to 3.3 rad a tick (steady, or
revving: five ticks up, then a drop 1.6 times as fast). The car is parked,
driving 1 m a tick, turning 0.02 to 0.4 rad a tick, rolling, at the game's
~2 km coordinates, or with a mesh whose axes aren't along the dial. Measured
through the blended camera in the car's frame: the spindle's displacement,
the needle tip's on screen (1280x720), the tilt off the dial, and "flips"
(drawn the other way from the turn, or past the newer tick). `-v` prints
every frame.

Results (worst frame of 12 ticks):

- **Under kCutAngle, both of the game's blends miss, and more as the car
  turns.** Normal: up to 23 px at 0.49 rad a tick (parked) and 29 px with
  the car turning 0.2 a tick. Screw (`--screw_blend`): exact when parked,
  but worse than normal once the car moves (1 m a tick: 21 px; turning 0.05
  a tick: 63 px). That's because the needle's screw (car + needle) and the
  camera's screw (car only) take different paths between ticks.
- **The cut tests the world matrix's turn, the needle's plus the car's**,
  so with the car turning 0.2 a tick, a needle at 0.45 is already past it.
- **Past the cut, the needle takes the wheel path (kSpinning)**, since it
  turns about one of its own axes. That path takes the newer tick's angle
  and moves the mesh origin in a straight line, which is right for a wheel
  (origin on its axle) but puts this needle 0.3-1.2 m off its spindle:
  357 mm at the start of every tick at 0.6 rad a tick, swept back over the
  tick (hundreds to thousands of px). With the car turning 0.2 a tick, its
  angle also goes the wrong way on the first frame of a tick (flips).
  Where the wheel test fails (the mesh's axes not along the dial), the part
  isn't blended at all: the newer tick's world matrix with the blended
  camera, ~1 m off (the car's travel).
- **Proposal tested, "relative": blend each product relative to its camera
  (world * view, as a screw), then draw it with the blended camera.**
  Exact under the cut in every scene (0.0 mm, 0 px; 0.4 mm at 2 km).
  Parts riding with the camera then only show their own turn, and the cut
  tests only that.
- **"Relative, uncut" (no kCutAngle: any turn blends the short way):**
  exact at every needle speed up to 3.0 rad a tick. It fails only where a
  tick's turn is over pi (the rev's drop at 2.0, steady 3.3): it stays on
  its spindle but goes the short way round, which no blend of two ticks can
  avoid. 45 of 288 cases fail, all of them over pi, against 266 (normal),
  277 (screw) and 198 (relative with the cut).
- Wheels: next section.

## Test harness: `itc_recomp/tests/wheel_blend.cpp` (2026-10-19)

The same check for a car's wheel (0.32 m, origin on its axle, spin set by
the car's speed: half a turn a tick at ~108 km/h), measured against its
car's body as drawn (blended the same way), from a chase camera on the
player's car. The scenes: the player's own wheel straight or turning and
steering, another car's wheel alongside, a wheel first seen at speed,
accelerating from rest to 270 km/h, and braking from it. A fifth mode,
"history": relative and uncut, with each product's turn taken as the one
(plus whole turns) nearest its turn the tick before. "Wrong" frames: spin
over 0.3 rad off, or the hub over 2 cm off its place on the car.

- **Now (normal, screw, relative with the cut): wheels step at 30 Hz at
  every speed.** The wheel path takes the newer tick's spin, so 60-100% of
  frames are wrong and the rim is drawn up to 80 px off. The hub stays on
  the car (under 1 mm normal; the screw drifts up to 93 mm).
- **Relative, uncut: the player's own wheels are exact up to 108 km/h**
  (0 px), and wrong (blended the short way, backwards) above it.
- **Other cars' wheels come off their hubs when uncut**: 11 mm at 22 km/h,
  ~110 mm at 97-108 km/h. A screw takes a rolling wheel's motion as a turn
  about a line near where it touches the road, not about its hub, while
  its body follows its own screw. This is the needle's failure mirrored:
  a screw is only consistent with the parent's blend when both turn about
  the same line. Blending relative to the camera fixes that only for
  things riding with the camera.
- **History fixes fast spins only for a wheel seen speeding up from slow**
  (accelerating from rest to 270 km/h: 0 wrong frames of 415). A wheel
  first seen at speed takes the short way on its first tick and then
  stays on that wrong alias (constant 130-270 km/h: 44 of 55 wrong;
  braking from 270 km/h: 196 of 270).
- Conclusion: no rule on one product's two ticks alone is right for both
  a needle (turning about a line away from its origin, its parent moving
  with the camera) and another car's wheel (turning about its origin,
  its parent moving). Both are right blended as a screw **relative to
  their parent** (the car body): the needle turns about its spindle and
  the wheel about its hub. Then apply the parent's blend. The blend
  relative to the camera is the case where the parent is the camera.
  Knowing the parent needs the game's hierarchy: whether the multiplies
  of a part's local matrix by its parent's can be seen is not known yet.

## The angle cut removed (2026-10-19)

`PlanProduct` no longer cuts on the angle: a product blends unless it jumps
over kCutDistance, and any turn blends the short way. The wheel path
(kSpinning, `BlendSpinning`, its stats and log fields) is gone from the
game; `BlendSpinning` stays in `transform.cpp` for the tests. The angle cut
stays for cameras (`IsSameCamera`, the camera passes) and the per-object
path. In the tests, "normal" and "screw" are now the game's blends without
the cut. "relative" keeps the old cut, for comparison.

The tests again (worst frame):

- **Needles, plain blend (the default):** the wheel path's 0.3-1.2 m are
  gone, but the plain blend's own error grows with the swing: 26 mm at
  0.6 rad a tick, 71 mm at 1.0, 266 mm at 2.0 (parked); 42 mm at 0.6 with
  the car turning 0.2 a tick. No flips under pi a tick.
- **Needles, `--screw_blend`:** exact when parked at any speed under pi
  a tick, but off once the car moves (1 m a tick: 21 mm at 0.6, 75 mm at
  2.0; turning 0.2 a tick: 83 mm at 0.6, 514 mm at 3.0).
- **Needles, relative uncut (not in the game):** exact as before (0.0-0.5
  mm) up to 3.0 rad a tick.
- **Wheels, plain blend: exact up to 108 km/h** (0-2 px, hub under 1 mm)
  for the player's car and other cars, where they stepped at 30 Hz with
  the cut. Above 108 km/h, blended the short way (44 of 55 frames wrong,
  rim ~80 px off), as expected.
- **Wheels, `--screw_blend`: off their hubs at every speed** without the
  cut, since fast wheels now reach the screw: 15 mm at 22 km/h, 99 mm at
  54 km/h, ~470 mm at 108 km/h, 618 mm for another car at 270 km/h. So
  without the cut, `--screw_blend` breaks wheels. Before, the cut kept
  them on the wheel path.

So with the cut gone, the default (plain) blend interpolates wheels
properly up to 108 km/h, but leaves needles off their spindle on fast
swings. `--screw_blend` fixes parked needles but breaks wheels. The rule
that works for both is still to blend each part relative to its parent
(the previous section's conclusion).

## In the game with `--screw_blend`, no angle cut (`itc_088`, 2026-10-19)

The user: the tachometer's needle "is not off axis now but it's moving to
the wrong place", and the wheels look "juddery and broken".

- Wheels: as `wheel_blend.cpp` predicted. r31 `041900F0` (a wheel ~2.5 m
  from the camera, ~0.98 rad a tick) is drawn 20-27 px off on most frames,
  its angle jumping within ticks. Keep `--screw_blend` off for wheels.
- The car's first needle, r31 `03FC06A0`, is fine: no tick over 0.5 rad,
  no frame jumps.
- The second needle, r31 `03FC0130` (`03FB0130` in another car), is again
  blended only on logic passes (446 frames). On render-only passes the game
  draws it at one fixed pose (view 1.058 0.244 -0.057; 2,333 times, 98%
  of the mismatches near the camera), a median 2.8 rad (max 3.14) from the
  tick's pose: roughly the opposite way. Most likely what the user sees.
  It isn't known yet which dial it is.
- Why it isn't blended: on render-only passes the game draws again and
  each multiply is replaced by its tick's blend only if its inputs match
  the tick's (`Note`, the render-only check). This one's world matrix
  differs (the game's rest pose), so it's left as drawn.
- Two fixes considered. (1) For products keyed by r31, trust the key over
  the inputs check. (2) The user's preference to investigate: find the
  game code that poses the needle (only on logic passes) and have it pose
  it on render-only passes too, without advancing state. The inputs would
  then match and the blend work as is. Precedent: the rival logo
  (`--redraw_rival_logo` in `docs/functions.md`), where delivering the
  whole Update ran the cutscene too fast and a native override repeating
  only the draw worked.

## Next

1. ~~Log the pairing~~: done, not swapped (above).
2. Rather than special cases: screw interpolation (dual quaternions)
   for the normal blend, which doesn't depend on where a mesh's origin is.
   Fast swings over 0.5 rad, taken for wheels, are still to solve.

## Finding the code that poses the needle (`--log_dial_pose`, 2026-10-19)

`--log_dial_pose=<file>` (`src/debug/dial_pose_log.cpp`, written every 5 s)
logs, by pass type: each draw of the parts whose address ends in
`--log_dial_pose_parts` (default `0130,06A0`, the two needles) at
`sub_820D04E8` (caller, world matrix, the part's changed words); each call
of `sub_820E45A8` (below); and each needle angle it gets from
`sub_820D3688`.

**First drive (second car, cockpit view).** `--blend_while_drawing` is
off by default, so render-only passes draw from the game's memory as it
is: the rest pose is the game's own result.

- Car `055F....`: `055F0130` is drawn as part index 7 (caller
  `0x820E3B8C`, the same world matrix on every pass of a tick: not posed)
  and as **index 9 from `0x820E4914`**, the posed needle. On logic passes
  index 9 gets its real pose. On render-only passes it gets another
  rotation, ~1.1 m away (e.g. x -1882.0 against -1883.45, height 16.30
  against 15.22), the same on all render-only passes of a tick and moving
  with the car each tick: the needle at a rest angle.
- The part's first 0x200 bytes never change on either pass, so the pose
  isn't stored in the part.

**`sub_820E45A8(dash = r3, car = r4, r5 = 0x826BAC00, the camera passes)`
poses the cockpit dials** (one caller, in `itc_recomp.81.cpp`). For each
needle it works out an angle, turns the identity matrix by it
(`sub_820A2D30`), multiplies it by the needle's two local matrices and the
car's world matrix (car + 16) with `sub_8209BD50` / `sub_820A0AC0`, and
draws it with `sub_820D06F0` (a jump to `sub_820D04E8`):

| Needle | Angle | Local matrices | Drawn |
|---|---|---|---|
| first (`...06A0`?) | `sub_820D3688([dash + 5328], [[dash] + 12], car)` | dash + 32, dash + 96 | `sub_820D06F0(dash + 2176, r5 + 144, ...)` |
| second (`...0130`, index 9) | `sub_820D3688([dash + 5332], [[dash] + 24], car)`; if `[dash + 4]` is 5, from car + 704 clamped | dash + 272, dash + 336 | `sub_820D06F0(dash + 2416, ...)` at `0x820E4914` |

Each needle is skipped unless its model is set (`[[dash] + 8]` and
`[dash + 1700]` for the first, `[[dash] + 20]` and `[dash + 1940]` for the
second); `[dash + 5296]` / `[dash + 5300]` (or `5324` in mode 5) is stored
into the model + 48 first.

**`sub_820D3688(range = r3, kind = r4, car = r5)` is a needle's angle
from a car value**, with no state: it reads car + 704, 712, 716 or 720
(`kind` 0-3) and maps it linearly from `[range + 0, range + 4]` to
`[range + 8, range + 12]`, clamped at both ends, times a constant
(`[0x82000000 + 10568]`, probably degrees to radians). With no range it
returns `[0x82000000 + 2612]`. Nothing smooths or accumulates, so calling it
again is harmless. Car + 704 is probably the speed (mode 5 uses it).

In an attract race (chase camera), `sub_820E45A8` runs on logic and
render-only passes alike, with mode 7 and no ranges.

**Second and third drives (second car, cockpit).** (`...0130` turned out
to be the turbo gauge, not the tachometer.) The dash is in **mode
5**, so the second needle's angle comes from **car + 704** (clamped), not
`sub_820D3688`, and the dash's first needle isn't used (`...06A0` is posed
elsewhere, drawn from `0x820E4474` / `0x820E4594`). `sub_820E45A8` runs on
every pass, and its needle's local matrices (dash + 272/336) never change.
But **car + 704 drops by 2.0 on every pass**: -0.4 on a logic pass, then
-2.4, -4.4, -6.4, -8.4 on the render-only passes, and back on the next
logic pass. Below its range the angle clamps to -2.14 rad (the logic
passes' was -1.22 here): that is the rest pose. On logic passes the value
moves smoothly with the drive (about -0.4 to 1+): a gauge value (boost
pressure, it turned out; see "The fix" below). At 30 fps every pass is a logic pass, so the drift never shows.

**The writer: `sub_820DAC68`, the per-car update called from Draw**
(`this` = car - 4288; it passes `this + 4288` on as the car). Its first
instructions are `[this + 4992] -= [0x82000A30]` (1.0), before any
branch: car + 704 loses 1.0 every call, on every pass. Found with
`--find_per_frame_state --watch_addresses=051F1380` in an attract race
(car `051F10C0`, `itc_094`: tick handler `82093DC0` > message handler
`8215C770` > `sub_820C5DE8` > `sub_820DAC68`, 0.232 -> -0.768 -> -1.768
...); the attract car loses 1.0 a pass, the user's 2.0 (likely two calls
a pass). The value is set again on logic passes (the watch only covers
render-only passes; `sub_8215C780` stores `[r26 + 2944]` into a + 704
field, not yet confirmed to be this one). `frame_state_diff.cpp` already
overrides `sub_820DAC68` (`ITC_FRAME_STATE_WRAP`).

So the game itself works out the wrong pose between ticks: the needle
reads a value that Draw's per-car update keeps lowering on every pass,
which only a logic tick puts right.

## The fix: `--per_tick_car_gauges` (2026-10-19, on by default)

`sub_820DAC68` is now overridden in `src/frame_pacing/per_frame_logic.cpp`
(beside the play time, which is the same pattern): on a render-only pass
it adds `[0x82000A30]` (1.0) back to car + 704 before the call, so the
call's own step leaves it as the logic pass left it. The dials and
anything else reading it see what a 30 fps pass sees; the logic tick sets
it as before. The needle's pose then matches the tick's on render-only
passes, so `Note` blends it like any other part. The `--find_per_frame_state`
wrapper `sub_820DAC68` had (`ITC_FRAME_STATE_WRAP` in
`frame_state_diff.cpp`) moved into the override, with `--debug_hide`.

Attract race with `--log_dial_pose` (cut short by the user, who saw nothing
wrong): 751 dial calls, car + 704 on every render-only pass the same as on
its logic pass.

**Cockpit drive with the fix (second car, 2026-10-19; `dial4.log`,
`near4.log`).** The user: it fixed the **turbo (boost) gauge**, not the
tachometer, which still has the bad interpolation. So:

- `...0130` (index 9, `sub_820E45A8`'s second needle, mode 5) is the
  **turbo gauge**, and **car + 704 is boost pressure** (about -0.4 at
  idle: vacuum). Not the revs, as guessed above.
- The **tachometer is `...06A0`**, drawn from `0x820E4474` (index 0),
  `0x820E4594` (index 6), `0x820E31B4` (1) and `0x820E31F8` (3), posed by
  other code (not `sub_820E45A8`). The game poses it right on every pass:
  it was blended on every frame in all earlier logs. So its fault is the
  blend itself: the plain matrix blend cuts across the dial on fast swings
  (the harness: 26 mm off its spindle at 0.6 rad a tick, 71 mm at 1.0, 266
  mm at 2.0), the earlier sections' problem.
- The turbo fix isn't exact yet: on render-only passes car + 704 now holds
  still, but **1.0 below the logic pass's value** (e.g. 0.6136 on the
  logic pass, -0.3864 on each render-only pass). Before the fix the
  render-only passes went -2.0, -4.0, ... from the logic pass's value.
  So with two `sub_820DAC68` calls a pass in this car, the logic pass's
  dial seems to read the value before one of them (the order within a
  logic pass, and where the tick sets the value, still to log). The
  needle no longer falls to its rest angle, but in-between frames are
  still drawn off the tick's pose (the blend's inputs check rejects them).

Next: the tachometer (find what poses `...06A0`, and blend its angle
rather than its matrix, or blend it about its spindle), and the turbo's
remaining 1.0.

## The tachometer's code (2026-10-19)

**`sub_820E42E8(dash = r3, car = r4, r5 = the camera passes)` poses the
tachometer's dash** (draws from `0x820E4474`, index 0, and `0x820E4594`,
index 6; part `...06A0`). It's `sub_820E45A8`'s recipe again: it gets two
angles from `sub_820D3740([dash + 2896], [dash], car, &a, &b)`, and for
each turns the identity by the angle (`sub_820A2D30`), multiplies by two
local matrices (dash + 32/+96 for the first, dash + 272/+336 for the
second) and the car's world matrix (car + 16), and draws it with
`sub_820D06F0` (dash + 496, then dash + 736).

**`sub_820D3740(table = r3, dash's first word's object = r4, car = r5,
&a = r6, &b = r7)`**, no state, piecewise linear with clamps:

- `a` (index 0) from **car + 696**: 0 below `[0x82000A34]`; up to
  `table + 0` (or `+ 8` when `[r5]`, the dash's word, isn't 0) it's
  `value / from * table[16]`; up to `table + 4` (or `+ 12`) linear to
  `table[20]`; above, `table[20]`.
- `b` (index 6) from **car + 700**: `table[28]` if `table[24]` > value,
  else linear over `table[32..44]` in two pieces, clamped to `table[44]`.

Car + 696 and + 700 are written next to + 704 by `sub_8215C780`
(`sub_820C0128` and `sub_820C0140`'s results), likely the revs and the
speed. Which of the two needles is the tachometer (index 0 or 6) isn't
logged yet.
