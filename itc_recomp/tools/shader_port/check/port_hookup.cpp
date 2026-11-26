// port_hookup <vs|ps> <microcode, big-endian> <interpolator mask> <param gen> <rect 0|1> <out.spv>
// The renderer's path (FindGamePort, MakeFromPort) from the command line.
#define XXH_INLINE_ALL
#include "xxhash.h"
#include "shader_ports.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
namespace renderer {
uint64_t MicrocodeHash(const uint8_t* ucode, uint32_t size) { return XXH3_64bits(ucode, size); }
}
int main(int argc, char** argv) {
  if (argc != 7) return 1;
  std::ifstream in(argv[2], std::ios::binary);
  std::vector<uint8_t> ucode((std::istreambuf_iterator<char>(in)), {});
  auto stage = !std::strcmp(argv[1], "vs") ? renderer::ShaderStage::kVertex : renderer::ShaderStage::kPixel;
  uint64_t hash = renderer::MicrocodeHash(ucode.data(), uint32_t(ucode.size()));
  const renderer::GamePort* port = renderer::FindGamePort(stage, ucode.data(), uint32_t(ucode.size()), hash);
  if (!port) { std::fprintf(stderr, "no port for %016llX\n", (unsigned long long)hash); return 3; }
  renderer::TranslatedShader shader;
  if (!renderer::MakeFromPort(*port, ucode.data(), uint32_t(ucode.size()), std::strtoul(argv[3], 0, 0),
                            uint32_t(std::strtol(argv[4], 0, 0)), std::atoi(argv[5]) != 0, shader)) return 4;
  std::ofstream(argv[6], std::ios::binary).write(reinterpret_cast<const char*>(shader.spirv.data()), shader.spirv.size() * 4);
}
