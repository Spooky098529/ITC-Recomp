// Translates Xenos microcode with the SDK's translator, the way SBX's native
// renderer does (src/native/shaders.cpp), and writes the SPIR-V and what the
// renderer needs to bind it. For checking the ports against the translator.
//   translate <vs|ps> <microcode.bin (big-endian words)> <num_reg> <interp mask>
//             <param_gen reg or 255> <rect 0|1> <out.spv> <out.txt>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
#include <rex/graphics/pipeline/shader/spirv.h>
#include <rex/graphics/pipeline/shader/spirv_translator.h>
#include <rex/string/buffer.h>
using namespace rex::graphics;
std::string& FLAGS_dump_shaders_storage_() { static std::string s; return s; }
int main(int argc, char** argv) {
  if (argc < 9) return 2;
  bool vertex = !strcmp(argv[1], "vs");
  std::ifstream in(argv[2], std::ios::binary);
  std::vector<char> bytes((std::istreambuf_iterator<char>(in)), {});
  std::vector<uint32_t> words(bytes.size() / 4);
  memcpy(words.data(), bytes.data(), words.size() * 4);
  for (auto& w : words) w = __builtin_bswap32(w);
  uint32_t num_reg = atoi(argv[3]), mask = strtoul(argv[4], 0, 0), pg = atoi(argv[5]);
  bool rect = atoi(argv[6]);
  SpirvShaderTranslator::Features f(false);
  f.spirv_version = spv::Spv_1_5;
  f.full_draw_index_uint32 = true;
  f.clip_distance = true;
  f.cull_distance = true;
  f.image_view_format_swizzle = true;
  f.demote_to_helper_invocation = true;
  SpirvShaderTranslator tr(f, false, false, false);
  SpirvShader shader(vertex ? xenos::ShaderType::kVertex : xenos::ShaderType::kPixel, 1,
                     words.data(), words.size(), std::endian::native);
  rex::string::StringBuffer dis;
  shader.AnalyzeUcode(dis);
  uint32_t regs = shader.GetDynamicAddressableRegisterCount(num_reg);
  SpirvShaderTranslator::Modification mod(vertex ? tr.GetDefaultVertexShaderModification(regs)
                    : tr.GetDefaultPixelShaderModification(regs));
  if (vertex) {
    mod.vertex.interpolator_mask = mask;
    if (rect) mod.vertex.host_vertex_shader_type = Shader::HostVertexShaderType::kRectangleListAsTriangleStrip;
  } else {
    mod.pixel.interpolator_mask = mask;
    if (pg < 16) { mod.pixel.param_gen_enable = 1; mod.pixel.param_gen_interpolator = pg; }
  }
  auto* t = shader.GetOrCreateTranslation(mod.value);
  if (!tr.TranslateAnalyzedShader(*t) || !t->is_valid()) { fprintf(stderr, "failed\n"); return 1; }
  auto& bin = t->translated_binary();
  std::ofstream(argv[7], std::ios::binary).write((const char*)bin.data(), bin.size());
  FILE* o = fopen(argv[8], "w");
  auto& c = shader.constant_register_map();
  fprintf(o, "float");
  for (int i = 0; i < 256; i++) if (c.float_bitmap[i >> 6] >> (i & 63) & 1) fprintf(o, " %d", i);
  fprintf(o, "\ntextures");
  for (auto& b : shader.GetTextureBindingsAfterTranslation())
    fprintf(o, " %u:%d:%d", b.fetch_constant, int(b.dimension), int(b.is_signed));
  fprintf(o, "\nsamplers");
  for (auto& b : shader.GetSamplerBindingsAfterTranslation())
    fprintf(o, " %u:%d:%d:%d:%d", b.fetch_constant, int(b.mag_filter), int(b.min_filter),
            int(b.mip_filter), int(b.aniso_filter));
  fprintf(o, "\nvertex");
  for (auto& b : shader.vertex_bindings()) fprintf(o, " %u:%u", b.fetch_constant, b.stride_words);
  fprintf(o, "\ninterpolators_written %u\ncolor_targets %u\n", shader.writes_interpolators(),
          shader.writes_color_targets());
  fclose(o);
  if (getenv("DISASM")) fputs(shader.ucode_disassembly().c_str(), stdout);
  return 0;
}
