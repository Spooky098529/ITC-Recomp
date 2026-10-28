// The decals' TGA reader (see tga.h), call for call as the recompiled
// functions.

#include "car_models/tga.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/heap.h"
#include "std_string/std_string.h"

#include <cstdlib>

namespace car_models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Car models: {} refused after its checks", what);
    std::abort();
  }
}

// VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE) and VirtualFree(address,
// 0, MEM_RELEASE), as their adapters do them (src/hooks/heap.cpp).
uint32_t VirtualAlloc(uint32_t size) {
  return memory::AllocPages(size, 4096);
}
void VirtualFree(uint32_t address) {
  memory::FreePages(address);
}

}  // namespace

void TgaHeaderClear(uint8_t* base, uint32_t header) {
  ITC_NATIVE_RUNNING("sub_820EEBC0 TgaHeader::Clear");
  guest::StoreU8(base, header + 0, 0);
  guest::StoreU8(base, header + 1, 0);
  guest::StoreU8(base, header + 2, 0);
  for (uint32_t offset : {4u, 6u}) {
    guest::StoreU16(base, header + offset, 0);
  }
  guest::StoreU8(base, header + 8, 0);
  for (uint32_t offset : {10u, 12u, 14u, 16u}) {
    guest::StoreU16(base, header + offset, 0);
  }
  guest::StoreU8(base, header + 18, 0);
  guest::StoreU8(base, header + 19, 0);
}

void TgaImageClear(uint8_t* base, uint32_t image) {
  ITC_NATIVE_RUNNING("sub_820EED38 TgaImage::Clear");
  for (uint32_t offset = 4; offset <= 24; offset += 4) {
    Store(base, image + offset, 0);
  }
}

void TgaImageFree(uint8_t* base, uint32_t image) {
  ITC_NATIVE_RUNNING("sub_820EED58 TgaImage::Free");
  if (const uint32_t pixels = Load(base, image + 12); pixels != 0) {
    VirtualFree(pixels);
    Store(base, image + 12, 0);
  }
}

void TgaReaderInit(uint8_t* base, uint32_t reader) {
  ITC_NATIVE_RUNNING("sub_820EEB58 TgaReader::TgaReader");
  Require(std_string::Tidy(base, reader, false, 0), "the reader's _Tidy");
  TgaHeaderClear(base, reader + kTgaReaderHeader);
  TgaImageClear(base, reader + kTgaReaderImage);
  Require(std_string::AssignText(base, reader, kTgaReaderText, 0), "the reader's assign");
  Store(base, reader + 32, 0);
}

void TgaReaderFree(uint8_t* base, uint32_t reader) {
  ITC_NATIVE_RUNNING("sub_820EEAE8 TgaReader::~TgaReader");
  if (const uint32_t buffer = Load(base, reader + 32); buffer != 0) {
    VirtualFree(buffer);
    Store(base, reader + 32, 0);
  }
  TgaImageFree(base, reader + kTgaReaderImage);
  // The header's destructor (sub_82106540) does nothing.
  Require(std_string::Tidy(base, reader, true, 0), "the reader's _Tidy");
}

uint32_t TgaParseHeader(uint8_t* base, uint32_t header, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_820EEBF8 TGA::ParseHeader");
  auto byte = [&](uint32_t i) { return guest::LoadU8(base, buffer + i); };
  guest::StoreU8(base, header + 0, byte(0));
  guest::StoreU8(base, header + 1, byte(1));
  guest::StoreU8(base, header + 2, byte(2));
  // The u16s: the file's little-endian pairs at 3, 5, 8, 10, 12, 14.
  static constexpr struct {
    uint32_t from, to;
  } kWords[] = {{3, 4}, {5, 6}, {8, 10}, {10, 12}, {12, 14}, {14, 16}};
  guest::StoreU8(base, header + 8, byte(7));
  for (const auto& w : kWords) {
    guest::StoreU16(base, header + w.to, uint16_t(byte(w.from) | byte(w.from + 1) << 8));
  }
  guest::StoreU8(base, header + 18, byte(16));
  guest::StoreU8(base, header + 19, byte(17));
  return 0;
}

uint32_t TgaDecode(uint8_t* base, uint32_t image, uint32_t buffer, uint32_t header) {
  ITC_NATIVE_RUNNING("sub_820EF168 TGA::Decode");
  Store(base, image + 8, header);
  Store(base, image + 4, buffer);
  const uint32_t height = guest::LoadU16(base, header + 16);
  Store(base, image + 20, guest::LoadU16(base, header + 14));
  Store(base, image + 24, height);
  const uint8_t bits = guest::LoadU8(base, header + 18);
  Require(bits == 24 || bits == 32, "TGA::Decode's depth");
  const uint32_t pixels = height * Load(base, image + 20);
  const uint32_t size = bits == 32 ? pixels << 2 : pixels * 3;
  Store(base, image + 16, size);
  Store(base, image + 12, VirtualAlloc(size));
  Require(guest::LoadU8(base, Load(base, image + 8) + 2) == 2, "TGA::Decode's type");
  Require(c_runtime::Memcpy(base, Load(base, image + 12), buffer + 18, Load(base, image + 16)),
          "TGA::Decode's memcpy");
  return 0;
}

uint32_t TgaReaderLoad(uint8_t* base, uint32_t reader, uint32_t size, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_820EEA78 TgaReader::Load");
  if (Load(base, reader + 32) != 0) {
    return 5;
  }
  Store(base, reader + 28, size);
  Store(base, reader + 32, buffer);
  TgaParseHeader(base, reader + kTgaReaderHeader, buffer);
  return TgaDecode(base, reader + kTgaReaderImage, Load(base, reader + 32),
                   reader + kTgaReaderHeader);
}

void TgaToArgb(uint8_t* base, uint32_t image, uint32_t pixels_out) {
  ITC_NATIVE_RUNNING("sub_820EEF08 TGA::ToARGB");
  const bool rgb = guest::LoadU8(base, Load(base, image + 8) + 18) == 24;
  const uint32_t size =
      rgb ? (Load(base, image + 20) * Load(base, image + 24)) << 2 : Load(base, image + 16);
  Store(base, pixels_out, VirtualAlloc(size));
  const uint32_t width = Load(base, image + 20);
  const uint32_t height = Load(base, image + 24);
  const uint32_t to = Load(base, pixels_out);
  const uint32_t from = Load(base, image + 12);
  for (uint32_t y = 0; y < height; y++) {
    const uint32_t row = to + ((width * y) << 2);
    const uint32_t source_row = (height - y - 1) * width;
    for (uint32_t x = 0; x < width; x++) {
      uint32_t argb;
      if (rgb) {
        const uint32_t p = from + source_row * 3 + 3 * x;
        argb = 0xFF000000u | uint32_t(guest::LoadU8(base, p + 2)) << 16 |
               uint32_t(guest::LoadU8(base, p + 1)) << 8 | guest::LoadU8(base, p);
      } else {
        argb = __builtin_bswap32(Load(base, from + (source_row << 2) + 4 * x));
      }
      Store(base, row + 4 * x, argb);
    }
  }
}

bool TgaDecodable(uint8_t* base, uint32_t buffer) {
  const uint8_t bits = guest::LoadU8(base, buffer + 16);
  return guest::LoadU8(base, buffer + 2) == 2 && (bits == 24 || bits == 32);
}

std::optional<uint32_t> TgaCreateImage(uint8_t* base, uint32_t header_out, uint32_t size,
                                       uint32_t buffer_at, uint32_t pixels_out,
                                       uint32_t stack) {
  if (!TgaDecodable(base, Load(base, buffer_at))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ED020 TGA::CreateImage");
  const uint32_t reader = stack - kTgaCreateImageFrameBytes + kTgaReaderInFrame;
  TgaReaderInit(base, reader);
  TgaReaderLoad(base, reader, size, Load(base, buffer_at));
  TgaToArgb(base, reader + kTgaReaderImage, pixels_out);
  Require(c_runtime::Memcpy(base, header_out, reader + kTgaReaderHeader, 20),
          "TGA::CreateImage's memcpy");
  Store(base, reader + 32, 0);
  TgaReaderFree(base, reader);
  return 1;
}

}  // namespace car_models
