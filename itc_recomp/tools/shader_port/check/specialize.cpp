// specialize <in.spv> <out.spv> <vs|ps> <interpolator mask> [SpecId=value ...]
// SpecializePort (src/native/port_spirv.cpp) from the command line.
#include "port_spirv.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
int main(int argc, char** argv) {
  if (argc < 5) return 1;
  std::ifstream in(argv[1], std::ios::binary);
  std::vector<char> b((std::istreambuf_iterator<char>(in)), {});
  renderer::PortSpecialization s;
  s.vertex = !std::strcmp(argv[3], "vs");
  s.interpolator_mask = std::strtoul(argv[4], nullptr, 0);
  for (int i = 5; i < argc; i++) {
    const char* eq = std::strchr(argv[i], '=');
    s.constants.push_back({uint32_t(std::strtoul(argv[i], nullptr, 0)), uint32_t(std::strtol(eq + 1, nullptr, 0))});
  }
  auto out = renderer::SpecializePort(reinterpret_cast<const uint32_t*>(b.data()), b.size() / 4, s);
  if (out.empty()) return 2;
  std::ofstream(argv[2], std::ios::binary).write(reinterpret_cast<const char*>(out.data()), out.size() * 4);
}
