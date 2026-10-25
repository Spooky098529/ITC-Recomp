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
#include "models/memory_blocks.h"
#include "models/xpr.h"
#include "shader_objects/shader_objects.h"

#include <fmt/format.h>

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
}

}  // namespace harness
