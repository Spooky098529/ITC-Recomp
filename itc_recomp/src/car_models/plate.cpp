// The car model's composed textures (see plate.h), call for call as the
// recompiled functions.

#include "car_models/plate.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "car_models/load_slots.h"
#include "car_models/tga.h"
#include "car_models/vinyl.h"
#include "graphics/resources.h"
#include "guest.h"
#include "native/buffers.h"

#include <cstdlib>

namespace car_models {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Half(uint8_t* base, uint32_t address) { return guest::LoadU16(base, address); }
// ld and std: a doubleword, big-endian.
uint64_t LoadDoubleword(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void StoreDoubleword(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

// memcpy as its override makes it (the callers checked that it's native).
void Copy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size, uint32_t caller) {
  if (!native::Buffers_Memcpy(base, destination, source, size, caller)) {
    ITC_LOG_ERROR("Car models: memcpy of {:#x} bytes from {:#x} to {:#x} refused", size, source,
                  destination);
    std::abort();
  }
}

// The slots' rectangles (16 bytes each: x, y, width, height) and the table
// sub_820ED6D0 looks W1 up in (8 bytes an entry: a bound, then an offset).
constexpr uint32_t kSlotTable = 0x82602AF0;
constexpr uint32_t kNumberTable = 0x8200664C;

int32_t Divide(int32_t a, int32_t b) { return a / b; }

}  // namespace

uint32_t PlateImage(uint8_t* base, uint32_t set, uint32_t kind, uint32_t index,
                    uint32_t header_out, uint32_t pixels_out) {
  ITC_NATIVE_RUNNING("sub_820C4DD8 PlateImage");
  static constexpr uint32_t kFirst[] = {0, 4, 7, 11, 15};
  const uint32_t j = (kind < 5 ? kFirst[kind] : 0) + index;
  Copy(base, header_out, j * 20 + set + 3732, 20, 0x820C4E50);
  Store(base, pixels_out, Load(base, ((j + 914) << 2) + set));
  return header_out;
}

void CopyRect(uint8_t* base, uint32_t dst, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7,
              uint32_t src, uint64_t r9, uint64_t r10, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ED630 CopyRect");
  StoreDoubleword(base, stack + 32, r4);
  StoreDoubleword(base, stack + 48, r6);
  StoreDoubleword(base, stack + 80, r9);
  StoreDoubleword(base, stack + 40, r5);
  StoreDoubleword(base, stack + 56, r7);
  StoreDoubleword(base, stack + 88, r10);
  const int32_t src_pitch = int32_t(Load(base, stack + 96));
  const int32_t dst_pitch = int32_t(Load(base, stack + 48));
  const uint32_t dst_index =
      uint32_t(dst_pitch * int64_t(int32_t(Load(base, stack + 36)))) + Load(base, stack + 32);
  const uint32_t src_index =
      uint32_t(src_pitch * int64_t(int32_t(Load(base, stack + 84)))) + Load(base, stack + 80);
  int32_t rows = int32_t(Load(base, stack + 44));
  uint32_t to = (dst_index << 2) + dst;
  uint32_t from = (src_index << 2) + src;
  if (rows <= 0) {
    return;
  }
  const uint32_t bytes = Load(base, stack + 40) << 2;
  const uint32_t dst_step = uint32_t(dst_pitch) << 2;
  const uint32_t src_step = uint32_t(src_pitch) << 2;
  do {
    Copy(base, to, from, bytes, 0x820ED6B8);
    to += dst_step;
    from += src_step;
  } while (--rows != 0);
}

std::optional<uint32_t> MakePlate(uint8_t* base, uint32_t header_a, uint32_t data_a,
                                  uint32_t header_b, uint32_t data_b, const uint64_t regs[4],
                                  uint32_t stack, const ReleaseCall& release) {
  if (!native::Buffers_NativeMemcpy()) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ED6D0 CarModel::MakePlate");
  const uint32_t f = stack - kPlateFrameBytes;
  // r7 to r10 into the caller's frame: with its 16 bytes at + 80, the words.
  for (uint32_t i = 0; i < 4; i++) {
    StoreDoubleword(base, stack + 48 + 8 * i, regs[i]);
  }
  const auto word = [&](uint32_t k) { return Load(base, stack + 48 + 4 * k); };
  const uint32_t set = Load(base, kSceneSet);
  TgaHeaderClear(base, f + 336);
  TgaHeaderClear(base, f + 128);
  TgaHeaderClear(base, f + 224);
  TgaHeaderClear(base, f + 160);
  TgaHeaderClear(base, f + 192);
  // The five images (their headers' width at +14 and height at +16).
  PlateImage(base, set, 0, 0, f + 336, f + 148);
  PlateImage(base, set, 2, 0, f + 224, f + 120);
  PlateImage(base, set, 4, 0, f + 160, f + 112);
  PlateImage(base, set, 3, 0, f + 192, f + 116);
  PlateImage(base, set, 1, 0, f + 128, f + 152);
  // The slots' rectangles: the table's, their sizes from the images.
  for (uint32_t i = 0; i < 12; i++) {
    const uint32_t slot = f + 592 + 16 * i;
    const uint64_t a = LoadDoubleword(base, kSlotTable + 16 * i);
    const uint64_t b = LoadDoubleword(base, kSlotTable + 16 * i + 8);
    StoreDoubleword(base, slot, a);
    StoreDoubleword(base, slot + 8, b);
    if (i == 0) {
      Store(base, f + 600, uint32_t(Divide(int32_t(Half(base, f + 350)), 6)));
      Store(base, f + 604, uint32_t(Divide(int32_t(Half(base, f + 352)), 6)));
    } else if (i == 1) {
      Store(base, f + 616, uint32_t(Divide(int32_t(Half(base, f + 206)), 12)));
      Store(base, f + 620, uint32_t(Divide(int32_t(Half(base, f + 208)), 10)));
    } else if (i < 5) {
      const uint32_t width = Half(base, f + 174);
      const uint32_t height = Half(base, f + 176);
      Store(base, slot + 8, uint32_t(Divide(int32_t(width), 10)));
      Store(base, slot + 12, height);
    } else {
      const uint32_t height = Half(base, f + 240);
      Store(base, slot + 8, 39);
      Store(base, slot + 12, height);
    }
  }
  const uint32_t width = Half(base, f + 142);
  const uint32_t height = Half(base, f + 144);
  TextureCreate(base, kSheetFormat, width, height, header_a, data_a, f);
  TextureCreate(base, kSheetFormat, width, height, header_b, data_b, f);
  const uint32_t surface = graphics::GetSurfaceLevel(base, Load(base, header_a), 0);
  graphics::LockSurfaceRect(base, surface, f + 360, 0, 0);
  const uint32_t pitch = Load(base, f + 360);
  graphics::UnlockSurface(base, surface);
  release(surface);
  // Image 1 into both.
  {
    uint32_t to_a = Load(base, data_a);
    uint32_t to_b = Load(base, data_b);
    uint32_t from = Load(base, f + 152);
    if (int32_t(height) > 0) {
      const uint32_t step = pitch & ~3u;
      const uint32_t row = width << 2;
      for (uint32_t y = height; y != 0; y--) {
        Copy(base, to_a, from, row, 0x820ED910);
        Copy(base, to_b, from, row, 0x820ED920);
        to_a += step;
        to_b += step;
        from += row;
      }
    }
  }
  const uint32_t number = word(1);
  const uint32_t pitch_pixels = pitch >> 2;
  const uint64_t slot1_at = LoadDoubleword(base, f + 608);
  const uint64_t slot1_size = LoadDoubleword(base, f + 616);
  const uint32_t cell_height = Load(base, f + 620);
  const uint32_t cell_width = Load(base, f + 616);
  for (uint32_t i = 0; i < 12; i++) {
    if (i == 0) {
      // Image 0 into B only, from row 5 x the slot's height.
      Store(base, f + 296, 30);
      Store(base, f + 300, 33);
      Store(base, f + 304, 0);
      Store(base, f + 312, 30);
      Store(base, f + 288, Load(base, f + 592));
      Store(base, f + 316, 33);
      Store(base, f + 400, pitch_pixels);
      Store(base, f + 292, Load(base, f + 596));
      Store(base, f + 308, Load(base, f + 604) * 5);
      Store(base, f + 384, Half(base, f + 350));
      Store(base, f + 388, Half(base, f + 352));
      Store(base, f + 404, Half(base, f + 144));
      Copy(base, f + 96, f + 384, 16, 0x820EDD9C);
      CopyRect(base, Load(base, data_b), LoadDoubleword(base, f + 288),
               LoadDoubleword(base, f + 296), LoadDoubleword(base, f + 400),
               LoadDoubleword(base, f + 408), Load(base, f + 148), LoadDoubleword(base, f + 304),
               LoadDoubleword(base, f + 312), f);
    } else if (i == 1) {
      // Image 3's cell for W1: the table's first bound at or above it
      // gives the cell (W1, or the entry before's offset + W1; W1 + 15 when
      // every bound is below it), 12 cells a row.
      int32_t cell = 0;
      for (uint32_t k = 0; k < 6; k++) {
        if (!(int32_t(Load(base, kNumberTable + 8 * k)) < int32_t(number))) {
          cell = k == 0 ? int32_t(number)
                        : int32_t(Load(base, kNumberTable + 8 * (k - 1) + 4) + number);
          break;
        }
        if (k == 5 && cell == 0) {
          cell = int32_t(number + 15);
        }
      }
      const int32_t row = cell / 12;
      Store(base, f + 328, cell_width);
      Store(base, f + 332, cell_height);
      Store(base, f + 448, pitch_pixels);
      const uint32_t column = uint32_t(cell) - uint32_t(row) * 12;
      Store(base, f + 320, uint32_t(int32_t(column) * int64_t(int32_t(cell_width))));
      Store(base, f + 324, uint32_t(row * int64_t(int32_t(cell_height))));
      Store(base, f + 512, Half(base, f + 206));
      Store(base, f + 516, Half(base, f + 208));
      Store(base, f + 452, Half(base, f + 144));
      Copy(base, f + 96, f + 512, 16, 0x820EDCB8);
      const uint64_t r9 = LoadDoubleword(base, f + 320);
      const uint64_t r10 = LoadDoubleword(base, f + 328);
      CopyRect(base, Load(base, data_a), slot1_at, slot1_size, LoadDoubleword(base, f + 448),
               LoadDoubleword(base, f + 456), Load(base, f + 116), r9, r10, f);
      Store(base, f + 480, pitch_pixels);
      Store(base, f + 576, Half(base, f + 206));
      Store(base, f + 580, Half(base, f + 208));
      Store(base, f + 484, Half(base, f + 144));
      Copy(base, f + 96, f + 576, 16, 0x820EDD14);
      CopyRect(base, Load(base, data_b), slot1_at, slot1_size, LoadDoubleword(base, f + 480),
               LoadDoubleword(base, f + 488), Load(base, f + 116), r9, r10, f);
    } else {
      // Image 4's cells for W2 to W4 (38 none), image 2's for W5 to W11
      // (37 none; 10 and up + 2): cell n at x = n x the slot's width.
      const bool letters = i >= 5;
      int32_t n = int32_t(word(i));
      if (n == (letters ? 37 : 38)) {
        continue;
      }
      if (letters && n >= 10) {
        n += 2;
      }
      const uint32_t slot = f + 592 + 16 * i;
      const uint32_t at = letters ? 272 : 256;  // the source's (x, y) and size
      Store(base, f + at + 4, 0);
      const uint32_t own_a = letters ? f + 432 : f + 560;
      Store(base, own_a, pitch_pixels);
      const uint32_t pair_a = letters ? f + 528 : f + 368;
      const uint32_t width_at = letters ? f + 238 : f + 174;
      const uint32_t height_at = letters ? f + 240 : f + 176;
      Store(base, pair_a, Half(base, width_at));
      const uint32_t cell_w = Load(base, slot + 8);
      const uint32_t cell_h = Load(base, slot + 12);
      Store(base, f + at + 8, cell_w);
      Store(base, f + at + 12, cell_h);
      Store(base, pair_a + 4, Half(base, height_at));
      Store(base, f + at, uint32_t(int32_t(cell_w) * int64_t(n)));
      Store(base, own_a + 4, Half(base, f + 144));
      Copy(base, f + 96, pair_a, 16, letters ? 0x820EDA60 : 0x820EDB7C);
      const uint64_t r9 = LoadDoubleword(base, f + at);
      const uint64_t r10 = LoadDoubleword(base, f + at + 8);
      const uint32_t pixels_at = letters ? f + 120 : f + 112;
      CopyRect(base, Load(base, data_a), LoadDoubleword(base, slot),
               LoadDoubleword(base, slot + 8), LoadDoubleword(base, own_a),
               LoadDoubleword(base, own_a + 8), Load(base, pixels_at), r9, r10, f);
      const uint32_t own_b = letters ? f + 496 : f + 416;
      const uint32_t pair_b = letters ? f + 464 : f + 544;
      Store(base, own_b, pitch_pixels);
      Store(base, pair_b, Half(base, width_at));
      Store(base, pair_b + 4, Half(base, height_at));
      Store(base, own_b + 4, Half(base, f + 144));
      Copy(base, f + 96, pair_b, 16, letters ? 0x820EDABC : 0x820EDBD8);
      CopyRect(base, Load(base, data_b), LoadDoubleword(base, slot),
               LoadDoubleword(base, slot + 8), LoadDoubleword(base, own_b),
               LoadDoubleword(base, own_b + 8), Load(base, pixels_at), r9, r10, f);
    }
  }
  // The five headers' destructors (empty).
  for (int k = 0; k < 5; k++) {
    c_runtime::EmptyDestructor();
  }
  return f + 336;
}

}  // namespace car_models
