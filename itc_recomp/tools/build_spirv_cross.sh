#!/usr/bin/env bash
# Builds SPIRV-Cross's command-line tool (spirv-cross) into
# itc_recomp/out/tools/spirv-cross/, for writing the shaders as HLSL
# (src/native/glsl/compile_game.py and compile.sh; docs/d3d11-backend.md,
# step 4). An offline tool like glslangValidator: nothing of it is built
# into itc. The tag matches the Vulkan SDK of the glslang and spirv-tools
# the shaders were made with. Run from the repo root.
set -euo pipefail

TAG=vulkan-sdk-1.4.363.0
ROOT=itc_recomp/out/tools/spirv-cross
SRC=$ROOT/src-$TAG
BUILD=$ROOT/build-$TAG

mkdir -p "$ROOT"
if [ ! -d "$SRC" ]; then
  git clone --depth 1 --branch "$TAG" \
    https://github.com/KhronosGroup/SPIRV-Cross.git "$SRC"
fi
cmake -S "$SRC" -B "$BUILD" -DCMAKE_BUILD_TYPE=Release \
  -DSPIRV_CROSS_CLI=ON -DSPIRV_CROSS_ENABLE_TESTS=OFF \
  -DSPIRV_CROSS_SHARED=OFF -DSPIRV_CROSS_STATIC=ON \
  -DSPIRV_CROSS_ENABLE_C_API=OFF >/dev/null
cmake --build "$BUILD" --target spirv-cross -j"$(nproc)" >/dev/null
cp "$BUILD/spirv-cross" "$ROOT/spirv-cross"
"$ROOT/spirv-cross" --revision
echo "$ROOT/spirv-cross"
