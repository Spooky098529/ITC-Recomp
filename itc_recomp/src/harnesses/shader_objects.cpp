// The shader objects (src/shader_objects/, docs/functions/shader-objects.md
// and models.md's shader file parse) against the recompiled functions. Each
// case fills its memory with random bytes, writes its inputs, runs the
// original and the plain C++ (through the register steps below, the
// original where it hands back) from the same inputs, and compares the
// memory exactly: nothing these functions write is computed as a float.

#include "harness.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/model_load.h"
#include "models/xpr.h"
#include "shader_objects/shader_objects.h"

#include <fmt/format.h>

#include <bit>
#include <cstdlib>
#include <mutex>
#include <string>
#include <vector>

namespace harness {

namespace {

namespace so = shader_objects;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    Store(base, address + i, uint32_t(r.Next()));
  }
}

// The register steps: the plain C++ through these, not the adapters.
void NativeParseFile(PPCContext& ctx, uint8_t* base) {
  so::ParseFile(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeVertexShaderInit(PPCContext& ctx, uint8_t* base) {
  if (!so::VertexShaderInit(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_822A8740(ctx, base);
  }
}
void NativePixelShaderInit(PPCContext& ctx, uint8_t* base) {
  so::PixelShaderInit(base, ctx.r3.u32, ctx.r4.u32);
}

// ShaderFile::Parse: random files (the magic's low 6 bits set or not), the
// record anywhere in the scratch area; the scratch area and r3.
void ParseFiles(Context& h) {
  Results results("ShaderFile::Parse (sub_822A8568)");
  for (int c = 0; c < 400; c++) {
    const uint32_t file = h.data + 4 * h.random.Below(256);
    const uint32_t record = h.data + 2048 + 4 * h.random.Below(256);
    uint32_t words[3] = {uint32_t(h.random.Next()), uint32_t(h.random.Next()),
                         uint32_t(h.random.Next())};
    if (c % 3 == 0) {
      words[0] &= ~0x3Fu;
    }
    if (c % 5 == 0) {
      words[1] = h.random.Below(4096);
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (int i = 0; i < 3; i++) {
        Store(h.base, file + 4 * i, words[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = file;
      t.r4.u64 = record;
      (v == 0 ? __imp__sub_822A8568 : NativeParseFile)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// PixelShader::Init: the object anywhere in the scratch area; the scratch
// area.
void PixelShaderInits(Context& h) {
  Results results("PixelShader::Init (sub_822A85B8)");
  for (int c = 0; c < 200; c++) {
    const uint32_t object = h.data + 4 * h.random.Below(900);
    const uint32_t microcode = uint32_t(h.random.Next());
    std::vector<uint8_t> fill(kScratchBytes);
    for (auto& b : fill) {
      b = uint8_t(h.random.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, h.data, fill.data(), kScratchBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = object;
      t.r4.u64 = microcode;
      (v == 0 ? __imp__sub_822A85B8 : NativePixelShaderInit)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    }
    results.Check(out[0], out[1], fmt::format("case {} (object at +{})", c, object - h.data));
  }
  results.Report();
}

// VertexShader::Init: an object with its header after it in a 16 KB block,
// the microcode in a 48 KB block of virtual or physical memory; fetch counts
// 0 to 32, and over it, a table inside the fields and microcode inside the
// object (the original's on the native side); the blocks compared.
void VertexShaderInits(Context& h) {
  Results results("VertexShader::Init (sub_822A8740)");
  constexpr uint32_t kObjectBytes = 16 * 1024;
  constexpr uint32_t kMicrocodeBytes = 48 * 1024 + 16;
  const uint32_t block = memory::Alloc(kObjectBytes, 0);
  const uint32_t virtual_code = memory::Alloc(kMicrocodeBytes, 0);
  const uint32_t physical_code = memory::AllocPhysical(kMicrocodeBytes, 0, 0xFFFFFFFFu, 4096, 0x404);
  for (int c = 0; c < 600; c++) {
    const uint32_t object = block + 4 * h.random.Below(64);
    uint32_t microcode = (c & 1) ? physical_code : virtual_code;
    uint32_t count = h.random.Below(33);
    uint32_t header = 4 * h.random.Below(256);  // the table's offset after the fields
    uint32_t first = h.random.Below(64);        // the table's +24
    std::string what = fmt::format("case {} count {}", c, count);
    if (c % 50 == 7) {
      count = 33 + h.random.Below(8);
      what += " (over 32)";
    }
    if (c % 50 == 11) {
      // Wholly inside the fields (its count cleared before the original
      // reads it), not over the header offset at +612.
      header = uint32_t(-int32_t(4 * (8 + h.random.Below(100))));
      what += " (the table inside the fields)";
    }
    if (c % 50 == 13) {
      microcode = object + 4;
      what += " (the microcode inside the object)";
    }
    const uint32_t table = object + so::kVertexShaderBytes + header;
    std::vector<uint32_t> words(count);
    for (auto& w : words) {
      w = uint32_t(h.random.Next());
      if (microcode == object + 4) {
        w &= ~0xFFFu;  // the microcode's start: inside the object
      } else if (c % 4 != 0) {
        w = (w & ~0xFFFu) | h.random.Below(300);
      }
    }
    std::vector<uint8_t> fill(kObjectBytes);
    for (auto& b : fill) {
      b = uint8_t(h.random.Next());
    }
    std::vector<uint8_t> code(kMicrocodeBytes);
    for (auto& b : code) {
      b = uint8_t(h.random.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, block, fill.data(), kObjectBytes);
      if (microcode != object + 4) {
        Write(h.base, microcode, code.data(), kMicrocodeBytes);
      }
      Store(h.base, object + 612, header);
      if (table + 32 <= block + kObjectBytes && table >= block) {
        Store(h.base, table + 24, first);
        Store(h.base, table + 28, count);
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t at = table + 4 * (first + 10) + 4 * i;
          if (at + 4 <= block + kObjectBytes) {
            Store(h.base, at, words[i]);
          }
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = object;
      t.r4.u64 = microcode;
      (v == 0 ? __imp__sub_822A8740 : NativeVertexShaderInit)(t, h.base);
      out[v].Bytes(Read(h.base, block, kObjectBytes));
      if (microcode != object + 4) {
        out[v].Bytes(Read(h.base, microcode, kMicrocodeBytes));
      }
    }
    results.Check(out[0], out[1], what);
  }
  results.Report();
  memory::FreePhysical(physical_code);
  memory::Free(virtual_code);
  memory::Free(block);
}

// --- The header copies, the texture relocation and the creates ---

void NativeCopyVertexHeader(PPCContext& ctx, uint8_t* base) {
  if (!so::CopyVertexHeader(base, ctx.r3.u32, ctx.r5.u32)) {
    __imp__sub_822A9FD8(ctx, base);
  }
}
void NativeCopyPixelHeader(PPCContext& ctx, uint8_t* base) {
  if (!so::CopyPixelHeader(base, ctx.r3.u32, ctx.r5.u32)) {
    __imp__sub_822A9EA8(ctx, base);
  }
}
void NativeRelocateTexture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::RelocateTexture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
struct InitCall {
  PPCContext ctx;
  uint8_t* base;
};
void InitOriginal(void* user, uint32_t object, uint32_t microcode) {
  auto& call = *static_cast<InitCall*>(user);
  call.ctx.r3.u64 = object;
  call.ctx.r4.u64 = microcode;
  __imp__sub_822A8740(call.ctx, call.base);
}
void NativeVertexShaderCreate(PPCContext& ctx, uint8_t* base) {
  InitCall call{ctx, base};
  so::CreateCalls calls;
  calls.init = &InitOriginal;
  calls.user = &call;
  if (const auto object = so::VertexShaderCreate(base, ctx.r3.u32, calls)) {
    ctx.r3.u64 = *object;
  } else {
    __imp__sub_822AA018(ctx, base);
  }
}
void NativePixelShaderCreate(PPCContext& ctx, uint8_t* base) {
  if (const auto object = so::PixelShaderCreate(base, ctx.r3.u32)) {
    ctx.r3.u64 = *object;
  } else {
    __imp__sub_822A9EE8(ctx, base);
  }
}

// The header copies: the object in a 4 KB block, the file (header sizes 0
// to 1,200, any alignment) and the record in another; both blocks compared.
// (A record inside the cleared bytes, the native side's fallback, would have
// the original copy from address 0.)
void HeaderCopies(Context& h) {
  constexpr uint32_t kBlockBytes = 4096;
  const uint32_t objects = memory::Alloc(kBlockBytes, 0);
  const uint32_t files = memory::Alloc(kBlockBytes, 0);
  for (int pixel = 0; pixel < 2; pixel++) {
    Results results(pixel ? "PixelShader::CopyHeader (sub_822A9EA8)"
                          : "VertexShader::CopyHeader (sub_822A9FD8)");
    const uint32_t fields = pixel ? so::kPixelShaderBytes : so::kVertexShaderBytes;
    for (int c = 0; c < 300; c++) {
      const uint32_t object = objects + 4 * h.random.Below(64);
      const uint32_t file = files + 64 + h.random.Below(1024);
      const uint32_t record = files + 4 * h.random.Below(14);
      const uint32_t bytes = c < 40 ? uint32_t(c) : h.random.Below(1200);
      std::vector<uint8_t> a(kBlockBytes), b(kBlockBytes);
      for (auto& x : a) x = uint8_t(h.random.Next());
      for (auto& x : b) x = uint8_t(h.random.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, objects, a.data(), kBlockBytes);
        Write(h.base, files, b.data(), kBlockBytes);
        const uint32_t at = record;
        Store(h.base, at, file);
        Store(h.base, file + 4, bytes);
        PPCContext t = CallContext(h);
        t.r3.u64 = object;
        t.r4.u64 = uint32_t(h.random.Next());
        t.r5.u64 = at;
        if (pixel) {
          (v == 0 ? __imp__sub_822A9EA8 : NativeCopyPixelHeader)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_822A9FD8 : NativeCopyVertexHeader)(t, h.base);
        }
        out[v].Bytes(Read(h.base, objects, kBlockBytes));
        out[v].Bytes(Read(h.base, files, kBlockBytes));
      }
      results.Check(out[0], out[1],
                    fmt::format("case {} ({} header bytes)", c, bytes));
    }
    results.Report();
  }
  memory::Free(files);
  memory::Free(objects);
}

void TextureRelocations(Context& h) {
  Results results("TextureHeader::Relocate (sub_8235A990, with sub_822A5B68)");
  for (int c = 0; c < 400; c++) {
    const uint32_t header = h.data + 4 * h.random.Below(900);
    const uint32_t a = uint32_t(h.random.Next()), b = uint32_t(h.random.Next());
    std::vector<uint8_t> fill(kScratchBytes);
    for (auto& x : fill) x = uint8_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, h.data, fill.data(), kScratchBytes);
      if (c % 4 == 0) {
        Store(h.base, header + 44, Load(h.base, header + 44) & ~0x3C0u);  // one level
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = header;
      t.r4.u64 = c % 3 == 0 ? (a & 0xFFFFF000u) : a;
      t.r5.u64 = b;
      (v == 0 ? __imp__sub_8235A990 : NativeRelocateTexture)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The creates: a shader file in a block (a header of 280 to 1,000 bytes
// holding a fetch table of 0 to 32 words, or 40, for a vertex shader;
// microcode of 12 bytes to 6 KB
// referenced by the fetches), the original and the native create from it;
// the object's bytes (its microcode pointer replaced by a marker), the
// microcode's bytes and r3 compared, then both freed.
void Creates(Context& h) {
  constexpr uint32_t kFileBytes = 16 * 1024;
  constexpr uint32_t kMarker = 0x4D435245;
  const uint32_t files = memory::Alloc(kFileBytes, 0);
  for (int pixel = 0; pixel < 2; pixel++) {
    Results results(pixel ? "PixelShader::Create (sub_822A9EE8)"
                          : "VertexShader::Create (sub_822AA018)");
    for (int c = 0; c < 150; c++) {
      const uint32_t file = files + 4 * h.random.Below(16);
      const uint32_t header = 280 + 4 * h.random.Below(180);
      const uint32_t code_bytes = 12 + 12 * h.random.Below(512);
      uint32_t magic = uint32_t(h.random.Next());
      if (pixel ? c % 5 != 0 : true) {
        magic = pixel ? (magic & ~0x3Fu) : (magic | 1u);
      }
      std::vector<uint8_t> fill(kFileBytes);
      for (auto& x : fill) x = uint8_t(h.random.Next());
      const uint32_t table = 32 + 4 * h.random.Below(4);  // the header's +20
      const uint32_t first = h.random.Below(8);
      const uint32_t count = c % 30 == 7 ? 40 : h.random.Below(33);
      std::vector<uint32_t> words(count);
      for (auto& w : words) {
        w = (uint32_t(h.random.Next()) & ~0xFFFu) | h.random.Below(code_bytes / 12);
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, files, fill.data(), kFileBytes);
        Store(h.base, file + 0, magic);
        Store(h.base, file + 4, header);
        Store(h.base, file + 8, code_bytes);
        if (!pixel) {
          Store(h.base, file + 20, table);
          Store(h.base, file + table + 24, first);
          Store(h.base, file + table + 28, count);
          for (uint32_t i = 0; i < count; i++) {
            const uint32_t at = table + 4 * (first + 10 + i);
            if (at + 4 <= header) {
              Store(h.base, file + at, words[i]);
            }
          }
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = file;
        if (pixel) {
          (v == 0 ? __imp__sub_822A9EE8 : NativePixelShaderCreate)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_822AA018 : NativeVertexShaderCreate)(t, h.base);
        }
        const uint32_t object = t.r3.u32;
        out[v].Word(object != 0);
        if (object != 0) {
          const uint32_t fields = pixel ? so::kPixelShaderBytes : so::kVertexShaderBytes;
          const uint32_t pointer = pixel ? 24 : 48;
          const uint32_t code = Load(h.base, object + pointer);
          Store(h.base, object + pointer, kMarker);
          out[v].Bytes(Read(h.base, object, fields + header));
          out[v].Bytes(Read(h.base, code, code_bytes));
          memory::crt::XMemFree(code, so::kMicrocodeAttributes);
          memory::crt::XMemFree(object, so::kObjectAttributes);
        }
      }
      results.Check(out[0], out[1],
                    fmt::format("case {} (magic {:X}, header {}, microcode {}, fetches {})", c,
                                magic, header, code_bytes, count));
    }
    results.Report();
  }
  memory::Free(files);
}

// --- Resource::Relocate, XPR::Create, Mesh::LoadShaders ---

void Require(bool done) {
  if (!done) {
    ITC_LOG_ERROR("harness: shader_objects: a model's set-up or teardown declined");
    std::abort();
  }
}

void NativeRelocateResource(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::RelocateResource(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeXprCreate(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = models::XprCreate(base, ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    __imp__sub_82596F68(ctx, base);
  }
}
void NativeLoadShaders(PPCContext& ctx, uint8_t* base) {
  InitCall call{ctx, base};
  so::CreateCalls calls;
  calls.init = &InitOriginal;
  calls.user = &call;
  if (!so::LoadShaders(base, ctx.r3.u32, ctx.r4.u32, calls)) {
    __imp__sub_82594D20(ctx, base);
  }
}

// A memory block's pages (Memory::AllocVirtual's, AllocPhysicalEx's or
// AllocPhysical's) given back.
void FreeBlock(uint32_t block, int kind) {
  if (block == 0) {
    return;
  }
  const uint32_t header = block - models::kBlockHeaderBytes;
  if (kind == 0) {
    memory::FreePages(header);
  } else if (kind == 1) {
    memory::FreePhysical(header);
  } else {
    memory::crt::XMemFree(header, models::kXprDataAllocation);
  }
}

// Resource::Relocate: resources of every type (a texture's dimensions and
// arrays, a surface with and without its parent flag, the parent in the
// scratch area) with random words; the scratch area and r3.
void ResourceRelocations(Context& h) {
  Results results("Resource::Relocate (sub_8235A9F8)");
  for (int c = 0; c < 600; c++) {
    const uint32_t resource = h.data + 4 * h.random.Below(400);
    const uint32_t parent = h.data + 2048 + 4 * h.random.Below(400);
    const uint32_t type = c < 32 ? uint32_t(c & 15) : h.random.Below(16);
    std::vector<uint8_t> fill(kScratchBytes);
    for (auto& x : fill) x = uint8_t(h.random.Next());
    const uint32_t address = c % 3 == 0 ? (uint32_t(h.random.Next()) & ~0xFFFu)
                                        : uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, h.data, fill.data(), kScratchBytes);
      uint32_t common = (Load(h.base, resource) & ~0xFu) | type;
      if (type == 4 && c % 2 == 0) {
        common &= ~0x40000000u;
      }
      Store(h.base, resource, common);
      if (type == 4) {
        Store(h.base, resource + 24, parent);
      }
      if (c % 5 == 0) {
        Store(h.base, resource + 48, Load(h.base, resource + 48) & 0xFFFu);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = resource;
      t.r4.u64 = address;
      (v == 0 ? __imp__sub_8235A9F8 : NativeRelocateResource)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} (type {})", c, type));
  }
  results.Report();
}

// XPR::Create: XPR2 files of 0 to 24 records (textures and others, their
// headers in the header block) with random words, and files that aren't
// XPR2. The object's words, and the blocks' bytes with the pointers the
// function makes taken back to offsets (the blocks sit at different
// addresses on each side), and r3; then the blocks freed.
void XprCreates(Context& h) {
  Results results("XPR::Create (sub_82596F68)");
  constexpr uint32_t kFileBytes = 32 * 1024;
  const uint32_t files = memory::Alloc(kFileBytes, 0);
  for (int c = 0; c < 120; c++) {
    const uint32_t records = h.random.Below(25);
    const uint32_t textures_at = 4 + 16 * records;
    const uint32_t header_bytes = textures_at + 52 * records + 4 * h.random.Below(64);
    const uint32_t data_bytes = c % 10 == 0 ? 0 : 1 + h.random.Below(8000);
    std::vector<uint8_t> fill(kFileBytes);
    for (auto& x : fill) x = uint8_t(h.random.Next());
    std::vector<bool> texture(records);
    for (uint32_t i = 0; i < records; i++) {
      texture[i] = h.random.Below(3) != 0;
    }
    const bool xpr2 = c % 17 != 5;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, files, fill.data(), kFileBytes);
      const uint32_t file = files;
      Store(h.base, file + 0, xpr2 ? 0x58505232 : 0x58505231);
      Store(h.base, file + 4, header_bytes);
      Store(h.base, file + 8, data_bytes);
      const uint32_t header = file + 12;
      Store(h.base, header, records);
      for (uint32_t i = 0; i < records; i++) {
        const uint32_t record = header + 4 + 16 * i;
        const uint32_t tag = Load(h.base, record) & 0xFFFFu;
        Store(h.base, record, (texture[i] ? 0x54580000u : 0x56420000u) | tag);
        Store(h.base, record + 4, textures_at + 52 * i);
      }
      const uint32_t xpr = h.data + 64;
      Fill(h.base, h.data, kScratchBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = xpr;
      t.r4.u64 = file;
      (v == 0 ? __imp__sub_82596F68 : NativeXprCreate)(t, h.base);
      out[v].Word(t.r3.u32);
      if (!xpr2) {
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        continue;
      }
      const uint32_t block = Load(h.base, xpr + 0), data = Load(h.base, xpr + 8);
      out[v].Word(Load(h.base, xpr + 4));
      out[v].Word(Load(h.base, xpr + 12));
      out[v].Word(Load(h.base, xpr + 16) - block);
      out[v].Word(Load(h.base, xpr + 20));
      std::vector<uint8_t> bytes = Read(h.base, block, header_bytes);
      auto word = [&](uint32_t offset, uint32_t minus) {
        const uint32_t w = Load(h.base, block + offset) - minus;
        for (int k = 0; k < 4; k++) bytes[offset + k] = uint8_t(w >> (24 - 8 * k));
      };
      for (uint32_t i = 0; i < records; i++) {
        word(4 + 16 * i + 12, block);
        if (texture[i]) {
          const uint32_t at = textures_at + 52 * i;
          word(at + 0x20, data);
          if (((Load(h.base, block + at + 44) >> 6) & 0xF) != 0) {
            word(at + 0x30, data);
          }
        }
      }
      out[v].Bytes(bytes);
      out[v].Bytes(Read(h.base, data, data_bytes));
      FreeBlock(block, 0);
      FreeBlock(data, 2);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {} ({} records, {} header bytes, {} data bytes{})", c, records,
                              header_bytes, data_bytes, xpr2 ? "" : ", not XPR2"));
  }
  memory::Free(files);
  results.Report();
}

// Mesh::LoadShaders: shader sections of 0 to 6 pairs (vertex files with
// fetch tables of 0 to 32 words, and 40; pixel files; microcode of 12 bytes
// to 3 KB) in a block, the model in another; the model's +420, the pairs'
// objects (their microcode pointers replaced by a marker) and microcode by
// their contents; then the blocks freed.
void LoadShaderSections(Context& h) {
  Results results("Mesh::LoadShaders (sub_82594D20)");
  constexpr uint32_t kSectionBytes = 64 * 1024;
  constexpr uint32_t kMarker = 0x4D435245;
  const uint32_t sections = memory::Alloc(kSectionBytes, 0);
  const uint32_t models_block = memory::Alloc(1024, 0);
  for (int c = 0; c < 60; c++) {
    const uint32_t pairs = c < 3 ? uint32_t(c) : h.random.Below(7);
    std::vector<uint8_t> fill(kSectionBytes);
    for (auto& x : fill) x = uint8_t(h.random.Next());
    std::vector<uint8_t> model_fill(1024);
    for (auto& x : model_fill) x = uint8_t(h.random.Next());
    struct Shader {
      uint32_t header, code_bytes, magic, count, first, table;
      std::vector<uint32_t> words;
    };
    std::vector<Shader> shaders(2 * pairs);
    for (uint32_t k = 0; k < 2 * pairs; k++) {
      Shader& sh = shaders[k];
      const bool vertex = k % 2 == 0;
      sh.header = 280 + 4 * h.random.Below(60);
      sh.code_bytes = 12 + 12 * h.random.Below(256);
      sh.magic = uint32_t(h.random.Next());
      sh.magic = vertex ? (sh.magic | 1u) : (sh.magic & ~0x3Fu);
      sh.table = 32 + 4 * h.random.Below(4);
      sh.first = h.random.Below(8);
      sh.count = (c % 20 == 9 && vertex) ? 40 : h.random.Below(33);
      sh.words.resize(sh.count);
      for (auto& w : sh.words) {
        w = (uint32_t(h.random.Next()) & ~0xFFFu) | h.random.Below(sh.code_bytes / 12);
      }
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, sections, fill.data(), kSectionBytes);
      Write(h.base, models_block, model_fill.data(), 1024);
      const uint32_t section = sections;
      Store(h.base, section, pairs);
      uint32_t pair = section + 32;
      for (uint32_t i = 0; i < pairs; i++) {
        uint32_t file = pair + 32;
        uint32_t sizes[2];
        for (int k = 0; k < 2; k++) {
          const Shader& sh = shaders[2 * i + k];
          Store(h.base, file + 0, sh.magic);
          Store(h.base, file + 4, sh.header);
          Store(h.base, file + 8, sh.code_bytes);
          if (k == 0) {
            Store(h.base, file + 20, sh.table);
            Store(h.base, file + sh.table + 24, sh.first);
            Store(h.base, file + sh.table + 28, sh.count);
            for (uint32_t w = 0; w < sh.count; w++) {
              Store(h.base, file + sh.table + 4 * (sh.first + 10 + w), sh.words[w]);
            }
          }
          sizes[k] = sh.header + sh.code_bytes;
          file += sizes[k];
        }
        Store(h.base, pair + 4, sizes[0]);
        Store(h.base, pair + 8, sizes[1]);
        pair = file;
      }
      const uint32_t model = models_block + 16;
      PPCContext t = CallContext(h);
      t.r3.u64 = model;
      t.r4.u64 = section;
      (v == 0 ? __imp__sub_82594D20 : NativeLoadShaders)(t, h.base);
      out[v].Word(Load(h.base, model + 420));
      const uint32_t block = Load(h.base, model + 24);
      for (uint32_t i = 0; i < pairs; i++) {
        for (int k = 0; k < 2; k++) {
          const Shader& sh = shaders[2 * i + k];
          const uint32_t object = Load(h.base, block + 16 * i + 4 * k);
          const uint32_t code = Load(h.base, block + 16 * i + 8 + 4 * k);
          const uint32_t fields = k == 0 ? so::kVertexShaderBytes : so::kPixelShaderBytes;
          const uint32_t pointer = k == 0 ? 48 : 24;
          out[v].Word(Load(h.base, object + pointer) == code);
          Store(h.base, object + pointer, kMarker);
          out[v].Bytes(Read(h.base, object, fields + sh.header));
          out[v].Bytes(Read(h.base, code, sh.code_bytes));
          FreeBlock(object, 0);
          FreeBlock(code, 1);
        }
      }
      if (pairs != 0) {
        FreeBlock(block, 0);
        Store(h.base, model + 24, 0);
      }
      out[v].Bytes(Read(h.base, models_block, 1024));
    }
    results.Check(out[0], out[1], fmt::format("case {} ({} pairs)", c, pairs));
  }
  memory::Free(models_block);
  memory::Free(sections);
  results.Report();
}

// --- The skeleton's set-up and bind ---

void NativeSkeletonSetUp(PPCContext& ctx, uint8_t* base) {
  if (!model_animation::SkeletonSetUp(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32)) {
    __imp__sub_8259A9E0(ctx, base);
  }
}
void NativeSkeletonBind(PPCContext& ctx, uint8_t* base) {
  if (!model_animation::SkeletonBind(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_82599A20(ctx, base);
  }
}

// SkeletonBase::SetUp and Skeleton::Bind: a skeleton (192 bytes of random
// words, with arrays from an earlier set-up or none) and a node file of 0 to
// 20 nodes and 0 to 10 entries; instance counts -1 to 3; matrices or none.
// The skeleton's words with its arrays compared by their contents (they sit
// at different addresses on each side), then freed.
void SkeletonSetUps(Context& h) {
  constexpr uint32_t kSkeletonBytes = 192;
  for (int bind = 0; bind < 2; bind++) {
    Results results(bind ? "Skeleton::Bind (sub_82599A20)"
                         : "SkeletonBase::SetUp (sub_8259A9E0, with NodeFile::View sub_825A21A8)");
    for (int c = 0; c < 200; c++) {
      const uint32_t nodes = c % 9 == 0 ? 0 : h.random.Below(21);
      const uint32_t entries = c % 7 == 0 ? 0 : h.random.Below(11);
      const int32_t instances = int32_t(h.random.Below(5)) - 1;
      const bool with_matrices = c % 3 == 0;
      const bool earlier = c % 2 == 1;
      const uint32_t chosen = h.random.Below(nodes + 1);
      std::vector<uint32_t> words(kSkeletonBytes / 4), file_words(64);
      for (auto& w : words) w = uint32_t(h.random.Next());
      for (auto& w : file_words) w = uint32_t(h.random.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t skeleton = h.data;
        const uint32_t file = h.data + 1024;
        for (uint32_t i = 0; i < words.size(); i++) Store(h.base, skeleton + 4 * i, words[i]);
        for (uint32_t i = 0; i < file_words.size(); i++) Store(h.base, file + 4 * i, file_words[i]);
        Store(h.base, file + 4, nodes);
        Store(h.base, file + 8, entries);
        Store(h.base, file + 12, chosen);
        for (uint32_t offset : {128u, 132u, 136u, 144u, 176u}) {
          Store(h.base, skeleton + offset, 0);
        }
        if (earlier) {
          Store(h.base, skeleton + 128, *memory::crt::OperatorNewArray(h.base, 40));
          Store(h.base, skeleton + 136, *memory::crt::OperatorNewArray(h.base, 16 + 144) + 16);
          Store(h.base, skeleton + 176, *memory::crt::OperatorNewArray(h.base, 64));
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = skeleton;
        t.r4.u64 = file;
        t.r5.u64 = with_matrices ? h.data + 2048 : 0;
        t.r6.s64 = instances;
        if (bind) {
          (v == 0 ? __imp__sub_82599A20 : NativeSkeletonBind)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_8259A9E0 : NativeSkeletonSetUp)(t, h.base);
        }
        // The arrays by their contents, their words replaced by 0.
        const uint32_t count = Load(h.base, skeleton + 160);
        struct Array {
          uint32_t offset, bytes, header;
        };
        const Array arrays[] = {{128, 20 * count, 0},
                                {136, 144 * nodes, 16},
                                {132, 64 * nodes, 16},
                                {144, 64 * entries, 16},
                                {176, 16 * nodes, 0}};
        for (const Array& a : arrays) {
          const uint32_t at = Load(h.base, skeleton + a.offset);
          out[v].Word(at != 0);
          if (at != 0 && !(a.offset == 176 && !bind && earlier)) {
            if (a.header) out[v].Word(Load(h.base, at - 16));
            out[v].Bytes(Read(h.base, at, a.bytes));
          }
        }
        const uint32_t nodes_at = Load(h.base, skeleton + 136);
        out[v].Word(Load(h.base, skeleton + 140) - nodes_at);
        const uint32_t matrices_at = Load(h.base, skeleton + 148);
        out[v].Word(matrices_at == Load(h.base, skeleton + 132) ? 1 : matrices_at);
        std::vector<uint8_t> rest = Read(h.base, skeleton, kSkeletonBytes);
        for (uint32_t offset : {128u, 132u, 136u, 140u, 144u, 148u, 176u}) {
          for (int k = 0; k < 4; k++) rest[offset + k] = 0;
        }
        out[v].Bytes(rest);
        for (const Array& a : arrays) {
          const uint32_t at = Load(h.base, skeleton + a.offset);
          if (at != 0) memory::crt::OperatorDeleteArray(at - a.header);
        }
      }
      results.Check(out[0], out[1],
                    fmt::format("case {} ({} nodes, {} entries, {} instances{}{})", c, nodes,
                                entries, instances, with_matrices ? ", matrices" : "",
                                earlier ? ", arrays before" : ""));
    }
    results.Report();
  }
}

// --- Model::Load (with Mesh::Parse) ---

struct LoadCall {
  PPCContext ctx;
  uint8_t* base;
};
void LoadCallFunction(void* user, uint32_t function, uint32_t r3, uint32_t r4, uint32_t r5,
                      uint32_t) {
  auto& call = *static_cast<LoadCall*>(user);
  PPCContext& ctx = call.ctx;
  uint8_t* base = call.base;
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.r5.u64 = r5;
  REX_CALL_INDIRECT_FUNC(function);
}
void LoadCallInit(void* user, uint32_t object, uint32_t microcode) {
  auto& call = *static_cast<LoadCall*>(user);
  call.ctx.r3.u64 = object;
  call.ctx.r4.u64 = microcode;
  __imp__sub_822A8740(call.ctx, call.base);
}
void NativeModelLoad(PPCContext& ctx, uint8_t* base) {
  LoadCall call{ctx, base};
  models::LoadCalls calls;
  calls.init = &LoadCallInit;
  calls.call = &LoadCallFunction;
  calls.user = &call;
  const uint32_t model = ctx.r3.u32;
  if (models::ModelLoad(base, model, ctx.r4.u32, ctx.r5.u32, calls)) {
    ctx.r3.u64 = Load(base, model + 412);
  } else {
    __imp__sub_82596920(ctx, base);
  }
}

// A side's blocks, for comparing their bytes with the pointers into them
// made offsets.
struct Region {
  uint32_t start, bytes;
};

void AddNormalized(Outputs& out, uint8_t* base, const std::vector<Region>& regions,
                   uint32_t start, uint32_t bytes) {
  std::vector<uint8_t> data = Read(base, start, bytes);
  for (uint32_t at = 0; at + 4 <= bytes; at += 4) {
    const uint32_t word = Load(base, start + at);
    for (size_t r = 0; r < regions.size(); r++) {
      if (word >= regions[r].start && word - regions[r].start < regions[r].bytes + 16) {
        const uint32_t marker = 0xB0000000u | (uint32_t(r) << 20) | (word - regions[r].start);
        for (int k = 0; k < 4; k++) data[at + k] = uint8_t(marker >> (24 - 8 * k));
        break;
      }
    }
  }
  out.Bytes(data);
}

// Model::Load: a model made by Model::Model at one address for both sides,
// and a part file built in a block: 0 to 3 shapes (resource headers of
// types 1, 2, 3 and 8, materials, skinned or not with their weights), a
// shader section of 0 to 2 pairs, vertex data; with or without the skeleton
// (a node file, track sections with no tracks), with or without an XPR2 file
// (a texture), a wrong version now and then. The model's 432 bytes and every
// block it holds, with the pointers into those blocks made offsets (they sit
// at different addresses on each side); then all freed.
void ModelLoads(Context& h) {
  Results results("Model::Load (sub_82596920, with Mesh::Parse sub_825963F0)");
  constexpr uint32_t kModelBytes = 432;
  constexpr uint32_t kFileBytes = 64 * 1024;
  constexpr uint32_t kHeader = 0x4000;   // the header block's bytes
  constexpr uint32_t kVertices = 0x800;  // the vertex data's
  const uint32_t model = memory::Alloc(kModelBytes, 0);
  const uint32_t files = memory::Alloc(kFileBytes, 0);
  const uint32_t xprs = memory::Alloc(4096, 0);
  auto store = [&](uint32_t at, uint32_t value) { Store(h.base, at, value); };
  auto storef = [&](uint32_t at, float value) { Store(h.base, at, std::bit_cast<uint32_t>(value)); };
  for (int c = 0; c < 80; c++) {
    const uint32_t shapes = c < 4 ? uint32_t(c) : h.random.Below(4);
    const uint32_t pairs = h.random.Below(3);
    const bool skeleton = c % 3 == 1;
    const bool with_xpr = c % 2 == 0;
    const bool wrong_version = c % 13 == 6;
    uint32_t nodes = 0, entries = 0;
    std::vector<uint32_t> code_sizes;
    // The file's words, built once and written for both sides.
    std::vector<uint32_t> file(kFileBytes / 4, 0);
    auto put = [&](uint32_t at, uint32_t value) { file[at / 4] = value; };
    auto putf = [&](uint32_t at, float value) { put(at, std::bit_cast<uint32_t>(value)); };
    put(0, wrong_version ? 0x00140000 : 0x00150000);
    put(4, 7);
    put(8, 9);
    put(12, shapes);
    put(16, 3);
    put(20, 0x80);
    put(24, 0x90);
    put(28, 0x100);  // the shapes
    put(32, 0xA0);
    put(64, kHeader);
    put(68, kVertices);
    put(72, kHeader + kVertices);  // the shader section
    put(76, skeleton ? 1 : 0);
    // Sections with no tracks, and the node file.
    put(44, 0x2000);
    if (skeleton) {
      put(36, 0x2100);
      put(48, 0x2040);
      put(52, 0x2060);
      put(56, 0x2080);
      put(60, 0x20A0);
      nodes = 1 + h.random.Below(4);
      entries = h.random.Below(3);
      put(0x2100 + 4, nodes);
      put(0x2100 + 8, entries);
      put(0x2100 + 12, h.random.Below(nodes));
      for (uint32_t k = 16; k <= 48; k += 4) put(0x2100 + k, 0x40 + k);
    }
    uint32_t free_at = 0x2400;  // materials and weights from here
    for (uint32_t i = 0; i < shapes; i++) {
      const uint32_t shape = 0x100 + 544 * i;
      const uint32_t types[] = {1, 2, 3, 8};
      for (uint32_t r : {288u, 320u, 352u}) {
        put(shape + r, (uint32_t(h.random.Next()) & 0xFFFFFF00u & ~0x40000000u) |
                           types[h.random.Below(4)]);
        put(shape + r + 24, uint32_t(h.random.Next()) & 0x0FFFFFFFu);
        put(shape + r + 28, uint32_t(h.random.Next()) & 0x0FFFFFFFu);
        put(shape + r + 44, h.random.Below(1024));
        put(shape + r + 48, uint32_t(h.random.Next()) & 0x0FFFFFFFu);
      }
      put(shape + 384, kHeader + 16 * h.random.Below(64));
      put(shape + 388, kHeader + 16 * h.random.Below(64));
      put(shape + 392, kHeader + 16 * h.random.Below(64));
      const uint32_t skinned = h.random.Below(3) == 0 ? 0 : 1;
      put(shape + 428, skinned);
      // The materials.
      const uint32_t materials = free_at;
      const uint32_t count = h.random.Below(3);
      put(shape + 396, materials);
      put(materials, count);
      for (uint32_t j = 0; j < count; j++) {
        const uint32_t entry = materials + 512 * j;
        put(entry + 368, (uint32_t(h.random.Next()) & 0xFFFFFF00u) | types[h.random.Below(4)]);
        put(entry + 368 + 24, uint32_t(h.random.Next()) & 0x0FFFFFFFu);
        put(entry + 368 + 48, uint32_t(h.random.Next()) & 0x0FFFFFFFu);
        put(entry + 400, 16 * h.random.Below(64));
      }
      free_at += 16 + 512 * 3;
      put(shape + 400, 0x60);
      put(shape + 404, 0x70);
      // The weights: vertices, their counts and floats.
      const uint32_t weights = free_at;
      const uint32_t vertices = h.random.Below(5);
      put(shape + 408, weights);
      put(weights + 0, vertices);
      put(weights + 8, weights + 32);
      uint32_t data = weights + 32 + 4 * vertices;
      put(weights + 12, data);
      put(weights + 16, 0x50);
      for (uint32_t v = 0; v < vertices; v++) {
        const uint32_t n = 1 + h.random.Below(4);
        put(weights + 32 + 4 * v, n);
        for (uint32_t k = 0; k < n; k++) {
          for (uint32_t l = 0; l < 4; l++) {
            putf(data + 16 * k + 4 * l, float(h.random.Below(1u << 20)) / float(1u << 20));
          }
        }
        data += 16 * n;
      }
      free_at = (data + 15) & ~15u;
    }
    // The shader section after the vertex data.
    {
      uint32_t at = kHeader + kVertices;
      put(at, pairs);
      uint32_t pair = at + 32;
      for (uint32_t i = 0; i < pairs; i++) {
        uint32_t f = pair + 32;
        uint32_t sizes[2];
        for (int k = 0; k < 2; k++) {
          const uint32_t header = 280;
          const uint32_t code = 12 * (1 + h.random.Below(40));
          put(f + 0, k == 0 ? 0x11u : 0x40u);
          put(f + 4, header);
          put(f + 8, code);
          code_sizes.push_back(code);
          if (k == 0) {
            put(f + 20, 32);
            put(f + 32 + 24, 0);
            const uint32_t fetches = h.random.Below(8);
            put(f + 32 + 28, fetches);
            for (uint32_t w = 0; w < fetches; w++) {
              put(f + 32 + 40 + 4 * w, h.random.Below(code / 12));
            }
          }
          sizes[k] = header + code;
          f += sizes[k];
        }
        put(pair + 4, sizes[0]);
        put(pair + 8, sizes[1]);
        pair = f;
      }
    }
    for (uint32_t k = kHeader / 4; k < (kHeader + kVertices) / 4; k++) {
      file[k] = uint32_t(h.random.Next()) & 0x0FFFFFFFu;
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t k = 0; k < file.size(); k++) store(files + 4 * k, file[k]);
      // An XPR2 file with one texture record.
      for (uint32_t k = 0; k < 1024; k++) store(xprs + 4 * k, 0);
      store(xprs + 0, 0x58505232);
      store(xprs + 4, 256);
      store(xprs + 8, 4096 - 12 - 256);
      store(xprs + 12, 1);                     // the header block: one record
      store(xprs + 12 + 4, 0x54580000);        // TX
      store(xprs + 12 + 8, 64);                // its texture header
      store(xprs + 12 + 16, 128);              // the name
      store(xprs + 12 + 64 + 0x20, 0x00001003);
      store(xprs + 12 + 64 + 44, 0x80);
      store(xprs + 12 + 64 + 0x30, 0x00002005);
      Fill(h.base, model, kModelBytes, 0);
      Require(models::ModelConstruct(h.base, model));
      PPCContext t = CallContext(h);
      t.r3.u64 = model;
      t.r4.u64 = files;
      t.r5.u64 = with_xpr ? xprs : 0;
      (v == 0 ? __imp__sub_82596920 : NativeModelLoad)(t, h.base);
      out[v].Word(t.r3.u32);
      // The side's blocks: the model's, its shaders', its XPR's, its
      // skeleton's.
      std::vector<Region> regions;
      auto add = [&](uint32_t start, uint32_t bytes) {
        if (start != 0) regions.push_back({start, bytes});
      };
      add(Load(h.base, model + 16), kHeader);
      add(Load(h.base, model + 20), kVertices);
      const uint32_t pair_block = Load(h.base, model + 24);
      if (Load(h.base, model + 420) != 0) {
        add(pair_block, 16 * Load(h.base, model + 420));
        for (uint32_t i = 0; i < Load(h.base, model + 420); i++) {
          add(Load(h.base, pair_block + 16 * i + 0), so::kVertexShaderBytes + 280);
          add(Load(h.base, pair_block + 16 * i + 4), so::kPixelShaderBytes + 280);
          add(Load(h.base, pair_block + 16 * i + 8), code_sizes[2 * i]);
          add(Load(h.base, pair_block + 16 * i + 12), code_sizes[2 * i + 1]);
        }
      }
      add(Load(h.base, model + 56), 256);
      add(Load(h.base, model + 64), 4096 - 12 - 256);
      const uint32_t sk = model + 112;
      add(Load(h.base, sk + 128), 20);
      const uint32_t sizes[] = {64 * nodes, 144 * nodes, 64 * entries};
      const uint32_t offsets[] = {132u, 136u, 144u};
      for (int k = 0; k < 3; k++) {
        const uint32_t at = Load(h.base, sk + offsets[k]);
        if (at != 0) add(at - 16, 16 + sizes[k]);
      }
      add(Load(h.base, sk + 176), 16 * nodes);
      // The model's words, then each block's bytes.
      AddNormalized(out[v], h.base, regions, model, kModelBytes);
      for (const Region& r : regions) {
        AddNormalized(out[v], h.base, regions, r.start, r.bytes);
      }
      // Freed: the blocks, then the model's members.
      for (uint32_t i = 0; pair_block != 0 && i < Load(h.base, model + 420); i++) {
        FreeBlock(Load(h.base, pair_block + 16 * i + 0), 0);
        FreeBlock(Load(h.base, pair_block + 16 * i + 4), 0);
        FreeBlock(Load(h.base, pair_block + 16 * i + 8), 1);
        FreeBlock(Load(h.base, pair_block + 16 * i + 12), 1);
      }
      if (Load(h.base, model + 420) != 0) FreeBlock(pair_block, 0);
      FreeBlock(Load(h.base, model + 16), 0);
      FreeBlock(Load(h.base, model + 20), 1);
      FreeBlock(Load(h.base, model + 56), 0);
      FreeBlock(Load(h.base, model + 64), 2);
      for (uint32_t offset : {16u, 20u, 24u, 420u, 56u, 64u}) store(model + offset, 0);
      Require(models::ModelDestruct(h.base, model).has_value());
    }
    results.Check(out[0], out[1],
                  fmt::format("case {} ({} shapes, {} shader pairs{}{}{})", c, shapes, pairs,
                              skeleton ? ", skeleton" : "", with_xpr ? ", XPR" : "",
                              wrong_version ? ", wrong version" : ""));
  }
  memory::Free(xprs);
  memory::Free(files);
  memory::Free(model);
  results.Report();
}

// --- The memory blocks ---

// The makers' register steps.
void NativeAllocVirtual(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::AllocVirtual(base, ctx.r3.u32);
}
void NativeAllocPhysicalEx(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::AllocPhysicalEx(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                       ctx.r7.u32);
}
void NativeAllocPhysical(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::AllocPhysical(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeFreeDue(PPCContext& ctx, uint8_t* base) {
  if (!models::FreeDue(base)) {
    __imp__sub_8258E1D8(ctx, base);
  }
}

// A block's header as the outputs: whether there's a block, its offset in
// its page, the header's words; then freed by `free`.
template <typename Free>
void MadeBlock(Outputs& out, uint8_t* base, uint32_t block, uint32_t alignment, Free free) {
  out.Word(block != 0);
  if (block == 0) {
    return;
  }
  const uint32_t header = block - models::kBlockHeaderBytes;
  out.Word(header % alignment);
  for (uint32_t i = 0; i < 16; i += 4) {
    out.Word(Load(base, header + i));
  }
  free(header);
}

void BlockMakers(Context& h) {
  const uint32_t sizes[] = {0, 1, 100, 4095, 4096, 4097, 8191, 8192, 65536, 300000};
  {
    Results results("Memory::AllocVirtual (sub_8258DFF0)");
    for (const uint32_t size : sizes) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.r3.u64 = size;
        (v == 0 ? __imp__sub_8258DFF0 : NativeAllocVirtual)(t, h.base);
        out[v].Word(t.r3.u32 != 0 && memory::IsPages(t.r3.u32 - models::kBlockHeaderBytes));
        MadeBlock(out[v], h.base, t.r3.u32, 4096, [](uint32_t header) {
          memory::FreePages(header);
        });
      }
      results.Check(out[0], out[1], fmt::format("{} bytes", size));
    }
    results.Report();
  }
  {
    // The calls seen in play (docs/functions/shader-objects.md).
    struct Args {
      uint32_t low, high, alignment, protection;
    };
    const Args args[] = {{0, 0xFFFFFFFFu, 32, 0x404}, {0, 0xFFFFFFFFu, 4, 0x404},
                         {0, 0xFFFFFFFFu, 4, 4},      {0, 0x1FFFFFFFu, 0, 4},
                         {0, 0xFFFFFFFFu, 2048, 4},   {0, 0xFFFFFFFFu, 4096, 0x20000004}};
    Results results("Memory::AllocPhysicalEx (sub_8258E098)");
    for (const uint32_t size : sizes) {
      for (const Args& a : args) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          PPCContext t = CallContext(h);
          t.r3.u64 = size;
          t.r4.u64 = a.low;
          t.r5.u64 = a.high;
          t.r6.u64 = a.alignment;
          t.r7.u64 = a.protection;
          (v == 0 ? __imp__sub_8258E098 : NativeAllocPhysicalEx)(t, h.base);
          MadeBlock(out[v], h.base, t.r3.u32, a.alignment > 4096 ? a.alignment : 4096,
                    [](uint32_t header) { memory::FreePhysical(header); });
        }
        results.Check(out[0], out[1],
                      fmt::format("{} bytes, alignment {}, protection {:X}", size, a.alignment,
                                  a.protection));
      }
    }
    results.Report();
  }
  {
    const uint32_t attributes[] = {0x24800000, 0x64800000, 0xA0000000, 0xB5800000, 0xC4000000,
                                   0xE0000000};
    Results results("Memory::AllocPhysical (sub_8258E138)");
    for (const uint32_t size : sizes) {
      for (const uint32_t a : attributes) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          PPCContext t = CallContext(h);
          t.r3.u64 = size;
          t.r4.u64 = a;
          (v == 0 ? __imp__sub_8258E138 : NativeAllocPhysical)(t, h.base);
          MadeBlock(out[v], h.base, t.r3.u32, (a & 0x80000000u) ? 4096 : 16,
                    [&](uint32_t header) { memory::crt::XMemFree(header, a); });
        }
        results.Check(out[0], out[1], fmt::format("{} bytes, attributes {:X}", size, a));
      }
    }
    results.Report();
  }
}

// PendingFrees::FreeDue on a list of the case's own swapped in for the
// game's (under the lock, put back after): blocks of each kind (0 pages, 1
// physical, 2 XMemAlloc's physical or heap blocks, 3 and above heap blocks
// left alone) with 0 to 3 passes left; the list's size, the nodes left (by
// their headers) and the headers left (their +0 and +8), and which
// blocks the layer still holds.
void FreesDue(Context& h) {
  Results results("PendingFrees::FreeDue (sub_8258E1D8)");
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  const uint32_t list = models::kPendingFrees;
  const uint32_t saved_head = Load(h.base, list + models::kListHead);
  const uint32_t saved_size = Load(h.base, list + models::kListSize);
  for (int c = 0; c < 120; c++) {
    const uint32_t count = h.random.Below(12);
    struct Plan {
      uint32_t kind, passes, attributes;
    };
    std::vector<Plan> plans(count);
    for (auto& p : plans) {
      p.kind = h.random.Below(6);
      p.passes = h.random.Below(4) == 0 ? 0 : h.random.Below(3);
      const uint32_t physical[] = {0x24800000, 0xA0000000, 0xB5800000};
      p.attributes = physical[h.random.Below(3)];
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      const uint32_t head = h.data;
      std::vector<uint32_t> headers(count), nodes(count);
      for (uint32_t i = 0; i < count; i++) {
        const Plan& p = plans[i];
        switch (p.kind) {
          case 0:
            headers[i] = memory::AllocPages(8192, 4096);
            break;
          case 1:
            headers[i] = memory::AllocPhysical(8192, 0, 0xFFFFFFFFu, 4096, 4);
            break;
          case 2:
            headers[i] = memory::crt::XMemAlloc(h.base, 8192, p.attributes);
            break;
          default:
            headers[i] = memory::Alloc(64, 0);
            break;
        }
        Store(h.base, headers[i] + 0, models::kBlockTag);
        Store(h.base, headers[i] + 4, p.kind);
        Store(h.base, headers[i] + 8, p.passes);
        Store(h.base, headers[i] + 12, p.attributes);
        nodes[i] = *memory::crt::OperatorNew(h.base, models::kNodeBytes);
        Store(h.base, nodes[i] + models::kNodeValue, headers[i]);
      }
      for (uint32_t i = 0; i <= count; i++) {
        const uint32_t node = i == 0 ? head : nodes[i - 1];
        const uint32_t next = i == count ? head : nodes[i];
        Store(h.base, node + models::kNodeNext, next);
        Store(h.base, next + models::kNodePrevious, node);
      }
      Store(h.base, list + models::kListHead, head);
      Store(h.base, list + models::kListSize, count);
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_8258E1D8 : NativeFreeDue)(t, h.base);
      out[v].Word(Load(h.base, list + models::kListSize));
      out[v].Word(Load(h.base, head + models::kNodePrevious) ==
                  (count == 0 ? head : Load(h.base, head + models::kNodePrevious)));
      std::vector<bool> left(count, false);
      uint32_t walked = 0, previous = head;
      for (uint32_t node = Load(h.base, head + models::kNodeNext); node != head && walked <= count;
           previous = node, node = Load(h.base, node + models::kNodeNext), walked++) {
        uint32_t index = count;
        for (uint32_t i = 0; i < count; i++) {
          if (nodes[i] == node) {
            index = i;
          }
        }
        out[v].Word(index);
        out[v].Word(Load(h.base, node + models::kNodePrevious) == previous);
        if (index < count) {
          left[index] = true;
        }
      }
      for (uint32_t i = 0; i < count; i++) {
        const Plan& p = plans[i];
        out[v].Word(left[i]);
        const bool heap_block = p.kind >= 3 || (p.kind == 2 && !(p.attributes & 0x80000000u));
        const bool held = p.kind == 0 ? memory::IsPages(headers[i])
                          : heap_block ? memory::IsHeapBlock(headers[i])
                                       : left[i] || p.passes != 0;
        out[v].Word(held);
        if (held) {
          out[v].Word(Load(h.base, headers[i] + 0));
          out[v].Word(Load(h.base, headers[i] + 8));
        }
        if (left[i]) {
          memory::crt::OperatorDelete(nodes[i]);
        }
        if (held) {
          if (p.kind == 0) {
            memory::FreePages(headers[i]);
          } else if (p.kind == 1) {
            memory::FreePhysical(headers[i]);
          } else if (p.kind == 2) {
            memory::crt::XMemFree(headers[i], p.attributes);
          } else {
            memory::Free(headers[i]);
          }
        }
      }
      Store(h.base, list + models::kListHead, saved_head);
      Store(h.base, list + models::kListSize, saved_size);
    }
    results.Check(out[0], out[1], fmt::format("case {} ({} blocks)", c, count));
  }
  results.Report();
}

}  // namespace

void ShaderObjects(Context& h) {
  ParseFiles(h);
  PixelShaderInits(h);
  VertexShaderInits(h);
  BlockMakers(h);
  FreesDue(h);
  HeaderCopies(h);
  TextureRelocations(h);
  Creates(h);
  ResourceRelocations(h);
  XprCreates(h);
  LoadShaderSections(h);
  SkeletonSetUps(h);
  ModelLoads(h);
}

}  // namespace harness
