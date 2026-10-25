// The shader objects (src/shader_objects/, docs/functions/shader-objects.md
// and models.md's shader file parse) against the recompiled functions. Each
// case fills its memory with random bytes, writes its inputs, runs the
// original and the plain C++ (through the register steps below, the
// original where it hands back) from the same inputs, and compares the
// memory exactly: nothing these functions write is computed as a float.

#include "harness.h"

#include "base/log.h"
#include "memory/heap.h"
#include "shader_objects/shader_objects.h"

#include <fmt/format.h>

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

}  // namespace

void ShaderObjects(Context& h) {
  ParseFiles(h);
  PixelShaderInits(h);
  VertexShaderInits(h);
}

}  // namespace harness
