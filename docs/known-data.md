# Known data

The game's globals and object fields worked out so far (the functions
are in [`functions/`](functions/functions.md)).

| Address | What | Notes |
|---|---|---|
| vehicle+0x30 | Speed (float, m/s) | Read by `Vehicle::GetSpeedKmh`. |
| vehicle+0xD0 | Vehicle world matrix | 4x4 row-major: rotation rows, then position row (w = 1). The car's forward axis is -row 0. |
| vehicle+0xCF0 | Copy of the world matrix | Identical every tick; not yet known which one drawing reads. |
| `0x826BAC10` | Camera view matrix | Pass 0's view (+0x10). The camera position is -t·Rᵀ. In the car's frame the views sit at (-5.48, 1.34, 0) for chase, (-0.44, 0.67, 0) for bumper and (-0.26, 0.85, -0.38) for cockpit. Writing it has no effect while driving: Draw rebuilds it from the eye and target. |
| `0x826BAC00` | Camera pass array | 21 objects of 592 bytes: view +0x10, projection +0x50, view-projection +0x90, eye +0x190, look-at target +0x1A0, up +0x1B0. Pass settings at +564 to +588, active flag at +588. Pass 0 is the main 1280x720 view; pass 10 a 1024x1024 reflection and 11 a 512x512 shadow map, both rebuilt in Draw. |
| `[0x82814E34]` | Title screen object | +4 state (5 = start the attract race), +36 idle ticks. See "The title screen" in `title-screen.md`. |
| car + 16 | The car's world matrix, for drawing | Copied in by `sub_8215C780` each tick; the cockpit dials' parent. |
| car + 696, + 700, + 704 | Speed (km/h), revs, boost | Set each tick by `sub_8215C780`; the cockpit dials read them (`sub_820E42E8`, `sub_820E45A8`). `sub_820DAC68` takes 1.0 off + 704 every pass. |
| `0x70180000` | Main thread stack | During Draw, holds temporary world matrices and camera copies passed to the recorders (for example 0x7018E640). Not render state. |
