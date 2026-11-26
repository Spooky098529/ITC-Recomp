# check

Runs each ported shader against the SDK's translation of the same microcode
and compares every output. Both run on the same Vulkan device with the same
random inputs (constants, textures, interpolators, vertex data, system
constants, interpolator masks, the generated parameter, rectangle lists),
so any device works; Mesa's software device (lavapipe) is enough.

| File | What |
|---|---|
| `check_ports.py` | The driver: for each shader, a few random cases; prints `ok` or the first mismatch. |
| `translate.cpp`, `translate_stubs.cpp` | The SDK's translator, set up as the renderer sets it up (`src/native/shaders.cpp`). |
| `vkdiff.cpp` | Runs the two shaders: a pixel shader over a 32x32 target, fed by `pixel_test.vert`; a vertex shader on points, its outputs recorded by `vertex_test.geom`. |
| `port_hookup.cpp` | The renderer's own lookup and fitting (`src/native/shader_ports.cpp`) from the command line: finds the port by the patched microcode and makes its SPIR-V. |
| `specialize.cpp` | `SpecializePort` (`src/native/port_spirv.cpp`) alone, for checking ports before they're in the table. |
| `build.sh` | Builds the tools. |

On Linux or WSL with g++ 13 or later, the Vulkan headers and loader
(`libvulkan-dev`), `glslangValidator` and `spirv-opt`, and Python's `xxhash`:

```
./build.sh <rexglue-sdk folder>
VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/lvp_icd.json python3 check_ports.py \
    --translate build/translate --vkdiff build/vkdiff --specialize build/specialize \
    --hookup build/port_hookup --ports ../../../src/native/glsl/game \
    <BUILD_OUT folder> <BUILD_CRS_OUT folder>
```

(`VK_ICD_FILENAMES` picks lavapipe; leave it out for the default device.)
With `--hookup`, the ports tested are the ones the renderer has
(`generated_game_shaders.h`, found through `game_shader_ports.h`); without
it, each port's GLSL is compiled as `compile_game.py` compiles it.

What's random per case: the vertex fetches' formats, signedness, exponents,
strides and offsets (patched into the microcode as the game does it, so
the translator and the port see the same layout); the float constants
(with zeros, ones and infinities; the literal constants as the game
uploads them); bool and loop constants; the alpha test, gamma and
exponent bias settings; texture contents and signs. `--cases` sets how
many cases a shader gets (default 4), `--seed` the random seed, and
`--keep <folder>` keeps a mismatch's files.

**Hand-written shaders** (`src/native/glsl/materials/`, see
`docs/shaders.md`) don't match bit for bit, so they're checked with a
tolerance. Write their full sources first, then point `--ports` there:

```
cd ../../../src/native/glsl && python3 compile_game.py --emit /tmp/shaders && cd -
VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/lvp_icd.json python3 check_ports.py \
    --translate build/translate --vkdiff build/vkdiff --specialize build/specialize \
    --ports /tmp/shaders --tolerance 1e-4 <shader files or folders>
```

With `--tolerance`, the inputs have no infinities and every interpolator
written (a hand-written shader keeps neither the console's 0 * infinity = 0
nor the interpolator mask), and a value counts as the same when it's within
the tolerance of the other, relative to the largest value in its pixel or
vertex output (and at least 1), or when either is infinite or NaN.
`--outliers` (default 0.002) is the share of values per case that may be
further off: random inputs can make a result ill-conditioned, like a point
light's falloff near a pole. `--levels 1` gives the random textures one mip level, so the level
picked doesn't matter: use it for shaders that sample where the port's
level is undefined (the course's projected shaders sample inside
branches). Add `--hookup build/port_hookup` (after running
`compile_game.py` without `--emit`) to test what the renderer actually
ships. The hand-written shaders pass at 1e-4 with seed 1 and at 1e-3 with
other seeds; a few cases on a seed can need 1e-3.

`translate` is also handy alone: `DISASM=1 build/translate ps shader.bin 63 0xFF 255 0 out.spv
out.txt` writes the translation and prints the microcode's disassembly
(the microcode as big-endian words: a shader file from its header size
on).
