// The vinyl sheet's decals as they arrive (see decals.h), call for call as
// the recompiled functions.

#include "car_models/decals.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_models/load_slots.h"
#include "car_models/raster.h"
#include "car_models/tga.h"
#include "car_models/vinyl.h"
#include "guest.h"
#include "memory/heap.h"

#include <cstdlib>
#include <cstring>
#include <vector>

namespace car_models {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return uint64_t(Load(base, address)) << 32 | Load(base, address + 4);
}
void Copy(uint8_t* base, float value, uint32_t address) {
  guest::StoreFloat(base, address, value);
}

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Car models: {} refused after its checks", what);
    std::abort();
  }
}

// The stickers' eight groups of six records (64 bytes each), by group.
constexpr uint32_t kStickerGroups[8] = {1360, 976, 1744, 2128, 3280, 2896, 2512, 592};

bool Arrived(int32_t state) {
  return state != 1 && state != 0 && state != 2;
}

// Vinyl::DrawDecal with the header DrawDecals' frame holds at `header`.
void Draw(uint8_t* base, uint32_t region, uint32_t mirrored, uint32_t header, uint64_t record0,
          uint64_t record1, double angle, uint32_t frame) {
  const DecalArguments arguments{region,
                                 mirrored,
                                 {Load64(base, header), Load64(base, header + 8),
                                  uint64_t(Load(base, header + 16)) << 32},
                                 {record0, record1},
                                 angle};
  Require(DrawDecal(base, arguments, frame).has_value(), "Vinyl::DrawDecal");
}

// The four words Vinyl::DrawDecal reads from its caller's frame, and the
// 16 bytes at +96 (memcpy).
void PutDrawWords(uint8_t* base, uint32_t car, uint32_t f, uint32_t from) {
  Store(base, f + 124, 0);
  Store(base, f + 140, Load(base, car + 6696));
  Store(base, f + 132, Load(base, car + kSheetData));
  Store(base, f + 116, Load(base, f + 144));
  Require(c_runtime::Memcpy(base, f + 96, from, 16), "Vinyl::DrawDecals' memcpy");
}

// One slot: its TGA decoded and its file released; false when it hasn't
// arrived.
bool Decode(uint8_t* base, uint32_t slot, uint32_t f) {
  Store(base, f + 144, 0);
  TgaHeaderClear(base, f + 192);
  TgaHeaderClear(base, f + 256);
  if (!Arrived(int32_t(Load(base, slot + 20)))) {
    return false;
  }
  Require(TgaCreateImage(base, f + 192, Load(base, slot + 8), slot + 16, f + 144, f).has_value(),
          "TGA::CreateImage");
  Require(LoadSlotReleaseBuffer(base, slot).has_value(), "LoadSlot::ReleaseBuffer");
  return true;
}

}  // namespace

void ClearCorner(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820D3C80 Vinyl::ClearCorner");
  const uint32_t sizes = kVinylSheetWidth;
  const uint32_t sheet = Load(base, car + kSheetData);
  const uint32_t rows = Load(base, sizes + 12);
  uint32_t row = ((Load(base, sizes + 4) - rows) * Load(base, sizes + 0) << 2) + sheet;
  for (int32_t i = 0; i < int32_t(Load(base, sizes + 12)); i++) {
    guest::ForEachHostPart(base, row, Load(base, sizes + 8) << 2,
                           [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
    row += Load(base, sizes + 0) << 2;
  }
}

uint32_t DrawDecals(uint8_t* base, uint32_t car, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D4078 Vinyl::DrawDecals");
  const uint32_t f = stack - kDrawDecalsFrameBytes;
  const float zero = guest::LoadFloat(base, kDecalsZero);  // f30
  const float one = guest::LoadFloat(base, kDecalsOne);    // f31
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t slot = car + kLayerSlots + 48 * i;
    const uint32_t layer = car + 240 + 64 * i;  // r31
    if (!Decode(base, slot, f)) {
      continue;
    }
    if (guest::LoadU16(base, f + 206) == 1024 && guest::LoadU16(base, f + 208) == 1024) {
      // The whole sheet: scale and offset (1, 1), (1, 1)... from the frame.
      for (uint32_t at : {160u, 164u, 168u, 172u}) {
        Copy(base, one, f + at);
      }
      for (uint32_t at : {176u, 180u, 184u}) {
        Copy(base, zero, f + at);
      }
      Copy(base, one, f + 188);
      PutDrawWords(base, car, f, f + 160);
      Draw(base, 9, 0, f + 192, Load64(base, f + 176), Load64(base, f + 184), double(zero), f);
    } else if (i < 3) {
      PutDrawWords(base, car, f, layer + 16);
      Draw(base, Load(base, car + 216), 0, f + 192, Load64(base, layer - 16),
           Load64(base, layer - 8), double(guest::LoadFloat(base, layer)), f);
    } else {
      Require(c_runtime::Memcpy(base, f + 224, f + 192, 20), "Vinyl::DrawDecals' memcpy");
      PutDrawWords(base, car, f, layer + 16);
      Draw(base, 1, 0, f + 192, Load64(base, layer - 16), Load64(base, layer - 8),
           double(guest::LoadFloat(base, layer)), f);
      PutDrawWords(base, car, f, layer + 16);
      Draw(base, 2, 0, f + 224, Load64(base, layer - 16), Load64(base, layer - 8),
           double(guest::LoadFloat(base, layer)), f);
      // The copy's empty destructor (sub_82106540).
    }
    memory::FreePages(Load(base, f + 144));  // VirtualFree(pixels, 0, MEM_RELEASE)
  }
  for (uint32_t j = 0; j < 48; j++) {
    const uint32_t slot = car + kStickerSlots + 48 * j;
    if (!Decode(base, slot, f)) {
      continue;
    }
    const uint32_t group = j / 6;
    const uint32_t record = car + kStickerGroups[group] + ((j - group * 6) << 6);
    PutDrawWords(base, car, f, record + 48);
    Draw(base, Load(base, record + 8), Load(base, record + 4), f + 192, Load64(base, record + 16),
         Load64(base, record + 24), double(guest::LoadFloat(base, record + 32)), f);
    memory::FreePages(Load(base, f + 144));
  }
  return 1;
}

namespace {

// The first slot of the layers', else the stickers', whose request is
// `request`; 0 for none.
uint32_t MatchingSlot(uint8_t* base, uint32_t request, uint32_t car) {
  for (uint32_t i = 0; i < 6; i++) {
    if (Load(base, car + kLayerSlots + 48 * i + 4) == request) {
      return car + kLayerSlots + 48 * i;
    }
  }
  for (uint32_t j = 0; j < 48; j++) {
    if (Load(base, car + kStickerSlots + 48 * j + 4) == request) {
      return car + kStickerSlots + 48 * j;
    }
  }
  return 0;
}

// Whether a slot is still loading, the matched one (`reset`) reset by then.
bool AnyLoading(uint8_t* base, uint32_t car, uint32_t reset) {
  auto loading = [&](uint32_t slot) {
    return slot != reset && int32_t(Load(base, slot + 20)) == 2;
  };
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t record = car + kLayerSlots + 48 * i;
    if (loading(record) || loading(record + 24)) {
      return true;
    }
  }
  for (uint32_t j = 0; j < 48; j++) {
    const uint32_t record = car + kStickerSlots + 48 * j;
    if (loading(record) || loading(record + 24)) {
      return true;
    }
  }
  return false;
}

}  // namespace

bool DecalsNative(uint8_t* base, uint32_t request, uint32_t car) {
  const uint32_t matched = MatchingSlot(base, request, car);
  if (matched != 0) {
    std::vector<uint32_t> freed;
    auto drawable = [&](uint32_t slot) {
      if (slot != matched && !Arrived(int32_t(Load(base, slot + 20)))) {
        return true;
      }
      uint32_t pages = 0;
      if (Load(base, slot + 16) == 0 || !TgaDecodable(base, Load(base, slot + 16)) ||
          !LoadSlotReleasable(base, slot, &pages)) {
        return false;
      }
      if (pages != 0) {
        for (uint32_t other : freed) {
          if (other == pages) {
            return false;
          }
        }
        freed.push_back(pages);
      }
      return true;
    };
    for (uint32_t i = 0; i < 6; i++) {
      if (!drawable(car + kLayerSlots + 48 * i)) {
        return false;
      }
    }
    for (uint32_t j = 0; j < 48; j++) {
      if (!drawable(car + kStickerSlots + 48 * j)) {
        return false;
      }
    }
  }
  return AnyLoading(base, car, matched) || TextureFreeable(base, car + kSheetHeader);
}

uint32_t OnDecalLoaded(uint8_t* base, uint32_t request, uint32_t car, uint32_t stack,
                       const CompressCall& compress) {
  ITC_NATIVE_RUNNING("sub_820D5750 CarModel::OnDecalLoaded");
  const uint32_t f = stack - kOnDecalLoadedFrameBytes;
  uint32_t r3 = request;
  if (const uint32_t slot = MatchingSlot(base, request, car); slot != 0) {
    Store(base, slot + 20, 3);
    DrawDecals(base, car, f);
    LoadSlotReset(base, slot);
    r3 = slot;
  }
  if (AnyLoading(base, car, 0)) {
    return r3;
  }
  ClearCorner(base, car);
  r3 = compress(Load(base, kVinylSheetWidth + 0), Load(base, kVinylSheetWidth + 4),
                car + kSheetHeader, car + kSheetData);
  Store(base, car + 4088, 3);
  return r3;
}

}  // namespace car_models
