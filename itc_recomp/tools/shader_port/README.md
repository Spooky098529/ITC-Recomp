From SBX-Recomp (`sbx_pc/tools/shader_port/` at b128953), used with its author's OK; changed here: our paths, the vertex fetches' swizzle constant, and `table_from_json.py` (ours). See `docs/shaders.md`.

# shader_port

Writes the game's shaders (Xenos microcode) as GLSL for the native
renderer, and the renderer's table of them. `docs/shaders.md` explains the
ports and how the renderer uses them; `docs/part_shaders.md` is the part
files' shaders for the mod tool.

| File | What |
|---|---|
| `xenos_ucode.py` | Reads a shader file (XPU: constant table, literal constants, microcode) and decodes the microcode. |
| `ucode_to_glsl.py` | Writes one shader as GLSL, computing what the SDK's translator does; raises `Unsupported` for what no game shader uses. |
| `xe_interface.glsl` | What the ports share: the buffers, fetches, instructions without an operator, and what happens before and after the shader. Copied beside the ports. |
| `port_all.py` | Ports every shader in its inputs, once each (by microcode hash), and writes `shaders.json` and the renderer's table (`--table`). |
| `write_doc.py` | Writes the tables in `docs/shaders.md` (`catalog`) and `docs/part_shaders.md` (`parts`) from `shaders.json`. |
| `check/` | Runs each port against the SDK's translation with the same inputs (`check/README.md`). |

Needs Python 3 and `xxhash` (`pip install xxhash`). From this folder:

```
python port_all.py --out ../../src/native/glsl/game --table ../../src/native/game_shader_ports.h \
    <BUILD_OUT folder> <BUILD_CRS_OUT folder> ../../src/graphics/compiled_shaders.h
cd ../../src/native/glsl && python3 compile_game.py
python write_doc.py catalog ../../src/native/glsl/game
python write_doc.py parts ../../src/native/glsl/game
```

Inputs can be folders, `.xpu` shader files, `.part` files (their shader
sections are read directly; the part shaders get the mod tool's ids) and C
headers of shader files (`compiled_shaders.h`, the shaders the game
compiles at boot). `--check <glslangValidator>` compiles each port as it's
written. The run lists any of the mod tool's 39 part shaders it didn't
find.

`compile_game.py` (in `src/native/glsl`) compiles the ports into
`generated_game_shaders.h`, with glslangValidator and spirv-opt.
