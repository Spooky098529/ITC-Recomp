#!/bin/sh
# Builds the comparison tools (README.md). Linux or WSL, g++ 13 or later and
# the Vulkan headers and loader (libvulkan-dev); glslangValidator and
# spirv-opt on the path.
#   ./build.sh <rexglue-sdk folder> [out folder]
set -e
sdk=$(cd "$1" && pwd)
here=$(cd "$(dirname "$0")" && pwd)
src="$here/../../../src"
vulkan="$src/native/vulkan"
out=${2:-"$here/build"}
mkdir -p "$out/obj"
inc="-I$sdk/include -I$sdk/src -I$sdk/thirdparty/fmt/include -I$sdk/thirdparty/spdlog/include
     -I$sdk/thirdparty/glslang -I$sdk/thirdparty/xxHash -I$sdk/thirdparty/vulkan-headers/include
     -I$sdk/thirdparty/spirv-headers/include -I$sdk/thirdparty/renderdoc
     -I$sdk/thirdparty/tomlplusplus/include -I$sdk/thirdparty/simde -DFMT_HEADER_ONLY -DSPDLOG_HEADER_ONLY"

# translate: the SDK's translator, set up as the renderer sets it up.
sources="src/graphics/pipeline/shader/shader.cpp src/graphics/pipeline/shader/spirv.cpp
  src/graphics/pipeline/shader/spirv_builder.cpp src/graphics/pipeline/shader/spirv_translator.cpp
  src/graphics/pipeline/shader/spirv_translator_alu.cpp src/graphics/pipeline/shader/spirv_translator_fetch.cpp
  src/graphics/pipeline/shader/spirv_translator_memexport.cpp src/graphics/pipeline/shader/spirv_translator_rb.cpp
  src/graphics/pipeline/shader/translator.cpp src/graphics/pipeline/shader/translator_disasm.cpp
  src/graphics/format/ucode.cpp src/graphics/util/draw.cpp src/graphics/register_file.cpp
  src/graphics/registers.cpp src/graphics/xenos.cpp src/graphics/pipeline/texture/extent.cpp
  src/graphics/pipeline/texture/info.cpp src/graphics/pipeline/texture/info_formats.cpp
  src/graphics/pipeline/texture/util.cpp src/ui/graphics_util.cpp src/core/filesystem_posix.cpp
  src/core/logging.cpp src/core/math_gcc.cpp src/core/string_buffer.cpp src/core/memory.cpp
  thirdparty/glslang/SPIRV/InReadableOrder.cpp thirdparty/glslang/SPIRV/Logger.cpp
  thirdparty/glslang/SPIRV/SpvBuilder.cpp thirdparty/glslang/SPIRV/SpvPostProcess.cpp"
objects=""
for s in $sources; do
  o="$out/obj/$(echo "$s" | tr / _).o"
  [ -f "$o" ] || g++ -std=c++23 -O1 -mssse3 -msse4.1 -mavx2 $inc -c "$sdk/$s" -o "$o"
  objects="$objects $o"
done
g++ -std=c++23 -O1 $inc "$here/translate.cpp" "$here/translate_stubs.cpp" $objects -o "$out/translate"

# vkdiff: runs two shaders with the same inputs.
{
  echo "#pragma once"
  echo "#include <cstdint>"
  for f in pixel_test.vert vertex_test.geom vertex_test.frag; do
    glslangValidator -V --target-env vulkan1.3 "$here/$f" -o "$out/$f.spv" > /dev/null
    python3 -c "import struct,sys; d=open(sys.argv[1],'rb').read(); print('inline constexpr uint32_t k_%s[] = {%s};' % (sys.argv[2], ','.join('0x%x' % w for w in struct.unpack('<%dI' % (len(d) // 4), d))))" "$out/$f.spv" "$(echo $f | tr . _)"
  done
} > "$out/harness_shaders.h"
g++ -std=c++20 -O1 -I"$out" "$here/vkdiff.cpp" -lvulkan -o "$out/vkdiff"

# specialize and port_hookup: the renderer's own code (src/native/vulkan).
g++ -std=c++20 -O1 -I"$src" -I"$vulkan" "$here/specialize.cpp" "$vulkan/port_spirv.cpp" -o "$out/specialize"
g++ -std=c++20 -O1 -I"$src" -I"$vulkan" -I"$sdk/include" -I"$sdk/thirdparty/vulkan-headers/include" \
  -I"$sdk/thirdparty/xxHash" -I"$sdk/thirdparty/fmt/include" -DFMT_HEADER_ONLY "$here/port_hookup.cpp" \
  "$vulkan/shader_ports.cpp" "$vulkan/port_spirv.cpp" -o "$out/port_hookup"
echo "built in $out"
