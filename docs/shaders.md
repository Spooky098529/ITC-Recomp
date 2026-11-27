# Shaders

**In this repository (2026-11-26, `pc-renderer`):** this page and the work
it describes come from SBX-Recomp (`../SBX-Recomp`, `docs/shaders.md` at
`b128953`; its author is OK with our using it): the tool
(`itc_recomp/tools/shader_port/`), the ports (`src/native/glsl/game/`), the
hand-written shaders (`materials/`, `post/`, `boot/`, `vertex/`), the
renderer's table and fitting (`src/native/shader_ports.*`,
`port_spirv.*`). What differs here (docs/pc-renderer.md, step 2):

- **The vertex fetches' fields come from the declaration** (since
  2026-11-27, `docs/pc-renderer.md`, step 2). The graphics library's patch
  of a vertex shader's microcode for the bound declaration
  (`sub_822AE340`) no longer runs: the object's microcode stays the
  file's, and the renderer makes each fetch's instruction from the
  declaration and the strides the shaders were loaded with, as the patch's
  first loop does (`graphics::shader_patch::FetchFromDeclaration`; the
  patch's sort and merge into mini-fetches left out), then fits the port
  (`FitVertexFetches`, `shader_ports.cpp`): each fetch's layout fields
  from its made instruction, and a swizzle for `xe_vfetch`
  (`XE_VF<n>_SWIZZLE`, specialization constant 100 + 10n + 6): what the
  fetch's result gives each component the file's swizzle then reads (the
  declaration's component order, D3DCOLOR's red and blue exchanged:
  `zyxw`). Shader lookups are kept by the declaration (its address and id)
  and the strides.
- **16 more shaders** (ours, not in SBX's run): ported by
  `tools/shader_port/port_all.py` from the shader files the game loads here
  (`--dump_shader_files=<dir>`), merged into `glsl/game/shaders.json` and
  the table by `tools/shader_port/table_from_json.py`; 151 in all.
- **Vertex shaders take vertex attributes** (since 2026-11-27): fetch n
  reads its vertex's element as raw words from the attributes at locations
  3n to 3n + 2 (`XE_VF_IN` in `xe_interface.glsl`), unpacked and
  byte-swapped in the shader as before; the renderer binds each fetch's
  stream in the vertex arena (`src/native/vertex_arena.h`) at its element
  and makes the vertex input state from the fetches' fields. A rectangle
  list is drawn an instance a rectangle (four vertices), each fetch bound
  three times at instance rate, a vertex apart, so an invocation still
  runs the shader for the rectangle's three vertices and makes the
  fourth. Indices are kept in the host's byte order in the arena and the
  base vertex is the draw's `vertexOffset`; the shaders' index remap (base,
  24-bit mask, the VGT_MIN/MAX clamp), index loads and the line loop's
  closing index are gone.
- **PC inputs** (since 2026-11-27, `docs/pc-renderer.md`, step 2): the
  shaders read no 360 register files or descriptors.
  - Vertex data is in the host's byte order (the vertex arena swaps each
    stream from its fetch constant's endian mode as it copies it in), so
    `xe_vfetch` swaps nothing.
  - A texture is one view and one sampler, bound by its stage: no signed
    view, no sign modes, gamma or exponent picked per component, its size
    from `textureSize` and its LOD bias the instruction's (the sampler's
    own, `render::SamplerState`, from D3D's MipMapLodBias). The only thing
    the draw says of a texture is its reader scale (a pass's image handed
    over, step 4). No texture in play has signed, biased or gamma
    components or an exponent bias (measured; one that does is logged).
  - Float constants: each stage's registers from the lowest the shader
    reads to the highest (all 256 for one that addresses them by the loop
    index; its literal constants, which it holds, left out), a constant's
    name being `xe_fc[its register - the lowest]`. Bools a bit each, loops
    as D3D's int4s (`xe_loop_count`, `xe_loop_start`, `xe_loop_step`).
  - The draw's own constants (`draw_constants.h`, `XeDrawConstants`): the
    flags (polygonal, line, the alpha test's compare), the alpha reference,
    the NDC scale and offset, the targets' exponent biases, the reader
    scales. The fetch constants block is gone.
  So `tools/shader_port/check` (written for the translator's interface) no
  longer binds what the ports read; the sections below on matching the
  translation describe how the ports were made.
- SBX's own additions not used here are removed: the menu texture filter
  and the distance-field font, the AO's normals and light outputs and its
  dithered alpha. `--motion_blur_quality` and `--bloom_quality` stay
  (always 1, the game's).
- Every shader is drawn from its GLSL; the SDK's translator isn't used
  (since 2026-11-26, `docs/pc-renderer.md`, step 3): a shader with no GLSL,
  or a draw its GLSL can't make (point sprites), stops the program, named
  (`ITC_DUMP_UNPORTED=<dir>` in the environment writes its microcode
  there, for `tools/shader_port`). `--native_shaders` and
  `--native_shaders_off` went with the translator.

To remake the SPIR-V after changing the GLSL: `cd itc_recomp/src/native/glsl
&& python3 compile_game.py` (glslangValidator and spirv-opt).

SBX's page follows, its paths changed to ours.

---


The game's shaders written as GLSL for the native renderer, one file per
shader in `itc_recomp/src/native/glsl/game/`, made by
`itc_recomp/tools/shader_port/` from the game's shader files (see
`docs/game_data.md`, "Shaders", for the file format and where they load).
The part files' shaders (the cars' and the course objects' materials) have
their own page for the mod tool: `docs/part_shaders.md`.

Status (2026-11-18): all 135 shaders found are ported (30 vertex, 105
pixel). Each port was run against the SDK's translation of the same
microcode with random inputs, and every pixel and vertex output matched bit
for bit (`itc_recomp/tools/shader_port/check`). The renderer uses them with
`--native_shaders` (on by default since 2026-11-18;
`--native_shaders=false` translates the microcode instead).

The ports are the reference. The shaders the renderer ships are all
hand-written GLSL that says what the shader does instead of what the
microcode does (see "Hand-written shaders"): the 105 pixel shaders (the 28
car part pixel shaders `ps_00`-`ps_27`, the course's 35 in `BUILD_CRS_OUT`
1248-1330, the 27 post-effect pixel shaders and six lit ones in
`BUILD_OUT` 00000004-00000070 and 22801-22811's odd entries, the 9 the game
compiles at boot) and the 30 vertex shaders.

## What's ported

- The archive's shaders: `BUILD_OUT` 3-70 and 22800-22811.
- The course's: `BUILD_CRS_OUT` 1247-1358.
- The shaders the game compiles at boot (`src/graphics/compiled_shaders.h`).
- The part files' shaders: 25 of the mod tool's 39 (`vs_00`-`ps_27`) are in
  the part files sampled so far. `port_all.py` reads `.part` files itself,
  so running it over all of `BUILD_OUT` finds the other 14 (`ps_09`-`ps_13`,
  `ps_18`, `ps_20`, `ps_22`, `ps_24`, `ps_26`, `ps_27`, `vs_04`, `vs_05`,
  `vs_09`) and ports them the same way.

The ports are written from the game's microcode, not copies of it: the
repository has the GLSL and its SPIR-V, no Xbox 360 shader files. The
materials are simple (a few lighting models, one to three textures), so the
ports are also the place to change how something looks.

## How a port reads

A port is the shader's instructions in order, one statement each (or a
short block in braces when an instruction does two things at once).

- `r0`, `r1`, ... are the shader's registers; `p` is the predicate, `ps` the
  previous scalar result, `a0` the address register.
- Constants have their names from the shader's constant table
  (`matWVPVS_0`, `DLightDir_1`; an array's registers are `Name_0`,
  `Name_1`), defined as the packed constant buffer's element. Literal
  constants are written in place (`lit255`). A constant read relative to
  the loop index is `xe_fc[loop_aL.x + n]`.
- Bool constants are tested with `xe_bool(n)`, named in a comment (a pixel
  shader's bools are 128 on: `UseProjTex`, its b0, is 128).
- Vertex shaders write `xe_position` and `xe_interpolators[n]`, pixel
  shaders `xe_color0`.
- Shaders with loops or jumps run as a `switch` on the control flow index
  `pc` in a loop, as the microcode's control flow does; the others run
  straight through.
- The instructions without an operator are functions in
  `xe_interface.glsl`: `xe_mul` (a multiply where 0 times anything is 0),
  `xe_dp3`, `xe_max`, `xe_cube`, the fetches.

**Literal constants** (confirmed). A shader file can carry float constants
the compiler made from numbers in the HLSL. The block is at the offset in
the header's +0x0C: 24 bytes of header, then groups of a u16 place, a u16
count of floats, and the floats. The register is 208 + (place & 0xFFF) / 16,
so the literals fill the registers up to c255. The game uploads them with
the shader; the ports write them in place.

**Vertex fetches** (confirmed). In the files every vertex fetch reads fetch
constant 95 with format, stride and offset 0. The game fills them in for
the vertex declaration in use (the vertex shader patcher, `sub_822AB028`),
so one shader file runs with several layouts and its hash at run time isn't
the file's. A port takes each fetch's fields (`XE_VF<n>_CONSTANT`,
`_OFFSET`, `_STRIDE`, `_FORMAT`, `_FLAGS`, `_EXP_ADJUST`) as specialization
constants, which the renderer reads from the patched instruction. The
"layout-free hash" is the microcode's hash with the patched fields zeroed
(each vertex fetch's bits 20-26 of word 0, bits 12-29 of word 1, and word
2); the renderer finds a vertex shader's port by it.

## Same as the translation

A port computes what the SDK's SPIR-V translator does, step for step, and
takes its place without other changes: the same descriptor sets (0 guest
memory; 1 the constants: 0 system, 1 and 2 float, 3 bool and loop, 4 fetch;
2 and 3 the vertex and pixel textures, images then samplers), the same
texture and sampler order, the same float constant packing, the same
interpolator locations. In detail, as `xe_interface.glsl` does it:

- Arithmetic: every result is `precise` (the translator's NoContraction),
  multiplies use the 0 rule, `max`/`min` pick by `>=`/`<`, comparisons are
  ordered except "not equal", saturation is a clamp to 0-1.
- Texture fetches: the rounding offset (1.5/1024 of a texel, divided by the
  size from the fetch constant), the LOD bias (the fetch constant's word 4
  bits 12-21, plus the instruction's) applied by scaling the coarse
  derivatives, both the unsigned and the signed view sampled and picked per
  component by the system constants' signs (signed, biased `x * 2 - 1`, or
  gamma to linear), then the exponent bias.
- Vertex fetches: every format the translator reads, the endian swap from
  the fetch constant, the normalization constants as the translator's
  (exact float values), the "no zero" signed mode, the exponent.
- Vertex shaders: the vertex index (line loop closing index, endian, base,
  24-bit mask, clamp), the position to Vulkan's clip space (W reciprocal,
  XY and Z divided by W, NDC scale and offset), and rectangle lists: each
  invocation runs the shader for the rectangle's three vertices (indexed
  from guest memory when the draw has an index buffer) and makes the fourth
  corner, as the translator's `kRectangleListAsTriangleStrip`.
- Pixel shaders: the interpolators the vertex shader writes (others read as
  0), the generated parameter (pixel position, facing, line flag) in the
  register the renderer asks for, the alpha test, the render target's
  exponent bias and gamma conversion.

One thing kept as the translator has it though the hardware differs: the
texture result's exponent bias is read from the fetch constant's word 4
bits 13-18, which overlap the LOD bias (bits 12-21); the hardware's
exponent is word 3 bits 13-18. A texture with a LOD bias gets a wrong
exponent in both. Fixing it is one line in `xe_texture_result`, once the
translation is no longer the reference.

The ports don't do what no game shader uses; `ucode_to_glsl.py` refuses it
(`Unsupported`), and the renderer keeps translating such a shader: 3D or
stacked textures, unnormalized coordinates, register LOD or gradients,
texture fetch opcodes other than `tfetch`, mini vertex fetches, predicated
vertex fetches, subroutine calls, registers relative to the loop index,
memory export, depth export, the point size and vertex kill export. Point
sprites' generated coordinates (`param_gen_point`) aren't made either.

## In the renderer

`src/native/shader_ports.cpp` finds a shader's port and fits it to the draw;
`ShaderCache::Get` (`shaders.cpp`) uses it (there is no translation since
2026-11-26):

- A pixel shader's port is found by its microcode hash, a vertex shader's by
  the layout-free hash (the table, `game_shader_ports.h`, is written by
  `port_all.py`).
- The port's SPIR-V is copied and fitted (`port_spirv.cpp`): the
  specialization constants' values (the interpolator mask and the
  generated parameter's register; rectangle lists and each vertex fetch's
  fields, made from the declaration) and the interpolators'
  locations (an interpolator in the mask goes at its index among the
  mask's, the translator's locations; one that isn't goes at 16 + n, where
  nothing reads it). So the pipeline needs no VkSpecializationInfo.
- What a shader reads comes from the table (`table_from_json.py` from
  `shaders.json`): the float constants, the interpolators it writes or
  reads and the color targets it writes, besides the textures and
  samplers. The vertex fetch constants are read from the fetches as made
  for the declaration (the files all say 95). Checked against the translator's analysis for every
  shader and interface of an attract race and the pad replay before it
  went (2026-11-26): no difference.
- How a vertex and a pixel shader connect: the vertex shader's written
  interpolators and the pixel shader's first registers' worth (up to 16),
  less the generated parameter's (SQ_PROGRAM_CNTL bit 18, SQ_CONTEXT_MISC
  bits 8-15).
- Vertex shaders take vertex attributes (above), up to 24 (the renderer
  checks the device's limits at start-up).
- The system constants are ours (`system_constants.h`), at the offsets the
  ports read.

## Hand-written shaders

The ports compute what the microcode computes, register by register, which
makes them exact but hard to change. The hand-written shaders in
`src/native/glsl/materials/` are the same shaders written as ordinary GLSL:
named inputs, a lighting loop, a texture blend. They're what the renderer
ships; the ports stay in `glsl/game/` as the reference they're checked
against.

- The hand-written shaders are in four folders: `materials/` (lit and
  unlit models), `post/` (post effects and screen passes), `boot/` (the
  2D shaders the game compiles at boot) and `vertex/` (every vertex
  shader). Each folder's `variants.txt` lists
  its shaders: the name in `game/` (the port's, `ps_<hash>` or `vs_<hash>`), the file it's
  made from, and the defines that pick its features. A shader that isn't
  listed ships as its port. The files are compiled side by side, so a file
  name is used once across the folders.
- `compile_game.py` puts a header in front of each listed shader, made from
  `game/shaders.json`: `xe_interface.glsl`, the constants by their D3D names
  (`EyePos`, `DLightDir(i)` for an array, as the translator packs them),
  the textures by their sampler names (`tx0`, `cubetx0`) at the
  translator's bindings, and the defines; a constant's size as
  `<name>_COUNT`; for a vertex shader, each vertex fetch's fields as
  specialization constants (100 + 10 per fetch, as the ports have them).
  The result replaces the port
  under the same `k_game_` name in `generated_game_shaders.h`, so the table
  and the renderer don't change. `--emit <folder>` writes every shader's
  full source instead, for the check.
- `common.glsl` has what the material shaders share: the texture fetches
  (with the console's rounding offset), cube map coordinates, the light
  loops' ranges (loop constants 30 and 31), the highlight, and multiplies
  where 0 times anything is 0 (`times`, `scale_color`).
- A sampler name that shares a fetch constant with another (the course's
  four `plmask` names, all fetch constant 11) is defined as the first one.
  A constant array the shader uses only part of (`DLightColor[2]` alone)
  gets a table of where its elements are. A shader can test for a constant
  with `#ifdef` (`lit.frag` uses `ModTexAlpha` only where it exists).

**The lit material** (`lit.frag`, 25 of the 28 car part shaders; the
others are `unlit.frag`). Ambient (`AmbientColor` plus
`MatAmbientDiffuse.rgb`), diffuse and specular light from up to 3
directional lights and 8 point lights, each point light with a falloff
`1 / (a*d^2 + b*d + c)` (`PntLightAtten`) and a spot cone (`SpotLightCone`:
full inside the cosine x, `(cosine - y) * z` outside). The defines:

| Define | What |
|---|---|
| `POSITION_REG`, `NORMAL_REG`, `COLOR_REG`, `UV0_REG`-`UV2_REG`, `TANGENT_REG` | The interpolator each input is in. |
| `NORMALIZE_NORMAL` | Normalize the interpolated normal (some shaders use it as is). |
| `TX0_UV`-`TX2_UV` | Color textures and their coordinates; with tx0 and tx1, tx1 is blended over tx0 by its alpha. |
| `TX1_OVER` | With tx0 and tx1, tx1 laid over tx0 as layers (22805): tx0's alpha is at least the vertex alpha, the alpha is a + b - ab, and the color (tx1 times its alpha plus tx0 times what shows through) is divided by it, 0 where the alpha is under 0.001. The vertex alpha isn't multiplied in again. |
| `BASE_COLOR` | tx0 over the `BaseColor` constant, by tx0's alpha. |
| `BTX0_UV` | A normal map (needs `TANGENT_REG`). |
| `SPECULAR`, `SPTX0_UV` | Highlights; a specular map multiplying `MatSpecularColor`. |
| `SPECULAR_FLOOR` | The smallest cosine the highlight uses (1e-5 unless set; ps_11 and ps_12 use 0). |
| `ENV_CUBE`, `ENV_SPHERE` | A reflection from `cubetx0` or `spheretx0`, by `MatParam.y`. |
| `PROJTEX_CLAMP` | No negative colors when `UseProjTex` is set. |
| `ALPHA_SATURATE` | Clamp the alpha to 0-1. |
| `FIXED_LIGHTS` | Always 3 directional and 4 point lights (00000016); otherwise loop constants 31 and 30 say how many. |
| `SHADOW_REG` | A shadow map for directional light 0 (`dlshd00`, the 16 taps in `common.glsl`) when `UseProjTex` is set. |
| `BACKLIT_HIGHLIGHT` | Each highlight times 1 - min(N.L, 0) (22801-22809). |
| `TWO_SIDED` | Lit from both sides (22805): the diffuse light by abs(N.L), the highlight by abs(R.L) (with `SPECULAR_FLOOR=0.0`). |
| `DIRECTIONAL_COUNT`, `POINT_COUNT` | With `FIXED_LIGHTS`, how many (3 and 4 unless set). A shader without one kind's constants has none of it. |
| `REFLECTION_MASK=<uv>` | `refmasktx0`'s alpha moves the specular color toward 0.1 and the reflection toward 0.7 (22801). |
| `SPECULAR_ALPHA` | tx0's alpha scales the specular color and the reflection (22803, 22807). |
| `FRESNEL` | The highlights and the reflection times 0.2 + 0.8 * (1 - N.V)^5. |
| `BASE_ALPHA`, `BASE_COLOR_OVER` | The alpha is `BaseColor`'s; the base color over tx0 instead of under it. |
| `SPHERE_NORMALIZE` | The sphere map's view direction is normalized. |

**The course materials.** `course_surface.glsl` is their textures: tx0,
and tx1 over it, each scaled by its `ModTexAlpha` component; tx1 is added
(`DETAIL_ADD`) or blended (`DETAIL_BLEND`) by its alpha, times the vertex
alpha with `VERTEX_ALPHA`; the alpha is tx0's plus tx1's (tx0's times the
vertex alpha with tx0 alone). The shade (the vertex color, plus the light
where there are lights) multiplies tx0 (`SHADE_BASE`), tx1
(`SHADE_DETAIL`), the result (`SHADE_RESULT`), or nothing.
`course_lights.glsl` is the course's point lights: four, each on when its
color's w is over 0.5, with the falloff and spot cone above.

- `course.frag` (21 shaders): unlit; with a reflection (`ENV_CUBE`: the
  cube map by `MatParam.y`, times 1 minus tx1's alpha); or with the point
  lights' diffuse light added to the vertex color (`POINT_LIGHTS`).
- `course_projected.frag` (14 shaders): the point lights with diffuse light
  and a highlight (exponent 30), and with `UseProjTex` the light-space
  textures. Light 0 has a shadow map (`plshd00`, 16 taps at fixed offsets
  over `shadowMapSize.x`): the share of taps whose stored depth is less
  than the point's, 1 outside the map's frustum. Each light has a mask
  (`plmask00`), 0 outside its frustum. A frustum is x and y within -1 to 1
  and the depth within 0 to 1, after dividing by w; its texture's y is
  down. Two projectors: `prjtx0` subtracts the sum of its channels,
  `prjtx1` adds its color times 3, each times how much the surface faces it
  (the interpolated normal with `ProjectionPos.xyz`) and a fade over 3
  units from its height (`ProjectionPos.w`); then no negative colors.
  `COLOR_REG` is left out of the four shaders without a vertex color: they
  add only the highlight. `BTX0_UV`: a normal map, whose alpha scales the
  normal (so it dims the diffuse light and the highlight).
- The interpolators are in the same order in all 14: position, normal,
  tangent (with a normal map), the texture coordinates, light 0's shadow
  map coordinates (`SHADOW_REG`), the two projectors', the four lights'
  masks', the vertex color.

**Post effects** (`post/`, by `BUILD_OUT` entry): the velocity buffer
(`screen_motion`, 10/14/68) and motion blur (8, 70); the glow: bright
passes (24-32: a 3x3 or 4x4 block reduced by average, maximum, exp or log
luminance, scaled by how far it's over a threshold), separable Gaussian
blurs of 5, 9 and 13 taps (48-62), an 8-tap blur with given offsets and
weights (38), the four levels combined (40, 42) and added to the scene
(46); the eye's adapted luminance (34) and tone mapping (36, 44); depth of
field (64); the final color adjustments from the display settings (66:
HSV, brightness, gamma, contrast, saturation); and textured or constant
colors (4, 6, 12, 18-22). Two take more samples than the game's when set
(F4, Graphics, Post-Process; with `--native_shaders`, on restart):
`--motion_blur_quality` (1-8) splits each of the motion blur's three taps
into that many along its third of the path, sharing its weight;
`--bloom_quality` (1-8) gives the glow's Gaussian blurs that many taps per
texel over the same width, weighted along the kernel between its taps and
scaled back to its total. Both are specialization constants (10 and 11,
set in `shader_ports.cpp`), so at 1 the shaders are the game's.
**Boot** (`boot/`): sprites, the font (one
texture channel as alpha), a depth view and a hue gradient.

SBX's menu texture filtering and distance-field font (`boot/menu_filter.glsl`,
`boot/font_sdf.glsl`) were never turned on here and are removed (2026-11-27).

**Vertex shaders** (`vertex/`, all 30). The game patches each vertex fetch
for the vertex layout in use (stride, offsets, formats), and the renderer
reads the fields back into specialization constants (`shader_ports.cpp`);
the hand-written shaders read each attribute through the same
`xe_vfetch` by its fetch's index, so the renderer's table and fitting are
the ports'. `vertex.glsl` has what they share: `fetch_attribute(n, used)`,
`MATRIX(name)` (a D3D matrix constant, four float4 rows for row vectors,
as a GLSL `mat4`, so `MATRIX(m) * v` is the same transform; `MATRIX_AT`
for a constant that holds several), the interpolators by register, and
`main()` with the rectangle-list expansion (each invocation runs the shader
for the rectangle's three vertices and makes the fourth). The defines name
which fetch each attribute is (`POSITION`, `NORMAL`, `TANGENT`, `COLOR`,
`UV0`-`UV2`, in the shader's order) and which interpolator each output goes
to (the `*_REG` defines, as the pixel shaders read them).

- `screen.vert` (8): the post effects', sprites' and boot screens' quads:
  the position as given (`POSITION_2D`: x and y only, z 0 and w 1;
  `POSITION_3D`: w 1), moved by half a pixel (`PIXEL_CENTER`), offset by
  `VS_CONSTANT_PARAM_POS_OFFSET` (`POSITION_OFFSET`) or through a matrix
  (`TRANSFORM=<constant>`), and the color and up to three sets of
  coordinates passed on.
- `model.vert` (22): the position through `matWVPVS`; the world position
  (`matWorldVS`) and normal (its 3x3; `NORMALIZE_NORMAL`), the tangent
  (normalized, with the bitangent's sign in w), the coordinates plus
  `TexOffsetVS` (one per set when it has several), the vertex color times
  `MatColorVS`; the position again (`CLIP_POSITION_REG`) and last frame's
  (`OLD_CLIP_POSITION_REG`, `matOldWVPVS`, for the velocity pass); the
  world position in a light's space: the directional light's shadow map
  (`SHADOW_REG`, `matDLVPVS`), or the course's seven (`COURSE_LIGHTS_REG`:
  light 0's shadow map `matPLVPVS`, the two projectors `matTXVPVS`, the
  four masks `matPLMVPVS`, in the order `course_projected.frag` reads them).

**Where they differ from the ports.** They use GLSL's arithmetic, not the
console's: no `precise`, ordinary `max` and `dot`, and a multiply where
infinity times 0 is NaN, not 0. Where an infinity comes up from ordinary inputs
they keep the console's result with `times` and `scale_color`: the
highlight (`pow(0, shininess)` with a floor of 0 and a negative
shininess, or a cosine over 1 with a scaled normal) and the course lights'
strength (a falloff of 1 / 0) are multiplied into their colors that way,
and so is `lit.frag`'s surface color into the light (a surface color of 0
under an infinite light is 0).
The course's projected shaders sample the light-space textures at every
pixel and use them where they apply; the ports sample inside branches,
where the texture level (picked from neighboring pixels' coordinates) is
undefined. They also read every interpolator they declare, without the
port's interpolator mask: an interpolator the vertex shader doesn't write
reads 0 on the console and in a port, but is undefined here.

**Checking them.** `check_ports.py --tolerance <relative>` compares the
shipped shaders (`--emit`'s sources, or with `--hookup` the renderer's own
lookup) against the translation. The inputs then have no infinities and
every interpolator written; a value counts as the same when it's within
the tolerance of the other (relative to the largest value in its pixel, and
at least 1), or when either is infinite or NaN. `--outliers` (default
0.2%) is the share of values that may be further off, for random inputs
that make a result ill-conditioned (a falloff near a pole, a highlight's
exponent on a cosine near 0). `--levels 1` makes the random textures one
level, so the level picked doesn't matter: the course's projected shaders
are checked that way. All 104 earlier hand-written shaders pass at 1e-4 (seed 1)
and 1e-3 (seeds 2 and 3), 6 cases each, also through the renderer's lookup
(`--hookup`); the earlier ones also on seeds 4-7. 22803 has 10 values past
1e-4 on seed 1 (8 allowed): a point light's falloff near a pole, in the
thousands; it passes at 1e-3 on every seed. 22805 passes at 1e-4 (seed 1,
4 values past it) and 1e-3 (seeds 2-5). The 30 vertex shaders pass at 1e-4
(seed 1) and 1e-3 (seeds 2 and 3), 6 cases each with random vertex
layouts (every other case a rectangle list), checked against their ports
(which match the translation bit for bit).

## Changing and checking the ports

From `itc_recomp/tools/shader_port/` (`README.md` there has the details):

```
py port_all.py --out ../../src/native/glsl/game --table ../../src/native/game_shader_ports.h \
    <BUILD_OUT folder> <BUILD_CRS_OUT folder> ../../src/graphics/compiled_shaders.h
cd ../../src/native/glsl
py compile_game.py
```

`itc_recomp/tools/shader_port/check` runs ports against translations (on any Vulkan
device; lavapipe is enough) and is how a change to the generator or the
interface is checked. With `--tolerance` it checks the hand-written
shaders instead (next section).

## Catalog

| Shader | Found in | Constants | Textures | Notes |
|---|---|---|---|---|
| `ps_4B65AAC8168F306F` | 00000004 | `DLightColor` |  |  |
| `ps_7C91ACFDA08200EB` | 00000006 | `DLightColor` | 0 `tx0` |  |
| `ps_7EB16456AAD56CBA` | 00000008, 00000070 | `PS_CONSTANT_PARAM_POSTBLUR`, `PS_CONSTANT_PARAM_POSTBLUR2` | 1 `tx1`, 0 `tx0` | Motion blur; 2 literal constants |
| `ps_ECCA4F5E7E71FB5F` | 00000010, 00000014, 00000068 |  |  | 1 literal constants |
| `ps_E0D070FB58A4EA03` | 00000012, 00000018, 00000020 |  | 0 `tx0` | Glow history cross-fade |
| `ps_6E9AB15C8DAAAB83` | 00000016 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 10 `dlshd00`, 0 `tx0` | Lighting; 10 literal constants |
| `ps_BDFAB64758384F20` | 00000022 |  | 0 `tx0` | 1 literal constants |
| `ps_0E2F6DFFB4C83835` | 00000024 | `PS_CONSTANT_PARAM_0`, `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 1 literal constants |
| `ps_2B77D92EF843AECC` | 00000026 | `PS_CONSTANT_PARAM_0`, `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 1 literal constants |
| `ps_673F221F054ED0A8` | 00000028 | `PS_CONSTANT_PARAM_0`, `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 2 literal constants |
| `ps_E8E9B15BB250C9A2` | 00000030 | `PS_CONSTANT_PARAM_0`, `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 1 literal constants |
| `ps_F21079DB9D2217A1` | 00000032 | `PS_CONSTANT_PARAM_0`, `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 3 literal constants |
| `ps_89DB3429AA053B0E` | 00000034 |  | 0 `tx0`, 1 `tx1` | 1 literal constants |
| `ps_8D3C07B2BE57E921` | 00000036 | `PS_CONSTANT_PARAM_0` | 0 `tx0`, 1 `tx1` | 2 literal constants |
| `ps_61DB6B3F91E0ED87` | 00000038 | `g_avSampleOffsets`, `g_avSampleWeights` | 0 `tx0` | Blur (sample offsets and weights) |
| `ps_9750599DF5ED3B39` | 00000040 | `PS_CONSTANT_PARAM_0`, `PS_CONSTANT_PARAM_1` | 3 `tx3`, 2 `tx2`, 0 `tx0`, 1 `tx1` | Bloom combine |
| `ps_28D0B59C5CB55C37` | 00000042 | `PS_CONSTANT_PARAM_0`, `PS_CONSTANT_PARAM_1` | 3 `tx3`, 2 `tx2`, 0 `tx0`, 1 `tx1`, 4 `tx4` | 1 literal constants |
| `ps_39755C8589500F4C` | 00000044 | `PS_CONSTANT_PARAM_0` | 0 `tx0`, 1 `tx1` | 1 literal constants |
| `ps_CC253DC700BC8028` | 00000046 | `PS_CONSTANT_PARAM_0` | 0 `tx0`, 2 `tx2` | 1 literal constants |
| `ps_92A5199E7EA052BB` | 00000048, 00000060 | `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 1 literal constants |
| `ps_4592974A77C2F02C` | 00000050, 00000062 | `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 1 literal constants |
| `ps_CEA84F137BB1E483` | 00000052 | `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 2 literal constants |
| `ps_41798B3BB7FDDB71` | 00000054 | `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 2 literal constants |
| `ps_85D67A0216D12CB9` | 00000056 | `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 3 literal constants |
| `ps_3EF414175C8B86B3` | 00000058 | `PS_CONSTANT_TEX_SIZE` | 0 `tx0` | 3 literal constants |
| `ps_D8928E2407A68256` | 00000064 | `PS_CONSTANT_PARAM_CAMERA`, `PS_CONSTANT_PARAM_DOF` | 0 `tx0`, 1 `tx1` | Depth of field; 1 literal constants |
| `ps_C5EDA6DC536A2095` | 00000066 | `PS_CONSTANT_BOOL_USE_GAMMA`, `PS_CONSTANT_BOOL_USE_HSV_SCALE`, `PS_CONSTANT_PARAM_CARIBRATE`, `PS_CONSTANT_PARAM_GAMMA`, `PS_CONSTANT_PARAM_HSV_SCALE` | 0 `tx0` | Final color (gamma, HSV, calibration); loops; 4 literal constants |
| `ps_9C3EB0925F0E75E8` | 00000107.part at 0x1C60, 00000128.part at 0x6BA0, 00000129.part at 0x6100, 00000130.part at 0x6780, 00000131.part at 0x7380, 00000132.part at 0x4100, 00000133.part at 0x47A0, 00000134.part at 0x4960 and 1 more | `AmbientColor`, `DLightColor`, `DLightDir`, `MatAmbientDiffuse`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `SpotLightCone`, `SpotLightDir` | 0 `tx0` | part shader `ps_00`; loops; 1 literal constants |
| `ps_B0F0ACD4B7AFFEC8` | 00000141.part at 0x780 | `ModTexAlpha` | 0 `tx0` | part shader `ps_01` |
| `ps_76DEE068496E05C1` | 00000581.part at 0xADEA0, 00000583.part at 0xB5E20, 00000588.part at 0xAE40, 00000591.part at 0xAE40, 00000594.part at 0xAE40, 00000597.part at 0xAE40, 00000600.part at 0xAE40, 00000861.part at 0x12BE0 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `matView` | 6 `spheretx0`, 0 `tx0` | part shader `ps_02`; loops; 2 literal constants |
| `ps_BDEC67D7B4DF91A1` | 00000581.part at 0xAE9A0, 00000583.part at 0xB6920, 00000587.part at 0x45940, 00000590.part at 0x34400, 00000593.part at 0x3FB00, 00000596.part at 0x35D60, 00000602.part at 0x3CD60, 00000614.part at 0x33E20 and 101 more | `AmbientColor`, `DLightColor`, `DLightDir`, `MatAmbientDiffuse`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `SpotLightCone`, `SpotLightDir` | 0 `tx0` | part shader `ps_03`; loops; 1 literal constants |
| `ps_ADC9112A32A92816` | 00000587.part at 0x44800, 00000590.part at 0x33CE0, 00000593.part at 0x3E9C0, 00000596.part at 0x34C20, 00000599.part at 0x3D580, 00000602.part at 0x3BC20, 00000605.part at 0x42960, 00000614.part at 0x32CE0 and 31 more | `AmbientColor`, `DLightColor`, `DLightDir`, `MatAmbientDiffuse`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `SpotLightCone`, `SpotLightDir` |  | part shader `ps_04`; loops; 1 literal constants |
| `ps_B2B10AE1E0E9FE13` | 00000587.part at 0x44EC0, 00000590.part at 0x332C0, 00000593.part at 0x3F080, 00000596.part at 0x352E0, 00000599.part at 0x3CB60, 00000602.part at 0x3C2E0, 00000605.part at 0x41F40, 00000614.part at 0x333A0 and 12 more | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `matView` | 6 `spheretx0` | part shader `ps_05`; loops; 2 literal constants |
| `ps_72DE42674BE25160` | 00000599.part at 0x3DCA0, 00000603.part at 0xAE40, 00000605.part at 0x43080, 00000666.part at 0xAE40, 00001036.part at 0x70C0, 00001089.part at 0x22640, 00001309.part at 0x2BD60, 00001320.part at 0x24BE0 and 39 more | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 0 `tx0` | part shader `ps_06`; loops; 2 literal constants |
| `ps_503D67F05D6DAA03` | 00000644.part at 0x38E60, 00000659.part at 0x21FC0, 00000674.part at 0x2C300, 00000704.part at 0x3C9E0, 00000869.part at 0x333A0, 00004440.part at 0x3C00 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` |  | part shader `ps_07`; loops; 2 literal constants |
| `ps_919CB09ADFA6FA77` | 00000659.part at 0x21640, 00000689.part at 0x21640, 00001348.part at 0x19140, 00001497.part at 0xDDE0, 00003156.part at 0xE7C0, 00003530.part at 0x112A0, 00003765.part at 0x5400, 00004006.part at 0x3E20 and 13 more | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 6 `cubetx0` (cube) | part shader `ps_08`; loops; 2 literal constants |
| `ps_7ECFFC514C780F01` | 00001012.part at 0x5060, 00001036.part at 0x5EC0, 00001052.part at 0x3880, 00001078.part at 0x53E0, 00001089.part at 0x23020, 00001104.part at 0x9060, 00001223.part at 0xBBE0, 00001250.part at 0x3C80 and 50 more | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 6 `cubetx0` (cube), 0 `tx0` | part shader `ps_16`; loops; 2 literal constants |
| `ps_F3C0D4132DB33EFE` | 00001078.part at 0x4F60, 00001223.part at 0xAFC0, 00001460.part at 0xAA60, 00001753.part at 0x14680, 00001982.part at 0xB240, 00002807.part at 0xD8E0, 00004536.part at 0x2580, 00005183.part at 0x189C0 and 14 more | `ModTexAlpha`, `UseProjTex` | 0 `tx0` | part shader `ps_15`; 1 literal constants |
| `ps_92355BEBF0E0C743` | 00001248, 00001304, 00001358 | `ModTexAlpha`, `UseProjTex` | 0 `tx0` | 1 literal constants |
| `ps_197CCCA2D2748351` | 00001250, 00001306, 00001346 | `ModTexAlpha`, `UseProjTex` | 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_959EB72BB94E0226` | 00001252, 00001308, 00001348 | `ModTexAlpha`, `UseProjTex` | 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_B9876682F784223B` | 00001254, 00001310, 00001350 | `ModTexAlpha`, `UseProjTex` | 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_AF8C30E46F0F278A` | 00001256, 00001312, 00001352 | `ModTexAlpha`, `UseProjTex` | 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_1D294C09E07EEED7` | 00001258, 00001314, 00001354 | `ModTexAlpha`, `UseProjTex` | 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_53C20517319EB8FE` | 00001260, 00001316, 00001356 | `ModTexAlpha`, `UseProjTex` | 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_BCE303B3AB695944` | 00001262 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 8 `plshd00`, 11 `plmask30`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 10 literal constants |
| `ps_D81F95EF99CDCE7A` | 00001264 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 10 literal constants |
| `ps_FB2B0F39180042D3` | 00001266 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 10 literal constants |
| `ps_87D87FF305829A05` | 00001268 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 10 literal constants |
| `ps_7D9889AF739804C8` | 00001270 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 10 literal constants |
| `ps_A79C043EB585A204` | 00001272 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 10 literal constants |
| `ps_654A188A1FB15D27` | 00001274 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 10 literal constants |
| `ps_A38DF64E59CB943C` | 00001276 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 1 `btx0`, 8 `plshd00`, 11 `plmask30`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 11 literal constants |
| `ps_4C302AE01D306FF7` | 00001278 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 2 `btx0`, 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 11 literal constants |
| `ps_F1E007F29B8A84AE` | 00001280 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 2 `btx0`, 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 11 literal constants |
| `ps_B6F76A9071E1A15B` | 00001282 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 2 `btx0`, 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 11 literal constants |
| `ps_B6BE65E9A178357D` | 00001284 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 2 `btx0`, 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 11 literal constants |
| `ps_794AB03BE0B770F4` | 00001286 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 2 `btx0`, 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 11 literal constants |
| `ps_33AE995D552BA7E8` | 00001288 | `EyePos`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `ProjectionPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `shadowMapSize` | 2 `btx0`, 8 `plshd00`, 11 `plmask30`, 1 `tx1`, 0 `tx0`, 9 `prjtx0`, 10 `prjtx1` | loops; 11 literal constants |
| `ps_3D74C039B92C9A15` | 00001290 | `EyePos`, `MatParam`, `ModTexAlpha`, `UseProjTex` | 6 `cubetx0` (cube), 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_A479B633DDE45E6F` | 00001292 | `EyePos`, `MatParam`, `ModTexAlpha`, `UseProjTex` | 6 `cubetx0` (cube), 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_A88B5308CB429EB7` | 00001294 | `EyePos`, `MatParam`, `ModTexAlpha`, `UseProjTex` | 6 `cubetx0` (cube), 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_0FC44A2B3333CB61` | 00001296 | `EyePos`, `MatParam`, `ModTexAlpha`, `UseProjTex` | 6 `cubetx0` (cube), 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_F785BBAF1CB0FC6D` | 00001298 | `EyePos`, `MatParam`, `ModTexAlpha`, `UseProjTex` | 6 `cubetx0` (cube), 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_E4790209FE04D555` | 00001300 | `EyePos`, `MatParam`, `ModTexAlpha`, `UseProjTex` | 6 `cubetx0` (cube), 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_BEBB1AEC84FE7D2F` | 00001302 | `EyePos`, `MatParam`, `ModTexAlpha`, `UseProjTex` | 6 `cubetx0` (cube), 0 `tx0` | 1 literal constants |
| `ps_CACE8FF17E207767` | 00001318, 00001332 | `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 0 `tx0` | loops; 1 literal constants |
| `ps_02E137445D415E82` | 00001320, 00001334 | `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 1 `tx1`, 0 `tx0` | loops; 1 literal constants |
| `ps_FBC2D684E3A1C861` | 00001322, 00001336 | `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 1 `tx1`, 0 `tx0` | loops; 1 literal constants |
| `ps_0D4BBF8CA1BD0F47` | 00001324, 00001338 | `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 1 `tx1`, 0 `tx0` | loops; 1 literal constants |
| `ps_72A2AB5CBA6A34A1` | 00001326, 00001340 | `ModTexAlpha`, `UseProjTex` | 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_10A76937892E52F3` | 00001328, 00001342 | `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 1 `tx1`, 0 `tx0` | loops; 1 literal constants |
| `ps_A31B395C13508BBB` | 00001330, 00001344 | `ModTexAlpha`, `UseProjTex` | 1 `tx1`, 0 `tx0` | 1 literal constants |
| `ps_27A79CBF2701BC34` | 00001497.part at 0xEF80, 00001753.part at 0x14B20, 00009892.part at 0x9960, 00011624.part at 0xE260, 00013792.part at 0x21DC0, 00020070.part at 0x93C0 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 6 `cubetx0` (cube), 0 `tx0`, 1 `tx1` | part shader `ps_14`; loops; 2 literal constants |
| `ps_B69BADFDF194591C` | 00003530.part at 0x135E0 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 6 `cubetx0` (cube), 2 `tx2` | part shader `ps_17`; loops; 2 literal constants |
| `ps_0E1E9AD0349FAC43` | 00007636.part at 0x16F80, 00017321.part at 0xD8C0, 00020326.part at 0x26080 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 0 `tx0`, 1 `tx1` | part shader `ps_19`; loops; 2 literal constants |
| `ps_BD9BB201950E597D` | 00009892.part at 0xA480 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 1 `tx1` | part shader `ps_23`; loops; 2 literal constants |
| `ps_806BEF915614FFDB` | 00015977.part at 0x7C00, 00015977.part at 0x7EC0, 00017947.part at 0x7580 | `UseProjTex` |  | part shader `ps_25` |
| `ps_703BAF8642EAEBBC` | 00020950.part at 0x9180 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 2 `tx2` | part shader `ps_21`; loops; 2 literal constants |
| `ps_A3888CA6DC7CAAAA` | 00022801 | `AmbientColor`, `BaseColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `matView`, `shadowMapSize` | 0 `tx0`, 7 `dlshd00`, 6 `spheretx0` | 11 literal constants |
| `ps_375EE1E85E22C6A3` | 00022803 | `AmbientColor`, `BaseColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `matView`, `shadowMapSize` | 0 `tx0`, 7 `dlshd00`, 6 `spheretx0` | 10 literal constants |
| `ps_0E7B2A6B2E322308` | 00022805 | `AmbientColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `matView` | 1 `tx1`, 0 `tx0`, 6 `spheretx0` | 2 literal constants |
| `ps_0B0CB89AB60BE5C7` | 00022807 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex`, `matView`, `shadowMapSize` | 0 `tx0`, 7 `dlshd00`, 6 `spheretx0` | 10 literal constants |
| `ps_AFAAAD8C18FA5956` | 00022809 | `AmbientColor`, `DLightColor`, `DLightDir`, `DLightSpecularColor`, `EyePos`, `MatAmbientDiffuse`, `MatParam`, `MatSpecularColor`, `ModTexAlpha`, `PntLightAtten`, `PntLightColor`, `PntLightPos`, `PntLightSpecularColor`, `SpotLightCone`, `SpotLightDir`, `UseProjTex` | 6 `cubetx0` (cube), 0 `tx0` | loops; 2 literal constants |
| `ps_4F4860F071E25904` | 00022811 | `UseProjTex` | 0 `tx0` | 1 literal constants |
| `ps_9567C79307ACC6F5` | k82001070_ps_3_0 |  | 0 `Texture` |  |
| `ps_800BAA981E59B832` | k820012E8_ps_3_0 |  | 0 `Texture` |  |
| `ps_46E16128D157D50B` | k82001540_ps_3_0 |  |  |  |
| `ps_E2CEF62EAE21A6AB` | k82006D28_ps_2_0 |  | 0 `Texture` |  |
| `ps_95CDB2FE143FD9E7` | k82006F90_ps_2_0 |  | 0 `Texture` |  |
| `ps_CA0FB1474BA9BB09` | k820071F8_ps_2_0 |  | 0 `Texture` |  |
| `ps_958E44CCD28B2BE7` | k82007460_ps_2_0 |  | 0 `Texture` |  |
| `ps_D6135642F477C186` | k8206E308_ps_3_0 | `EndY`, `StartY` |  | 2 literal constants |
| `ps_3290DE6676442FB7` | k8206E848_ps_3_0 |  | 0 `Texture` | 1 literal constants |
| `vs_B2BD800956768112` | 00000003 | `matWVPVS` |  | 1 vertex fetches; layout-free hash `22751FB6850AB182` |
| `vs_F5D1EFF5F688FB63` | 00000005 | `matWVPVS` |  | 2 vertex fetches; layout-free hash `872AB1DEEEE73E77` |
| `vs_F5BD719D22A61B14` | 00000007, 00000043, 00000063, 00000065 |  |  | 2 vertex fetches; layout-free hash `8EA8D034F269C561` |
| `vs_13D2A485CE84A7A9` | 00000009, 00000067 | `matOldWVPVS`, `matWVPVS` |  | 1 vertex fetches; layout-free hash `CF9DDAA6E451517F` |
| `vs_7A210C9DBDC19257` | 00000011, 00000017, 00000021, 00000023, 00000025, 00000027, 00000029, 00000031 and 14 more |  |  | 3 vertex fetches; layout-free hash `DE12D11D2A0EEA8D` |
| `vs_1579E2CF1BCA8E4D` | 00000013 | `matWVPVS` |  | 1 vertex fetches; layout-free hash `597DAE8323874E68` |
| `vs_14A3DA2062CEEF19` | 00000015, 00022800, 00022802, 00022806 | `MatColorVS`, `TexOffsetVS`, `matDLVPVS`, `matWVPVS`, `matWorldVS` |  | 4 vertex fetches; layout-free hash `2A5C8ED27033CACB` |
| `vs_EC767F40947991F7` | 00000019 | `VS_CONSTANT_PARAM_POS_OFFSET` |  | 3 vertex fetches; layout-free hash `6A69A934A7DE925F` |
| `vs_208503E5EA6BEA9B` | 00000069 |  |  | 4 vertex fetches; layout-free hash `6EA2862C5A46E2D8` |
| `vs_93F94842A4742732` | 00000141.part at 0x580 | `MatColorVS`, `TexOffsetVS`, `matWVPVS` |  | part shader `vs_01`; 3 vertex fetches; layout-free hash `2911F82AB7D14D75` |
| `vs_93996C44777D8E9A` | 00000581.part at 0xADC20, 00000581.part at 0xAE720, 00000583.part at 0xB5BA0, 00000583.part at 0xB66A0, 00000587.part at 0x456C0, 00000588.part at 0xABC0, 00000590.part at 0x34180, 00000591.part at 0xABC0 and 214 more | `MatColorVS`, `TexOffsetVS`, `matWVPVS`, `matWorldVS` |  | part shader `vs_02`; 4 vertex fetches; layout-free hash `98552BD957DA772B` |
| `vs_07147F7735F408F8` | 00000587.part at 0x445E0, 00000587.part at 0x44CA0, 00000590.part at 0x330A0, 00000590.part at 0x33AC0, 00000593.part at 0x3E7A0, 00000593.part at 0x3EE60, 00000596.part at 0x34A00, 00000596.part at 0x350C0 and 78 more | `MatColorVS`, `matWVPVS`, `matWorldVS` |  | part shader `vs_03`; 3 vertex fetches; layout-free hash `071F92CECE40CE18` |
| `vs_FA17356113DFBFDA` | 00001078.part at 0x4D80, 00001223.part at 0xADE0, 00001460.part at 0xA880, 00001753.part at 0x144A0, 00001982.part at 0xB060, 00002807.part at 0xD700, 00004536.part at 0x23A0, 00005183.part at 0x187E0 and 14 more | `MatColorVS`, `TexOffsetVS`, `matWVPVS` |  | part shader `vs_07`; 3 vertex fetches; layout-free hash `769378B0B07E7627` |
| `vs_86F28F82FA50FE98` | 00001249, 00001251, 00001253, 00001255, 00001257, 00001259, 00001305, 00001307 and 10 more | `MatColorVS`, `TexOffsetVS`, `matWVPVS` |  | 4 vertex fetches; layout-free hash `1684090CC7241117` |
| `vs_1DA94D62376538E1` | 00001261 | `MatColorVS`, `TexOffsetVS`, `matPLMVPVS`, `matPLVPVS`, `matTXVPVS`, `matWVPVS`, `matWorldVS` |  | 4 vertex fetches; layout-free hash `36FEF714C44C5112` |
| `vs_4044B298F90682EF` | 00001263, 00001265, 00001267, 00001269, 00001271, 00001273 | `MatColorVS`, `TexOffsetVS`, `matPLMVPVS`, `matPLVPVS`, `matTXVPVS`, `matWVPVS`, `matWorldVS` |  | 5 vertex fetches; layout-free hash `4EFF1B399073C3A3` |
| `vs_6B79546EE6E18088` | 00001275 | `MatColorVS`, `TexOffsetVS`, `matPLMVPVS`, `matPLVPVS`, `matTXVPVS`, `matWVPVS`, `matWorldVS` |  | 5 vertex fetches; layout-free hash `188488ABC9198C5E` |
| `vs_6C30284CE74E8A43` | 00001277, 00001279, 00001281, 00001283, 00001285, 00001287 | `MatColorVS`, `TexOffsetVS`, `matPLMVPVS`, `matPLVPVS`, `matTXVPVS`, `matWVPVS`, `matWorldVS` |  | 6 vertex fetches; layout-free hash `E3F7CF8B2ABF2599` |
| `vs_C56643D9D12D081E` | 00001497.part at 0xECE0, 00001753.part at 0x14880, 00007636.part at 0x16CE0, 00009892.part at 0x96C0, 00009892.part at 0xA1E0, 00011624.part at 0xDFC0, 00013792.part at 0x21B20, 00017321.part at 0xD620 and 2 more | `MatColorVS`, `TexOffsetVS`, `matWVPVS`, `matWorldVS` |  | part shader `vs_06`; 5 vertex fetches; layout-free hash `EFF9097B18FFCD68` |
| `vs_9AD90069B3591514` | 00003530.part at 0x13320, 00020950.part at 0x8EC0 | `MatColorVS`, `TexOffsetVS`, `matWVPVS`, `matWorldVS` |  | part shader `vs_08`; 6 vertex fetches; layout-free hash `44AB8939D5E2D90B` |
| `vs_BB9F34FE57827C8D` | 00015977.part at 0x7A60, 00015977.part at 0x7D20, 00017947.part at 0x73E0 | `MatColorVS`, `matWVPVS` |  | part shader `vs_10`; 2 vertex fetches; layout-free hash `56D058A119CF8AA7` |
| `vs_0AA13C6A33820C28` | 00022804, 00001289, 00001291, 00001293, 00001295, 00001297, 00001299, 00001319 and 11 more | `MatColorVS`, `TexOffsetVS`, `matWVPVS`, `matWorldVS` |  | 5 vertex fetches; layout-free hash `7DD79DCA71D9C08A` |
| `vs_C5A6F2B6C0BD3BC4` | 00022808, 00000107.part at 0x19E0, 00000128.part at 0x6920, 00000129.part at 0x5E80, 00000130.part at 0x6500, 00000131.part at 0x7100, 00000132.part at 0x3E80, 00000133.part at 0x4520 and 5 more | `MatColorVS`, `TexOffsetVS`, `matWVPVS`, `matWorldVS` |  | part shader `vs_00`; 4 vertex fetches; layout-free hash `2EFC0344109EEBF9` |
| `vs_E76C7F598230D74B` | 00022810, 00001247, 00001303, 00001357 | `MatColorVS`, `TexOffsetVS`, `matWVPVS` |  | 3 vertex fetches; layout-free hash `44ED2F6944F3BD86` |
| `vs_EB69AFFB9A940463` | k82001770_vs_3_0 | `matWVP` |  | 3 vertex fetches; layout-free hash `170AA3A01190DCED` |
| `vs_100B3DED4CFC57D1` | k820076C8_vs_3_0 |  |  | 1 literal constants; 3 vertex fetches; layout-free hash `A3F683FD360FA72B` |
| `vs_3E4D3F93E10ABFFF` | k8206EB20_vs_3_0 |  |  | 1 literal constants; 3 vertex fetches; layout-free hash `022E32C30924B782` |
