// The decals' TGA reader as plain C++ (docs/functions/car-models.md:
// TGA::CreateImage and the reader under it), over the allocation layer
// (memory/heap.h: VirtualAlloc's and VirtualFree's pages), std::string's
// plain C++ and memcpy's. Each works on guest memory where the original
// does (the reader lives in TGA::CreateImage's frame). They allocate and
// free, so they run natively only while --native_heap routes the game's
// heap to the layer (the adapters' condition). Adapters:
// src/hooks/car_models.cpp.
//
// The reader (76 bytes): +0 a std::string (28 bytes), +28 the file's size,
// +32 the file's buffer (0 when not loading), +36 the header (20 bytes),
// +56 the image (24 bytes: +4 the buffer, +8 the header's address, +12 the
// decoded pixels, +16 their size, +20 the width, +24 the height; +0 never
// written).
// The header (20 bytes, from the file's 18): +0 the id's length, +1 the
// color map's type, +2 the image's type, +4 and +6 the color map's first
// entry and length, +8 its depth, +10 and +12 the origin, +14 the width,
// +16 the height (the u16s from little-endian), +18 the bits per pixel,
// +19 the descriptor; +3 and +9 never written by the reader.

#pragma once

#include <cstdint>
#include <optional>

namespace car_models {

// TGA::CreateImage's stack frame, and where its reader, header and image
// sit in it.
constexpr uint32_t kTgaCreateImageFrameBytes = 208;
constexpr uint32_t kTgaReaderInFrame = 80;
constexpr uint32_t kTgaReaderHeader = 36;
constexpr uint32_t kTgaReaderImage = 56;
// The text the reader's string is assigned (with a count of 0).
constexpr uint32_t kTgaReaderText = 0x820021F0;

// sub_820EEBC0: the 20-byte header cleared (but for +3 and +9).
void TgaHeaderClear(uint8_t* base, uint32_t header);
// sub_820EED38: the image's +4 to +24 cleared.
void TgaImageClear(uint8_t* base, uint32_t image);
// sub_820EED58: the image's pixels (+12), when set, freed (VirtualFree,
// MEM_RELEASE) and +12 = 0.
void TgaImageFree(uint8_t* base, uint32_t image);
// sub_820EEB58: the string emptied (_Tidy, then assign of 0 characters),
// the header and image cleared, +32 = 0.
void TgaReaderInit(uint8_t* base, uint32_t reader);
// sub_820EEAE8: the buffer (+32), when set, freed (VirtualFree) and +32 =
// 0; the image freed; the string destroyed (_Tidy(true, 0)).
void TgaReaderFree(uint8_t* base, uint32_t reader);
// sub_820EEBF8 TGA::ParseHeader: the file's 18 header bytes into the
// 20-byte header, the u16s swapped. Returns 0.
uint32_t TgaParseHeader(uint8_t* base, uint32_t header, uint32_t buffer);
// sub_820EF168 TGA::Decode: the image's buffer, header, width and height
// set; for 24 or 32 bits the pixels' size and the pixels (VirtualAlloc)
// copied from buffer + 18 (type 2 only: TgaDecodable). Returns 0.
uint32_t TgaDecode(uint8_t* base, uint32_t image, uint32_t buffer, uint32_t header);
// sub_820EEA78: 5 when the reader is busy (+32 set); else +28 the size,
// +32 the buffer, the header parsed and the image decoded (its answer).
uint32_t TgaReaderLoad(uint8_t* base, uint32_t reader, uint32_t size, uint32_t buffer);
// sub_820EEF08 TGA::ToARGB: w x h ARGB words (VirtualAlloc) stored at
// `pixels_out`, the rows flipped; 24-bit pixels get alpha 255.
void TgaToArgb(uint8_t* base, uint32_t image, uint32_t pixels_out);

// Whether the file at `buffer` takes the paths ported here: an
// uncompressed true-color image (type 2) of 24 or 32 bits. The others (the
// RLE type 10, never seen in play, and the errors) run the original.
bool TgaDecodable(uint8_t* base, uint32_t buffer);

// sub_820ED020 TGA::CreateImage: the reader in its frame (`stack` - 208 +
// 80) set up, loaded from the buffer whose address is at `buffer_at`, the
// pixels converted (TgaToArgb into `pixels_out`), the 20-byte header copied
// to `header_out`, the buffer word cleared (so it isn't freed) and the
// reader freed. Returns 1; nullopt, with nothing done, where the file isn't
// TgaDecodable.
std::optional<uint32_t> TgaCreateImage(uint8_t* base, uint32_t header_out, uint32_t size,
                                       uint32_t buffer_at, uint32_t pixels_out,
                                       uint32_t stack);

}  // namespace car_models
