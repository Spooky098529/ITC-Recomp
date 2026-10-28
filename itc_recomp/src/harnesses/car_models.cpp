// The car models (src/car_models/, docs/functions/car-models.md) against the
// recompiled functions. Each case fills a block of guest memory with random
// bytes, writes its inputs, runs the original and the register steps
// (hooks/car_models.h) from the same inputs, and compares the block and r3.
// None of these functions loads or stores a float, so everything is
// compared exactly.

#include "harness.h"
#include "car_model_inputs.h"
#include "pending_frees.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_models/decals.h"
#include "car_models/raster.h"
#include "car_models/tga.h"
#include "car_models/vinyl.h"
#include "graphics/resources.h"
#include "hooks/car_models.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "models/memory_blocks.h"
#include "scene_loaders/objects.h"
#include "scene_loaders/pools.h"
#include "memory/heap.h"
#include "texture_compression/compress_surface.h"
#include "texture_compression/dxt45.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <limits>
#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_scene_loaders);
ITC_FLAG_DECLARE(bool, native_car_models);
ITC_FLAG_DECLARE(bool, native_texture_compression);

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x4000;
constexpr uint32_t kMarker = 0x50414745;  // a word that held the case's pages

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    Put(base, address + i, uint32_t(r.Next()));
  }
}

// One case through both versions: the block filled from `seed`, `setup(v,
// t)` writing the inputs and registers (from its own generator seeded the
// same for both), the function run, the block and r3 kept.
void Case(Context& h, Results& results, uint32_t block, uint64_t seed,
          const std::function<void(int, PPCContext&, Random&)>& setup,
          const std::function<void(int, Outputs&)>& take, GuestFunction recompiled,
          GuestFunction native, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    RandomBytes(h.base, block, kBlockBytes, r);
    PPCContext t = CallContext(h);
    setup(v, t, r);
    (v == 0 ? recompiled : native)(t, h.base);
    if (take) {
      take(v, out[v]);
    }
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Slots(Context& h, uint32_t block) {
  {
    Results results("LoadSlot::Reset (sub_820D0730)");
    for (int c = 0; c < 16; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](int, PPCContext& t, Random& r) { t.r3.u64 = block + 4 * r.Below(64); }, nullptr,
          __imp__sub_820D0730, hooks::CarModelsLoadSlotReset, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The buffer: none, or pages the allocation layer gives each side (the
    // word left compared as a marker while it's still the case's pages, and
    // whether they're still pages); shared or not.
    Results results("LoadSlot::ReleaseBuffer (sub_820D0758)");
    for (int c = 0; c < 32; c++) {
      uint32_t pages[2] = {0, 0};
      uint32_t slot = 0;
      Case(
          h, results, block, h.random.Next(),
          [&](int v, PPCContext& t, Random& r) {
            slot = block + 4 * r.Below(64);
            const bool shared = c % 4 >= 2 && r.Below(4) != 0;
            const bool buffer = c % 2 == 1 || r.Below(4) == 0;
            Put(h.base, slot + 12, shared ? (r.Below(2) ? 1 : uint32_t(r.Next()) | 1) : 0);
            pages[v] = buffer ? memory::AllocPages(1 + r.Below(0x8000), 4096) : 0;
            Put(h.base, slot + 16, pages[v]);
            t.r3.u64 = slot;
          },
          [&](int v, Outputs& out) {
            const uint32_t word = guest::LoadU32(h.base, slot + 16);
            if (pages[v] != 0 && word == pages[v]) {
              Put(h.base, slot + 16, kMarker);
            }
            const bool still = pages[v] != 0 && memory::IsPages(pages[v]);
            out.Word(still);
            if (still) {
              memory::FreePages(pages[v]);
            }
          },
          __imp__sub_820D0758, hooks::CarModelsLoadSlotReleaseBuffer, fmt::format("case {}", c));
    }
    results.Report();
  }
}

void DeferReleases(Context& h, uint32_t block) {
  Results results("DeferRelease (sub_820C3F50)");
  for (int c = 0; c < 64; c++) {
    Case(
        h, results, block, h.random.Next(),
        [&](int, PPCContext& t, Random& r) {
          const uint32_t set = block + 16 * r.Below(4);
          const uint32_t records = block + 0x100;
          // Counts: small mostly (up to 4 x 6 records), now and then ones
          // whose 4 x sum is 0 or negative as a signed word.
          uint32_t a = r.Below(4), b = r.Below(4);
          if (c % 8 == 7) {
            const uint32_t odd[] = {0x40000000u, 0x30000000u, 0xFFFFFFFFu, 0x80000000u};
            a = odd[r.Below(4)];
            b = r.Below(2);
          }
          Put(h.base, set + 40, a);
          Put(h.base, set + 12, b);
          Put(h.base, set + 52, records);
          // The records' states: in use mostly, now and then free.
          const uint32_t count = 4 * ((a + b) & 0x3FFFFFFFu);
          const uint32_t shown = count < 48 ? count : 48;
          for (uint32_t i = 0; i < shown; i++) {
            Put(h.base, records + 20 * i + 16, r.Below(5) == 0 ? 0 : 1 + r.Below(3));
          }
          t.r3.u64 = set;
          t.r4.u64 = r.Next();
          t.r5.u64 = r.Next();
          t.r6.u64 = r.Next();
          t.r7.u64 = r.Next();
        },
        nullptr, __imp__sub_820C3F50, hooks::CarModelsDeferRelease, fmt::format("case {}", c));
  }
  results.Report();
}

// CarModel::Release: the car model's inputs as for its reset
// (car_model_inputs.h), with the three slots' buffers (pages each side
// allocates, shared or not) and the set its sheets are put off into (+7256:
// records at random states, as DeferRelease's cases). The recompiled side
// with --native_car_models and --native_scene_loaders off. Compared as the
// resets are (harnesses/scene_loaders.cpp), the pages by whether they're
// still alive.
void Releases(Context& h, uint32_t block) {
  namespace cmi = car_model_inputs;
  namespace sl = scene_loaders;
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: car_models: --native_models was off at start-up: CarModel::Release's "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  const uint32_t scratch = block + 0x100;
  const uint32_t car = block + 0x1000;
  const uint32_t loads = car + sl::kCarModelLoads;
  const uint32_t set = block + 0x3000;
  const uint32_t records = set + 0x100;
  const uint32_t saved_set = guest::LoadU32(h.base, sl::kFirstSet);
  Results results("CarModel::Release (sub_820D4508, on the game's list)");
  for (int c = 0; c < 48; c++) {
    const uint64_t seed = h.random.Next();
    const cmi::Plan plan = cmi::MakePlan(h.random);
    const uint32_t first_set = uint32_t(h.random.Next());
    const uint32_t a = h.random.Below(3), b = h.random.Below(3);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random bytes(seed);
      RandomBytes(h.base, block, kBlockBytes, bytes);
      cmi::Held held;
      cmi::Write(h.base, loads, scratch, plan, true, held);
      Put(h.base, car + sl::kCarModelSet, set);
      Put(h.base, set + 40, a);
      Put(h.base, set + 12, b);
      Put(h.base, set + 52, records);
      for (uint32_t i = 0; i < 4 * (a + b); i++) {
        Put(h.base, records + 20 * i + 16, bytes.Below(4) == 0 ? 0 : 2);
      }
      Put(h.base, sl::kFirstSet, first_set);
      const pending_frees::ListState was = pending_frees::SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      t.r3.u64 = car;
      ITC_FLAG(native_scene_loaders) = v == 1;
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820D4508 : hooks::CarModelsCarModelRelease)(t, h.base);
      ITC_FLAG(native_scene_loaders) = true;
      ITC_FLAG(native_car_models) = true;
      pending_frees::TakeAddedNodes(h.base, was, out[v]);
      cmi::Take(h.base, loads, held, out[v]);
      out[v].Bytes(Read(h.base, block, kBlockBytes));
      out[v].Word(guest::LoadU32(h.base, sl::kFirstSet));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  Put(h.base, sl::kFirstSet, saved_set);
}

// The vinyl sheet's texture (car_models/vinyl.h). Each side allocates its
// own headers and data, so the words that hold one of a side's allocations
// are compared as which allocation and the offset into it.
struct Known {
  uint32_t address;
  uint32_t bytes;
};

void Address(Outputs& out, uint32_t value, const std::vector<Known>& known) {
  for (uint32_t i = 0; i < known.size(); i++) {
    if (known[i].address != 0 && value - known[i].address < known[i].bytes) {
      out.Word(1 + i);
      out.Word(value - known[i].address);
      return;
    }
  }
  out.Word(0);
  out.Word(value);
}

void Addresses(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
               const std::vector<Known>& known) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    Address(out, guest::LoadU32(base, address + i), known);
  }
}

// A texture's data bytes as Texture::Create sizes it (XGSetTextureHeader's
// answer, with no header written).
uint32_t TextureBytes(uint8_t* base, uint32_t format, uint32_t width, uint32_t height) {
  return graphics::XGSetTextureHeader(base, width, height, 1, 2, format, 0, 0, 0, 0, 0, 0);
}

uint32_t TextureSize(Random& r) {
  switch (r.Below(4)) {
    case 0:
      return 1u << (2 + r.Below(7));  // 4 to 256
    case 1:
      return 4 * (1 + r.Below(64));
    case 2:
      return 1 + r.Below(64);
    default:
      return 1 + r.Below(512);
  }
}

// Texture::Create: the sheet's and the vinyl's formats and others, sizes 1
// to 512 and 1,024; the two words it writes anywhere in the block. The
// block (the two words as allocations), r3, the header (its addresses as
// allocations), the data and its frame's stack words.
void TextureCreates(Context& h, uint32_t block) {
  namespace cm = car_models;
  Results results("Texture::Create (sub_820ECF08)");
  static constexpr uint32_t kFormats[] = {cm::kSheetFormat, cm::kVinylFormat, 0x18280186,
                                          0x1A200052, 0x1A22AAA6};
  for (int c = 0; c < 40; c++) {
    const uint64_t seed = h.random.Next();
    const uint32_t format = kFormats[h.random.Below(std::size(kFormats))];
    const uint32_t width = c == 0 ? 1024 : TextureSize(h.random);
    const uint32_t height = c == 0 ? 1024 : TextureSize(h.random);
    const uint32_t header_out = block + 4 * h.random.Below(32);
    const uint32_t data_out = block + 128 + 4 * h.random.Below(32);
    const uint32_t bytes = TextureBytes(h.base, format, width, height);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, kBlockBytes, r);
      PPCContext t = CallContext(h);
      const uint32_t frame = t.r1.u32 - cm::kTextureCreateFrameBytes;
      RandomBytes(h.base, frame, cm::kTextureCreateFrameBytes, r);
      t.r3.u64 = format;
      t.r4.u64 = width;
      t.r5.u64 = height;
      t.r6.u64 = header_out;
      t.r7.u64 = data_out;
      (v == 0 ? __imp__sub_820ECF08 : hooks::CarModelsTextureCreate)(t, h.base);
      const uint32_t header = guest::LoadU32(h.base, header_out);
      const uint32_t data = guest::LoadU32(h.base, data_out);
      const std::vector<Known> known = {{header, 52}, {data, bytes}};
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      Addresses(out[v], h.base, block, 256, known);
      out[v].Bytes(Read(h.base, block + 256, kBlockBytes - 256));
      Addresses(out[v], h.base, header, 52, known);
      out[v].Bytes(Read(h.base, data, bytes));
      Addresses(out[v], h.base, frame + 84, 28, known);
      memory::FreePages(header);
      memory::FreePhysical(data);
    }
    results.Check(out[0], out[1], fmt::format("case {}: {:08X} {} x {}", c, format, width,
                                              height));
  }
  results.Report();
}

// Texture::Free: a header (the layer's pages or none; a heap block in the
// layer's range would only log VirtualFree's error on both sides) and data
// (physical memory or none) at two words anywhere in the block. The block, r3 (the data's address as an
// allocation) and whether the header's pages are still alive.
void TextureFrees(Context& h, uint32_t block) {
  Results results("Texture::Free (sub_820ECFC8)");
  for (int c = 0; c < 48; c++) {
    const uint64_t seed = h.random.Next();
    const bool has_header = h.random.Below(6) != 0;
    const bool has_data = h.random.Below(4) != 0;
    const uint32_t data_bytes = 1 + h.random.Below(0x10000);
    const uint32_t header_in = block + 4 * h.random.Below(32);
    const uint32_t data_in = block + 128 + 4 * h.random.Below(32);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, kBlockBytes, r);
      const uint32_t header = has_header ? memory::AllocPages(52, 4096) : 0;
      const uint32_t data =
          has_data ? memory::AllocPhysical(data_bytes, 0, 0xFFFFFFFF, 0, 0x404) : 0;
      Put(h.base, header_in, header);
      Put(h.base, data_in, data);
      PPCContext t = CallContext(h);
      t.r3.u64 = header_in;
      t.r4.u64 = data_in;
      (v == 0 ? __imp__sub_820ECFC8 : hooks::CarModelsTextureFree)(t, h.base);
      const std::vector<Known> known = {{header, 52}, {data, data_bytes}};
      Address(out[v], t.r3.u32, known);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      Addresses(out[v], h.base, block, 256, known);
      out[v].Bytes(Read(h.base, block + 256, kBlockBytes - 256));
      const bool alive = header != 0 && memory::IsPages(header);
      out[v].Word(alive);
      if (alive) {
        memory::FreePages(header);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}: header {}, data {}", c, has_header,
                                              has_data));
  }
  results.Report();
}

// Vinyl::Compress's register steps (its adapter is gone: only
// CarModel::OnDecalLoaded's native version calls it).
void NativeVinylCompress(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = hooks::CarModelsVinylCompressAt(ctx, base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r6.u32);
}

// Vinyl::Compress: a sheet (Texture::Create's plain C++, A8R8G8B8, its data
// random bytes) of sizes 1 to 512, and once the game's 1,024 x 1,024; its
// two words anywhere in the block. The recompiled side with
// --native_car_models and --native_texture_compression off. The block (the
// words as allocations), r3, the new texture's header and data (its DXT5
// blocks), whether the sheet's header is still alive, the frame's stack
// words (+20 to +208: the five words XGCompressSurface saves there, its
// stack arguments and the locals, the addresses as allocations), the alpha
// reference (a float) and the premultiplied switch. Texture::Create's frame
// isn't compared here (TextureCreates does): XGCompressSurface's original
// saves its registers over it, which its plain C++ doesn't (its harness
// compares its locals only).
void VinylCompresses(Context& h, uint32_t block) {
  namespace cm = car_models;
  namespace tc = texture_compression;
  Results results("Vinyl::Compress (sub_820ED090)");
  const uint32_t saved_alpha = guest::LoadU32(h.base, tc::kAlphaReference);
  const uint32_t saved_premultiplied = guest::LoadU32(h.base, tc::kPremultiplied);
  for (int c = 0; c < 24; c++) {
    const uint64_t seed = h.random.Next();
    const uint32_t width = c == 0 ? 1024 : TextureSize(h.random);
    const uint32_t height = c == 0 ? 1024 : TextureSize(h.random);
    const uint32_t header_in = block + 4 * h.random.Below(32);
    const uint32_t data_in = block + 128 + 4 * h.random.Below(32);
    const uint32_t sheet_bytes = TextureBytes(h.base, cm::kSheetFormat, width, height);
    const uint32_t vinyl_bytes = TextureBytes(h.base, cm::kVinylFormat, width, height);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, kBlockBytes, r);
      PPCContext t = CallContext(h);
      const uint32_t frame = t.r1.u32 - cm::kVinylCompressFrameBytes;
      RandomBytes(h.base, frame - cm::kTextureCreateFrameBytes,
                  cm::kVinylCompressFrameBytes + cm::kTextureCreateFrameBytes, r);
      guest::StoreU32(h.base, tc::kAlphaReference, uint32_t(r.Next()));
      guest::StoreU32(h.base, tc::kPremultiplied, r.Below(2));
      cm::TextureCreate(h.base, cm::kSheetFormat, width, height, header_in, data_in, t.r1.u32);
      const uint32_t sheet = guest::LoadU32(h.base, header_in);
      const uint32_t sheet_data = guest::LoadU32(h.base, data_in);
      RandomBytes(h.base, sheet_data, sheet_bytes & ~3u, r);
      RandomBytes(h.base, frame - cm::kTextureCreateFrameBytes, cm::kTextureCreateFrameBytes, r);
      t.r3.u64 = width;
      t.r4.u64 = height;
      t.r5.u64 = header_in;
      t.r6.u64 = data_in;
      ITC_FLAG(native_car_models) = v == 1;
      ITC_FLAG(native_texture_compression) = v == 1;
      (v == 0 ? __imp__sub_820ED090 : NativeVinylCompress)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      ITC_FLAG(native_texture_compression) = true;
      const uint32_t header = guest::LoadU32(h.base, header_in);
      const uint32_t data = guest::LoadU32(h.base, data_in);
      // The sheet's data in the other view (Surface::LockRect's flag 0x10).
      const std::vector<Known> known = {{header, 52},
                                        {data, vinyl_bytes},
                                        {sheet, 52},
                                        {sheet_data, sheet_bytes},
                                        {sheet_data - 0x40000000u, sheet_bytes}};
      Address(out[v], t.r3.u32, known);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      Addresses(out[v], h.base, block, 256, known);
      out[v].Bytes(Read(h.base, block + 256, kBlockBytes - 256));
      Addresses(out[v], h.base, header, 52, known);
      out[v].Bytes(Read(h.base, data, vinyl_bytes));
      out[v].Word(memory::IsPages(sheet));
      Addresses(out[v], h.base, frame + 20, 208 - 20, known);
      out[v].Float(guest::LoadU32(h.base, tc::kAlphaReference));
      out[v].Word(guest::LoadU32(h.base, tc::kPremultiplied));
      memory::FreePages(header);
      memory::FreePhysical(data);
    }
    results.Check(out[0], out[1], fmt::format("case {}: {} x {}", c, width, height));
  }
  guest::StoreU32(h.base, tc::kAlphaReference, saved_alpha);
  guest::StoreU32(h.base, tc::kPremultiplied, saved_premultiplied);
  results.Report();
}

// The decals' TGA header's clear: a header anywhere in the block.
void TgaHeaderClears(Context& h, uint32_t block) {
  Results results("TgaHeader::Clear (sub_820EEBC0)");
  for (int c = 0; c < 16; c++) {
    Case(
        h, results, block, h.random.Next(),
        [&](int, PPCContext& t, Random& r) { t.r3.u64 = block + r.Below(0x200); }, nullptr,
        __imp__sub_820EEBC0, hooks::CarModelsTgaHeaderClear, fmt::format("case {}", c));
  }
  results.Report();
}

// TGA::CreateImage: a file (the layer's pages, as the archive's loads give
// them) of an uncompressed 24- or 32-bit image, its header's other bytes and
// its pixels random; sizes 0 to 128 mostly, a 1,024 x 1,024 layer of each
// depth and decal sizes (256 and 512). The header out, the buffer's word
// and the pixels' word apart in the block's first 256 bytes. The
// recompiled side with --native_car_models off. The block (the words as
// allocations), r3, the ARGB pixels, whether they're still pages, whether
// the file is, and the frame's +80 to +160 (the reader; the addresses as
// allocations, with the frame's own).
void TgaCreateImages(Context& h, uint32_t block) {
  namespace cm = car_models;
  Results results("TGA::CreateImage (sub_820ED020)");
  for (int c = 0; c < 40; c++) {
    const uint64_t seed = h.random.Next();
    const uint32_t bits = c % 2 == 0 ? 32 : 24;
    uint32_t width = 1 + h.random.Below(64), height = 1 + h.random.Below(64);
    if (c < 2) {
      width = height = 1024;
    } else if (c < 6) {
      width = c < 4 ? 256 : 512;
      height = h.random.Below(2) ? width : width / 2;
    } else if (c == 6) {
      width = 0;
    } else if (c == 7) {
      height = 0;
    } else if (c % 5 == 0) {
      width = 1 + h.random.Below(128);
      height = 1 + h.random.Below(128);
    }
    const uint32_t size = 18 + width * height * (bits / 8);
    const uint32_t buffer = memory::AllocPages(size, 4096);
    {
      Random r(seed ^ 0x5447);
      RandomBytes(h.base, buffer, (size + 3) & ~3u, r);
    }
    guest::StoreU8(h.base, buffer + 2, 2);
    guest::StoreU8(h.base, buffer + 12, uint8_t(width));
    guest::StoreU8(h.base, buffer + 13, uint8_t(width >> 8));
    guest::StoreU8(h.base, buffer + 14, uint8_t(height));
    guest::StoreU8(h.base, buffer + 15, uint8_t(height >> 8));
    guest::StoreU8(h.base, buffer + 16, uint8_t(bits));
    const uint32_t header_out = block + 128 + h.random.Below(128 - 20);
    const uint32_t buffer_at = block + 4 * h.random.Below(32);
    uint32_t pixels_out = block + 4 * h.random.Below(32);
    while (pixels_out == buffer_at) {
      pixels_out = block + 4 * h.random.Below(32);
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, kBlockBytes, r);
      PPCContext t = CallContext(h);
      const uint32_t frame = t.r1.u32 - cm::kTgaCreateImageFrameBytes;
      RandomBytes(h.base, frame, cm::kTgaCreateImageFrameBytes, r);
      Put(h.base, buffer_at, buffer);
      t.r3.u64 = header_out;
      t.r4.u64 = size;
      t.r5.u64 = buffer_at;
      t.r6.u64 = pixels_out;
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820ED020 : hooks::CarModelsTgaCreateImage)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      const uint32_t pixels = guest::LoadU32(h.base, pixels_out);
      const uint32_t bytes = width * height * 4;
      const std::vector<Known> known = {{pixels, bytes ? bytes : 1}, {buffer, size},
                                        {frame, cm::kTgaCreateImageFrameBytes}};
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      Addresses(out[v], h.base, block, 256, known);
      out[v].Bytes(Read(h.base, block + 256, kBlockBytes - 256));
      if (bytes != 0) {
        out[v].Bytes(Read(h.base, pixels, bytes));
      }
      Addresses(out[v], h.base, frame + 80, 80, known);
      const bool alive = pixels != 0 && memory::IsPages(pixels);
      out[v].Word(alive);
      out[v].Word(memory::IsPages(buffer));
      if (alive) {
        memory::FreePages(pixels);
      }
    }
    memory::FreePages(buffer);
    results.Check(out[0], out[1], fmt::format("case {}: {} bits, {} x {}", c, bits, width,
                                              height));
  }
  results.Report();
}

// The decals' rasterizer (car_models/raster.h). Its inner functions have no
// adapters (only their native callers reach them): their register steps are
// here. Each group's recompiled side runs with --native_car_models off.
void NativeSampleTexel(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      car_models::SampleTexel(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.r1.u32 - 80);
}
void NativeBlendPixel(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  car_models::BlendPixel(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32, ctx.r7.s32,
                         ctx.r8.u32, ctx.r1.u32);
}
void NativeDrawSpan(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  car_models::DrawSpan(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r6.s32, ctx.r7.s32,
                       ctx.r8.u32, ctx.r9.u32, ctx.r10.u32, ctx.r1.u32);
}
void NativeDrawTriangle(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  car_models::DrawTriangle(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                           ctx.r8.u32, ctx.r1.u32);
}
void NativeDecalCorner(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  car_models::DecalCorner(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r8.u32,
                          ctx.f1.f64);
}

// A float for a texel or a position: mostly within `range`; `odd` ones out
// of it. Texels (kind 0) also get NaNs, -infinity and negatives (they clamp
// to the image; +infinity would read past it, as the original would);
// positions (kind 1) only finite values a little out (an infinite or NaN row
// would make the triangle's row loops run 2^31 times, in both); anything
// (kind 2) any value.
float RasterFloat(Random& r, float range, bool odd, int kind = 0) {
  if (odd) {
    switch (kind == 1 ? 3 + r.Below(5) : r.Below(8)) {
      case 0:
        return std::numeric_limits<float>::quiet_NaN();
      case 1:
        return -std::numeric_limits<float>::infinity();
      case 2:
        return kind == 2 ? std::numeric_limits<float>::infinity() : range + 5.5f;
      case 3:
        return -float(r.Below(1u << 10)) / 7.0f;
      case 4:
        return kind == 2 ? 3.0e9f : range * 3;
      default:
        return range * 2 * float(r.Below(1u << 20)) / float(1u << 20) - range / 2;
    }
  }
  return range * float(r.Below(1u << 20)) / float(1u << 20);
}

void PutFloat(uint8_t* base, uint32_t address, float value) {
  guest::StoreFloat(base, address, value);
}

// The small rasterizer cases' layout in the block: the image (16 bytes),
// the region (12), the canvas word, the texel's floats, the vertices, the
// image's pixels and the canvas's.
constexpr uint32_t kImage = 0, kRegion = 16, kCanvas = 32, kUv = 48, kVertices = 64;
constexpr uint32_t kImagePixels = 0x400, kCanvasPixels = 0x1000;

struct RasterSizes {
  uint32_t image_w, image_h, image_pitch;
  uint32_t canvas_w, canvas_h, canvas_pitch;
};

RasterSizes MakeRasterSizes(Random& r) {
  RasterSizes z;
  z.image_w = 1 + r.Below(12);
  z.image_h = 1 + r.Below(12);
  z.image_pitch = 4 * (z.image_w + r.Below(4));
  z.canvas_w = 1 + r.Below(24);
  z.canvas_h = 1 + r.Below(24);
  z.canvas_pitch = 4 * (z.canvas_w + r.Below(8));
  return z;
}

// The structures written over the block's random bytes. Pixels: random,
// with alphas of 0, 255 and others.
void PutRaster(uint8_t* base, uint32_t block, const RasterSizes& z, Random& r) {
  Put(base, block + kImage + 0, block + kImagePixels);
  Put(base, block + kImage + 4, z.image_w);
  Put(base, block + kImage + 8, z.image_h);
  Put(base, block + kImage + 12, z.image_pitch);
  Put(base, block + kRegion + 0, z.canvas_w);
  Put(base, block + kRegion + 4, z.canvas_h);
  Put(base, block + kRegion + 8, z.canvas_pitch);
  Put(base, block + kCanvas, block + kCanvasPixels);
  for (uint32_t i = 0; i < z.image_pitch * z.image_h; i += 4) {
    uint32_t pixel = uint32_t(r.Next());
    switch (r.Below(4)) {
      case 0:
        pixel &= 0x00FFFFFF;
        break;
      case 1:
        pixel |= 0xFF000000;
        break;
      default:
        break;
    }
    Put(base, block + kImagePixels + i, pixel);
  }
}

void TexelCases(Context& h, uint32_t block) {
  Results results("SoftRaster::SampleTexel (sub_820ED2A0)");
  for (int c = 0; c < 400; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, kBlockBytes, r);
      const RasterSizes z = MakeRasterSizes(r);
      PutRaster(h.base, block, z, r);
      PPCContext t = CallContext(h);
      RandomBytes(h.base, t.r1.u32 - 80, 16, r);
      t.r3.u64 = block + kImage;
      t.f1.f64 = double(RasterFloat(r, float(z.image_w), c % 4 == 3));
      t.f2.f64 = double(RasterFloat(r, float(z.image_h), c % 4 == 3));
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820ED2A0 : NativeSampleTexel)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, t.r1.u32 - 80, 16));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void BlendCases(Context& h, uint32_t block) {
  Results results("SoftRaster::BlendPixel (sub_820ED498)");
  for (int c = 0; c < 200; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, kBlockBytes, r);
      const RasterSizes z = MakeRasterSizes(r);
      PutRaster(h.base, block, z, r);
      PPCContext t = CallContext(h);
      const uint32_t frame = t.r1.u32 - car_models::kBlendPixelFrameBytes;
      RandomBytes(h.base, frame - 80, 16, r);
      PutFloat(h.base, block + kUv, RasterFloat(r, float(z.image_w), c % 8 == 7));
      PutFloat(h.base, block + kUv + 4, RasterFloat(r, float(z.image_h), c % 8 == 7));
      t.r3.u64 = block + kImage;
      t.r4.u64 = block + kCanvas;
      t.r5.u64 = block + kRegion;
      t.r6.u64 = r.Below(z.canvas_w);
      t.r7.u64 = r.Below(z.canvas_h);
      t.r8.u64 = block + kUv;
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820ED498 : NativeBlendPixel)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      out[v].Bytes(Read(h.base, block, kBlockBytes));
      out[v].Bytes(Read(h.base, frame - 80, 16));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void SpanCases(Context& h, uint32_t block) {
  namespace cm = car_models;
  Results results("SoftRaster::DrawSpan (sub_820EDE00)");
  for (int c = 0; c < 300; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, kBlockBytes, r);
      const RasterSizes z = MakeRasterSizes(r);
      PutRaster(h.base, block, z, r);
      PPCContext t = CallContext(h);
      const uint32_t frame = t.r1.u32 - cm::kDrawSpanFrameBytes;
      const uint32_t low = frame - cm::kBlendPixelFrameBytes - 80;
      RandomBytes(h.base, low, t.r1.u32 + 96 - low, r);
      const bool odd = c % 8 == 7;
      for (uint32_t end = 0; end < 2; end++) {
        PutFloat(h.base, block + kVertices + 16 * end, RasterFloat(r, float(z.image_w), odd));
        PutFloat(h.base, block + kVertices + 16 * end + 4,
                 RasterFloat(r, float(z.image_h), odd));
      }
      Put(h.base, t.r1.u32 + 84, r.Below(2) ? 0x80000000u : 0);
      t.r3.u64 = block + kCanvas;
      t.r4.u64 = block + kRegion;
      t.r5.u64 = uint32_t(int32_t(r.Below(z.canvas_w + 16)) - 8);
      t.r6.u64 = uint32_t(int32_t(r.Below(z.canvas_w + 16)) - 8);
      t.r7.u64 = uint32_t(int32_t(r.Below(z.canvas_h + 4)) - 2);
      t.r8.u64 = block + kVertices;
      t.r9.u64 = block + kVertices + 16;
      t.r10.u64 = block + kImage;
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820EDE00 : NativeDrawSpan)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      out[v].Bytes(Read(h.base, block, kBlockBytes));
      out[v].Bytes(Read(h.base, frame + 80, 64));
      out[v].Bytes(Read(h.base, low, 16));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void TriangleCases(Context& h, uint32_t block) {
  namespace cm = car_models;
  Results results("SoftRaster::DrawTriangle (sub_820EE0C8)");
  for (int c = 0; c < 300; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, kBlockBytes, r);
      const RasterSizes z = MakeRasterSizes(r);
      PutRaster(h.base, block, z, r);
      PPCContext t = CallContext(h);
      const uint32_t frame = t.r1.u32 - cm::kDrawTriangleFrameBytes;
      const uint32_t span = frame - cm::kDrawSpanFrameBytes;
      const uint32_t low = span - cm::kBlendPixelFrameBytes - 80;
      RandomBytes(h.base, low, t.r1.u32 - low, r);
      const bool odd = c % 16 == 15;
      for (uint32_t i = 0; i < 3; i++) {
        const uint32_t p = block + kVertices + 16 * i, uv = block + kVertices + 48 + 16 * i;
        PutFloat(h.base, p, RasterFloat(r, float(z.canvas_w + 8), odd, 1) - 4);
        PutFloat(h.base, p + 4, RasterFloat(r, float(z.canvas_h + 8), odd, 1) - 4);
        PutFloat(h.base, uv, RasterFloat(r, float(z.image_w), false));
        PutFloat(h.base, uv + 4, RasterFloat(r, float(z.image_h), false));
      }
      t.r3.u64 = block + kCanvas;
      t.r4.u64 = block + kRegion;
      t.r5.u64 = block + kVertices;
      t.r6.u64 = block + kVertices + 48;
      t.r7.u64 = block + kImage;
      t.r8.u64 = r.Below(2) ? uint32_t(r.Next()) : 0;
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820EE0C8 : NativeDrawTriangle)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      out[v].Bytes(Read(h.base, block, kBlockBytes));
      out[v].Bytes(Read(h.base, frame + 84, 240 - 84));
      out[v].Bytes(Read(h.base, span + 80, 64));
      out[v].Bytes(Read(h.base, low, 16));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void CornerCases(Context& h, uint32_t block) {
  Results results("Vinyl::DecalCorner (sub_820ED1E8)");
  for (int c = 0; c < 200; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, block, 256, r);
      PPCContext t = CallContext(h);
      const bool odd = c % 8 == 7;
      for (uint32_t i = 0; i < 8; i++) {
        PutFloat(h.base, block + 4 * i, RasterFloat(r, 512.0f, odd, 2));
      }
      const bool same = r.Below(2) != 0;
      t.r3.u64 = same ? block : block + 64;
      t.r4.u64 = block;
      t.r5.u64 = block + 8;
      t.r6.u64 = block + 16;
      t.r8.u64 = block + 24;
      t.f1.f64 = double(odd ? RasterFloat(r, 7.0f, true, 2) : RasterFloat(r, 12.0f, false) - 6);
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820ED1E8 : NativeDecalCorner)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      out[v].Floats(h.base, block, 2);
      out[v].Floats(h.base, block + 64, 2);
      out[v].Bytes(Read(h.base, block + 8, 56));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// Vinyl::DrawDecal: decals as the game's (an image of 16 to 256 pixels a
// side, now and then 512 and the 1,024 x 1,024 layer over the whole sheet,
// region 9, as the game's, random ARGB with clear and opaque pixels) into a
// 1,024 x 1,024 sheet of random pixels: a region 0 to 9, mirrored or not, a scale from 0.25 to 2.25, an offset
// around the region, a rotation (now and then far out). The recompiled side
// with --native_car_models off. The whole sheet, r3, the caller's frame
// (+32 to +144: the registers spilled), the frame's +80 to +272 and the
// frames under it (the triangle's +84 to +240, the span's +80 to +144, the
// texel's words).
void DecalCases(Context& h) {
  namespace cm = car_models;
  Results results("Vinyl::DrawDecal (sub_820EE638)");
  const uint32_t sheet_width = guest::LoadU32(h.base, cm::kVinylSheetWidth);
  const uint32_t sheet_bytes = sheet_width * sheet_width * 4;
  const uint32_t sheet = memory::AllocPages(sheet_bytes, 4096);
  const uint32_t image = memory::AllocPages(1024 * 1024 * 4, 4096);
  for (int c = 0; c < 32; c++) {
    const uint64_t seed = h.random.Next();
    uint32_t w = 16 + h.random.Below(241), hh = 16 + h.random.Below(241);
    if (c == 0) {
      w = hh = 1024;
    } else if (c < 4) {
      w = 512;
      hh = c == 1 ? 512 : 256;
    }
    const uint32_t region = c == 0 ? 9 : h.random.Below(10);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      for (uint32_t i = 0; i < sheet_bytes; i += 4) {
        Put(h.base, sheet + i, uint32_t(r.Next()));
      }
      for (uint32_t i = 0; i < w * hh * 4; i += 4) {
        uint32_t pixel = uint32_t(r.Next());
        pixel = r.Below(3) == 0 ? pixel & 0x00FFFFFF : r.Below(2) ? pixel | 0xFF000000 : pixel;
        Put(h.base, image + i, pixel);
      }
      PPCContext t = CallContext(h);
      const uint32_t s = t.r1.u32;
      const uint32_t frame = s - cm::kDrawDecalFrameBytes;
      const uint32_t triangle = frame - cm::kDrawTriangleFrameBytes;
      const uint32_t span = triangle - cm::kDrawSpanFrameBytes;
      const uint32_t low = span - cm::kBlendPixelFrameBytes - 80;
      RandomBytes(h.base, low, s + 160 - low, r);
      // The caller's frame: the scale, the pixels and the sheet.
      const float scale = c == 0 ? 1.0f : 0.25f + float(r.Below(1000)) / 500.0f;
      PutFloat(h.base, s + 96, scale);
      PutFloat(h.base, s + 100, r.Below(4) == 0 ? scale * 0.5f : scale);
      Put(h.base, s + 116, image);
      Put(h.base, s + 124, 0);
      Put(h.base, s + 132, sheet);
      // The TGA header's 20 bytes: the width and height at +14 and +16
      // (big-endian u16s, as the reader leaves them).
      uint8_t header[24] = {};
      for (uint32_t i = 0; i < 20; i++) {
        header[i] = uint8_t(r.Next());
      }
      header[14] = uint8_t(w >> 8);
      header[15] = uint8_t(w);
      header[16] = uint8_t(hh >> 8);
      header[17] = uint8_t(hh);
      auto word64 = [&](uint32_t at) {
        uint64_t value = 0;
        for (uint32_t i = 0; i < 8; i++) {
          value = value << 8 | header[at + i];
        }
        return value;
      };
      t.r3.u64 = region;
      t.r4.u64 = r.Below(2) ? 1 + r.Below(4) : 0;
      t.r5.u64 = word64(0);
      t.r6.u64 = word64(8);
      t.r7.u64 = word64(16) & 0xFFFFFFFF00000000ull;
      const float offset_x = c == 0 ? 0.0f : float(int32_t(r.Below(600)) - 300);
      const float offset_y = c == 0 ? 0.0f : float(int32_t(r.Below(600)) - 300);
      t.r8.u64 = uint64_t(std::bit_cast<uint32_t>(offset_x)) << 32 |
                 std::bit_cast<uint32_t>(offset_y);
      t.r9.u64 = r.Next();
      t.f1.f64 = c == 0 ? 0.0 : double(float(int32_t(r.Below(2000)) - 1000) / 100.0f);
      if (c % 8 == 7) {
        t.f1.f64 = double(float(int32_t(r.Below(2000000)) - 1000000));
      }
      ITC_FLAG(native_car_models) = v == 1;
      (v == 0 ? __imp__sub_820EE638 : hooks::CarModelsDrawDecal)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, sheet, sheet_bytes));
      out[v].Bytes(Read(h.base, s + 32, 112));
      out[v].Bytes(Read(h.base, frame + 80, 272 - 80));
      out[v].Bytes(Read(h.base, triangle + 84, 240 - 84));
      out[v].Bytes(Read(h.base, span + 80, 64));
      out[v].Bytes(Read(h.base, low, 16));
    }
    results.Check(out[0], out[1], fmt::format("case {}: region {}, {} x {}", c, region, w, hh));
  }
  memory::FreePages(sheet);
  memory::FreePages(image);
  results.Report();
}

// A TGA file as the archive gives it (the layer's pages): uncompressed 24-
// or 32-bit, `w` x `h`, its other header bytes and pixels random (alphas 0,
// 255 and others).
uint32_t MakeTga(uint8_t* base, uint32_t w, uint32_t hh, uint32_t bits, Random& r,
                 uint32_t* size) {
  *size = 18 + w * hh * (bits / 8);
  const uint32_t buffer = memory::AllocPages(*size, 4096);
  RandomBytes(base, buffer, (*size + 3) & ~3u, r);
  guest::StoreU8(base, buffer + 2, 2);
  guest::StoreU8(base, buffer + 12, uint8_t(w));
  guest::StoreU8(base, buffer + 13, uint8_t(w >> 8));
  guest::StoreU8(base, buffer + 14, uint8_t(hh));
  guest::StoreU8(base, buffer + 15, uint8_t(hh >> 8));
  guest::StoreU8(base, buffer + 16, uint8_t(bits));
  if (bits == 32) {
    for (uint32_t i = 0; i < w * hh; i++) {
      const uint32_t at = buffer + 18 + 4 * i + 3;
      const uint32_t pick = r.Below(4);
      guest::StoreU8(base, at, pick == 0 ? 0 : pick == 1 ? 255 : guest::LoadU8(base, at));
    }
  }
  return buffer;
}

// CarModel::OnDecalLoaded: a car model's slots and records as the game
// leaves them (car_models/decals.h): layers and stickers arrived (state 3)
// or not (0, 1, 2), each arrived one with its TGA (16 to 96 pixels a side,
// now and then the 1,024 x 1,024 layer), the request's slot asked (1) or
// loading (2), records with regions 0 to 8, mirrors, offsets, scales and
// rotations; a second half slot loading now and then (no compression then),
// else the sheet compressed (a few cases: the recompiled side's compression
// is slow). The sheet (Texture::Create's plain C++) of random pixels. The
// recompiled side with --native_car_models and --native_texture_compression
// off. The car model's 8 KB (its words holding a side's allocations as
// which allocation), r3, the sheet's pixels or, compressed, the new
// texture's header and data, whether each file and the sheet are still
// pages, and DrawDecals' frame (+96 to +272; the pixels' words, freed, as
// whether set). The TGA headers' +3 and +9 in the frame are in a group of
// their own, expected to differ: the reader never writes them, so they're
// whatever the stack held there, and the original callees before them
// (Vinyl::DrawDecal's, the previous decal's) saved their registers over
// that stack, which their plain C++ doesn't (the stack leftovers of
// docs/loops/expected-differences.md). Nothing reads them: DrawDecal reads
// only the header's width and height.
void DecalLoadCases(Context& h) {
  namespace cm = car_models;
  namespace tc = texture_compression;
  Results results("CarModel::OnDecalLoaded (sub_820D5750)");
  constexpr uint32_t kCarBytes = 8192;
  const uint32_t allocation = memory::Alloc(kCarBytes + 16, 0);
  const uint32_t car = (allocation + 15) & ~15u;
  const uint32_t sheet_width = guest::LoadU32(h.base, cm::kVinylSheetWidth);
  const uint32_t sheet_bytes = TextureBytes(h.base, cm::kSheetFormat, sheet_width, sheet_width);
  const uint32_t vinyl_bytes = TextureBytes(h.base, cm::kVinylFormat, sheet_width, sheet_width);
  const uint32_t saved_alpha = guest::LoadU32(h.base, tc::kAlphaReference);
  const uint32_t saved_premultiplied = guest::LoadU32(h.base, tc::kPremultiplied);
  Results expected("CarModel::OnDecalLoaded (sub_820D5750), the TGA headers' unwritten bytes "
                   "(expected to differ)");
  // The frame's words holding a header's +3 and +9 (the copy at +192 and
  // the second at +224), and those bytes' masks.
  static constexpr std::pair<uint32_t, uint32_t> kHeaderLeftovers[] = {
      {192, 0x000000FFu}, {200, 0x00FF0000u}, {224, 0x000000FFu}, {232, 0x00FF0000u}};
  int compressed_cases = 0;
  for (int c = 0; c < 16; c++) {
    const uint64_t seed = h.random.Next();
    const bool compress = c % 6 == 1;
    Outputs out[2], leftovers[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomBytes(h.base, car, kCarBytes, r);
      PPCContext t = CallContext(h);
      const uint32_t s = t.r1.u32;
      const uint32_t frame = s - cm::kOnDecalLoadedFrameBytes - cm::kDrawDecalsFrameBytes;
      RandomBytes(h.base, frame - 1024, s + 64 - (frame - 1024), r);
      std::vector<Known> known;
      std::vector<uint32_t> files;
      // The sheet.
      cm::TextureCreate(h.base, cm::kSheetFormat, sheet_width, sheet_width,
                        car + cm::kSheetHeader, car + cm::kSheetData, frame);
      const uint32_t sheet = guest::LoadU32(h.base, car + cm::kSheetHeader);
      const uint32_t sheet_data = guest::LoadU32(h.base, car + cm::kSheetData);
      for (uint32_t i = 0; i < sheet_bytes; i += 4) {
        Put(h.base, sheet_data + i, uint32_t(r.Next()));
      }
      known.push_back({sheet, 52});
      known.push_back({sheet_data, sheet_bytes});
      known.push_back({sheet_data - 0x40000000u, sheet_bytes});
      Put(h.base, car + 216, r.Below(9));
      // The records: offsets, a rotation, a scale; the stickers' region and
      // mirror.
      auto record = [&](uint32_t at, bool sticker) {
        PutFloat(h.base, at + (sticker ? 16 : 0), float(int32_t(r.Below(400)) - 200));
        PutFloat(h.base, at + (sticker ? 20 : 4), float(int32_t(r.Below(400)) - 200));
        PutFloat(h.base, at + (sticker ? 32 : 16), float(int32_t(r.Below(700)) - 350) / 100.0f);
        const float scale = 0.5f + float(r.Below(100)) / 100.0f;
        PutFloat(h.base, at + (sticker ? 48 : 32), scale);
        PutFloat(h.base, at + (sticker ? 52 : 36), scale);
        if (sticker) {
          Put(h.base, at + 4, r.Below(3) == 0 ? 1 : 0);
          Put(h.base, at + 8, r.Below(9));
        }
      };
      for (uint32_t i = 0; i < 6; i++) {
        record(car + 224 + 64 * i, false);
      }
      static constexpr uint32_t kGroups[8] = {1360, 976, 1744, 2128, 3280, 2896, 2512, 592};
      for (uint32_t g = 0; g < 8; g++) {
        for (uint32_t k = 0; k < 6; k++) {
          record(car + kGroups[g] + 64 * k, true);
        }
      }
      // The slots.
      const uint32_t request = uint32_t(r.Next()) | 1;
      const uint32_t chosen = r.Below(4) == 0 ? r.Below(6) : 6 + r.Below(48);
      for (uint32_t n = 0; n < 54; n++) {
        const uint32_t slot =
            n < 6 ? car + cm::kLayerSlots + 48 * n : car + cm::kStickerSlots + 48 * (n - 6);
        Put(h.base, slot + 4, n == chosen ? request : uint32_t(r.Next()) & ~1u);
        Put(h.base, slot + 44, 0);
        const bool arrived = n == chosen || r.Below(6) == 0;
        // Compress cases leave no slot loading (state 2), so the call
        // reaches ClearCorner and the compress.
        Put(h.base, slot + 20,
            n == chosen ? 1 + r.Below(2) : arrived ? 3 : r.Below(compress ? 2 : 3));
        Put(h.base, slot + 12, 0);
        if (!arrived) {
          continue;
        }
        const bool layer = n < 6 && r.Below(8) == 0;
        uint32_t size = 0;
        const uint32_t w = layer ? 1024 : 16 + r.Below(81), hh = layer ? 1024 : 16 + r.Below(81);
        const uint32_t file = MakeTga(h.base, w, hh, r.Below(2) ? 32 : 24, r, &size);
        Put(h.base, slot + 8, size);
        Put(h.base, slot + 16, file);
        known.push_back({file, size});
        files.push_back(file);
      }
      if (!compress) {
        const uint32_t n = r.Below(54);
        const uint32_t slot =
            n < 6 ? car + cm::kLayerSlots + 48 * n : car + cm::kStickerSlots + 48 * (n - 6);
        Put(h.base, slot + 44, 2);
      }
      Put(h.base, car + 6692, uint32_t(r.Next()));
      Put(h.base, car + 6696, uint32_t(r.Next()));
      guest::StoreU32(h.base, tc::kAlphaReference, uint32_t(r.Next()));
      guest::StoreU32(h.base, tc::kPremultiplied, r.Below(2));
      t.r3.u64 = request;
      t.r6.u64 = car;
      ITC_FLAG(native_car_models) = v == 1;
      ITC_FLAG(native_texture_compression) = v == 1;
      (v == 0 ? __imp__sub_820D5750 : hooks::CarModelsOnDecalLoaded)(t, h.base);
      ITC_FLAG(native_car_models) = true;
      ITC_FLAG(native_texture_compression) = true;
      const uint32_t header = guest::LoadU32(h.base, car + cm::kSheetHeader);
      const uint32_t data = guest::LoadU32(h.base, car + cm::kSheetData);
      const bool compressed = header != sheet;
      compressed_cases += v == 1 && compressed;
      if (compressed) {
        known.push_back({header, 52});
        known.push_back({data, vinyl_bytes});
      }
      Address(out[v], t.r3.u32, known);
      out[v].Word(compressed);
      Addresses(out[v], h.base, car, kCarBytes, known);
      if (compressed) {
        Addresses(out[v], h.base, header, 52, known);
        out[v].Bytes(Read(h.base, data, vinyl_bytes));
      } else {
        out[v].Bytes(Read(h.base, sheet_data, sheet_bytes));
      }
      for (uint32_t file : files) {
        // The new sheet's header may take a freed file's pages.
        const bool alive = memory::IsPages(file) && !(compressed && file == header);
        out[v].Word(alive);
        if (alive) {
          memory::FreePages(file);
        }
      }
      out[v].Word(memory::IsPages(sheet));
      for (uint32_t at = 96; at < 272; at += 4) {
        uint32_t word = guest::LoadU32(h.base, frame + at);
        // The TGA headers' +3 and +9 (the reader never writes them: what
        // the stack held under TGA::CreateImage's frame, where the original
        // callees before it saved their registers), in their own group.
        for (const auto& [header, unwritten] : kHeaderLeftovers) {
          if (at == header) {
            leftovers[v].Word(word & unwritten);
            word &= ~unwritten;
          }
        }
        if (at == 116 || at == 144) {
          out[v].Word(word != 0);
        } else {
          Address(out[v], word, known);
        }
      }
      if (compressed) {
        memory::FreePages(header);
        memory::FreePhysical(data);
      } else {
        memory::FreePages(sheet);
        memory::FreePhysical(sheet_data);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}{}", c, compress ? " (compressed)" : ""));
    expected.Check(leftovers[0], leftovers[1], fmt::format("case {}", c));
  }
  guest::StoreU32(h.base, tc::kAlphaReference, saved_alpha);
  guest::StoreU32(h.base, tc::kPremultiplied, saved_premultiplied);
  memory::Free(allocation);
  ITC_LOG_INFO("harness: car_models: OnDecalLoaded compressed the sheet in {} of 16 cases",
               compressed_cases);
  results.Report();
  expected.Report();
}

void Rasterizer(Context& h, uint32_t block) {
  TexelCases(h, block);
  BlendCases(h, block);
  SpanCases(h, block);
  TriangleCases(h, block);
  CornerCases(h, block);
  if (hooks::NativeHeapActive()) {
    DecalCases(h);
  }
}

void Vinyls(Context& h, uint32_t block) {
  TgaHeaderClears(h, block);
  Rasterizer(h, block);
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_WARN("harness: car_models: --native_heap is off: the vinyl's cases skipped");
    return;
  }
  TgaCreateImages(h, block);
  TextureCreates(h, block);
  TextureFrees(h, block);
  VinylCompresses(h, block);
  DecalLoadCases(h);
}

}  // namespace

void CarModels(Context& h) {
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Slots(h, block);
  DeferReleases(h, block);
  Releases(h, block);
  Vinyls(h, block);
  memory::Free(allocation);
}

}  // namespace harness
