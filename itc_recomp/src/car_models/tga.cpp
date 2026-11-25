// The decals' TGA reader (see tga.h), call for call as the recompiled
// functions.

#include "car_models/tga.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/heap.h"
#include "schema/car_models.h"
#include "std_string/std_string.h"

#include <cstdlib>

namespace car_models {

namespace {

using schema::TgaHeader;
using schema::TgaImage;
using schema::TgaReader;

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
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

void TgaHeaderClear(uint32_t header) {
  ITC_NATIVE_RUNNING("sub_820EEBC0 TgaHeader::Clear");
  const TgaHeader h(header);
  h.set_id_length(0);
  h.set_color_map_type(0);
  h.set_image_type(0);
  h.set_color_map_first(0);
  h.set_color_map_length(0);
  h.set_color_map_depth(0);
  h.set_x_origin(0);
  h.set_y_origin(0);
  h.set_width(0);
  h.set_height(0);
  h.set_bits_per_pixel(0);
  h.set_descriptor(0);
}

void TgaImageClear(uint32_t image) {
  ITC_NATIVE_RUNNING("sub_820EED38 TgaImage::Clear");
  const TgaImage i(image);
  i.set_file(0);
  i.set_header(0u);
  i.set_pixels(0);
  i.set_size(0);
  i.set_width(0);
  i.set_height(0);
}

void TgaImageFree(uint32_t image) {
  ITC_NATIVE_RUNNING("sub_820EED58 TgaImage::Free");
  const TgaImage i(image);
  if (const uint32_t pixels = i.pixels(); pixels != 0) {
    VirtualFree(pixels);
    i.set_pixels(0);
  }
}

void TgaReaderInit(uint32_t reader) {
  ITC_NATIVE_RUNNING("sub_820EEB58 TgaReader::TgaReader");
  const TgaReader r(reader);
  Require(std_string::Tidy(r.text_address(), false, 0), "the reader's _Tidy");
  TgaHeaderClear(r.header_address());
  TgaImageClear(r.image_address());
  Require(std_string::AssignText(r.text_address(), kTgaReaderText, 0),
          "the reader's assign");
  r.set_file(0);
}

void TgaReaderFree(uint32_t reader) {
  ITC_NATIVE_RUNNING("sub_820EEAE8 TgaReader::~TgaReader");
  const TgaReader r(reader);
  if (const uint32_t buffer = r.file<schema::View>().address; buffer != 0) {
    VirtualFree(buffer);
    r.set_file(0);
  }
  TgaImageFree(r.image_address());
  // The header's destructor (sub_82106540) does nothing.
  Require(std_string::Tidy(r.text_address(), true, 0), "the reader's _Tidy");
}

uint32_t TgaParseHeader(uint32_t header, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_820EEBF8 TGA::ParseHeader");
  const schema::TgaFile file(buffer);
  const TgaHeader h(header);
  h.set_id_length(file.id_length());
  h.set_color_map_type(file.color_map_type());
  h.set_image_type(file.image_type());
  // The u16s from the file's little-endian pairs.
  const auto word = [](uint8_t lo, uint8_t hi) { return uint16_t(lo | hi << 8); };
  h.set_color_map_depth(file.color_map_depth());
  h.set_color_map_first(word(file.color_map_first_lo(), file.color_map_first_hi()));
  h.set_color_map_length(word(file.color_map_length_lo(), file.color_map_length_hi()));
  h.set_x_origin(word(file.x_origin_lo(), file.x_origin_hi()));
  h.set_y_origin(word(file.y_origin_lo(), file.y_origin_hi()));
  h.set_width(word(file.width_lo(), file.width_hi()));
  h.set_height(word(file.height_lo(), file.height_hi()));
  h.set_bits_per_pixel(file.bits_per_pixel());
  h.set_descriptor(file.descriptor());
  return 0;
}

uint32_t TgaDecode(uint32_t image, uint32_t buffer, uint32_t header) {
  ITC_NATIVE_RUNNING("sub_820EF168 TGA::Decode");
  const TgaImage i(image);
  const TgaHeader h(header);
  i.set_header(header);
  i.set_file(buffer);
  const uint32_t height = h.height();
  i.set_width(h.width());
  i.set_height(height);
  const uint8_t bits = h.bits_per_pixel();
  Require(bits == 24 || bits == 32, "TGA::Decode's depth");
  const uint32_t pixels = height * i.width();
  const uint32_t size = bits == 32 ? pixels << 2 : pixels * 3;
  i.set_size(size);
  i.set_pixels(VirtualAlloc(size));
  Require(i.header().image_type() == 2, "TGA::Decode's type");
  Require(c_runtime::Memcpy(i.pixels(), buffer + 18, i.size()), "TGA::Decode's memcpy");
  return 0;
}

uint32_t TgaReaderLoad(uint32_t reader, uint32_t size, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_820EEA78 TgaReader::Load");
  const TgaReader r(reader);
  if (r.file<schema::View>().address != 0) {
    return 5;
  }
  r.set_file_size(size);
  r.set_file(buffer);
  TgaParseHeader(r.header_address(), buffer);
  return TgaDecode(r.image_address(), r.file<schema::View>().address, r.header_address());
}

void TgaToArgb(uint32_t image, uint32_t pixels_out) {
  ITC_NATIVE_RUNNING("sub_820EEF08 TGA::ToARGB");
  const TgaImage i(image);
  const schema::CarModelsAddressWord out(pixels_out);
  const bool rgb = i.header().bits_per_pixel() == 24;
  const uint32_t size = rgb ? (i.width() * i.height()) << 2 : i.size();
  out.set_value(VirtualAlloc(size));
  const uint32_t width = i.width();
  const uint32_t height = i.height();
  const uint32_t to = out.value();
  const uint32_t from = i.pixels();
  for (uint32_t y = 0; y < height; y++) {
    const uint32_t row = to + ((width * y) << 2);
    const uint32_t source_row = (height - y - 1) * width;
    for (uint32_t x = 0; x < width; x++) {
      uint32_t argb;
      if (rgb) {
        const uint32_t p = from + source_row * 3 + 3 * x;
        argb = 0xFF000000u | uint32_t(guest::LoadU8(p + 2)) << 16 |
               uint32_t(guest::LoadU8(p + 1)) << 8 | guest::LoadU8(p);
      } else {
        argb = __builtin_bswap32(Load(from + (source_row << 2) + 4 * x));
      }
      Store(row + 4 * x, argb);
    }
  }
}

bool TgaDecodable(uint32_t buffer) {
  const schema::TgaFile file(buffer);
  const uint8_t bits = file.bits_per_pixel();
  return file.image_type() == 2 && (bits == 24 || bits == 32);
}

std::optional<uint32_t> TgaCreateImage(uint32_t header_out, uint32_t size,
                                       uint32_t buffer_at, uint32_t pixels_out,
                                       uint32_t stack) {
  const schema::CarModelsAddressWord file(buffer_at);
  if (!TgaDecodable(file.value())) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ED020 TGA::CreateImage");
  const TgaReader r(stack - kTgaCreateImageFrameBytes + kTgaReaderInFrame);
  TgaReaderInit(r.address);
  TgaReaderLoad(r.address, size, file.value());
  TgaToArgb(r.image_address(), pixels_out);
  Require(c_runtime::Memcpy(header_out, r.header_address(), 20),
          "TGA::CreateImage's memcpy");
  r.set_file(0);
  TgaReaderFree(r.address);
  return 1;
}

}  // namespace car_models
