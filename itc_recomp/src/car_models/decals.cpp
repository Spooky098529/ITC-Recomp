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
#include "schema/car_models.h"

#include <cstdlib>
#include <cstring>
#include <vector>

namespace car_models {

namespace {

using schema::CarModel;
using schema::LoadSlot;
using G = schema::CarModelsGlobals;

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
  const CarModel c(base, car);
  Store(base, f + 124, 0);
  Store(base, f + 140, c.unknown_6696());
  Store(base, f + 132, c.vinyl_sheet_data());
  Store(base, f + 116, Load(base, f + 144));
  Require(c_runtime::Memcpy(base, f + 96, from, 16), "Vinyl::DrawDecals' memcpy");
}

// One slot: its TGA decoded and its file released; false when it hasn't
// arrived.
bool Decode(uint8_t* base, uint32_t slot, uint32_t f) {
  Store(base, f + 144, 0);
  TgaHeaderClear(base, f + 192);
  TgaHeaderClear(base, f + 256);
  const LoadSlot s(base, slot);
  if (!Arrived(int32_t(s.state()))) {
    return false;
  }
  Require(TgaCreateImage(base, f + 192, s.size(), s.buffer_address(), f + 144, f).has_value(),
          "TGA::CreateImage");
  Require(LoadSlotReleaseBuffer(base, slot).has_value(), "LoadSlot::ReleaseBuffer");
  return true;
}

}  // namespace

void ClearCorner(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820D3C80 Vinyl::ClearCorner");
  const uint32_t sheet = CarModel(base, car).vinyl_sheet_data();
  const uint32_t rows = G::vinyl_corner_rows(base);
  uint32_t row = ((G::vinyl_sheet_height(base) - rows) * G::vinyl_sheet_width(base) << 2) + sheet;
  for (int32_t i = 0; i < int32_t(G::vinyl_corner_rows(base)); i++) {
    guest::ForEachHostPart(base, row, G::vinyl_corner_width(base) << 2,
                           [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
    row += G::vinyl_sheet_width(base) << 2;
  }
}

uint32_t DrawDecals(uint8_t* base, uint32_t car, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D4078 Vinyl::DrawDecals");
  const CarModel c(base, car);
  const uint32_t f = stack - kDrawDecalsFrameBytes;
  const float zero = G::float_zero(base);  // f30
  const float one = G::float_one(base);    // f31
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t slot = c.layer_slots_address() + 48 * i;
    const schema::CarModelDecalRecord layer(base, c.vinyl_layers_address() + 64 * i);
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
      PutDrawWords(base, car, f, layer.scale_address());
      Draw(base, c.layer0_region(), 0, f + 192, layer.offset_xy(), layer.offset_zw(),
           double(layer.rotation()), f);
    } else {
      Require(c_runtime::Memcpy(base, f + 224, f + 192, 20), "Vinyl::DrawDecals' memcpy");
      PutDrawWords(base, car, f, layer.scale_address());
      Draw(base, 1, 0, f + 192, layer.offset_xy(), layer.offset_zw(), double(layer.rotation()),
           f);
      PutDrawWords(base, car, f, layer.scale_address());
      Draw(base, 2, 0, f + 224, layer.offset_xy(), layer.offset_zw(), double(layer.rotation()),
           f);
      // The copy's empty destructor (sub_82106540).
    }
    memory::FreePages(Load(base, f + 144));  // VirtualFree(pixels, 0, MEM_RELEASE)
  }
  for (uint32_t j = 0; j < 48; j++) {
    const uint32_t slot = c.sticker_slots_address() + 48 * j;
    if (!Decode(base, slot, f)) {
      continue;
    }
    const uint32_t group = j / 6;
    const schema::CarModelDecalRecord record(base,
                                             car + kStickerGroups[group] + ((j - group * 6) << 6));
    PutDrawWords(base, car, f, record.scale_address());
    Draw(base, record.region(), record.mirrored(), f + 192, record.offset_xy(),
         record.offset_zw(), double(record.rotation()), f);
    memory::FreePages(Load(base, f + 144));
  }
  return 1;
}

namespace {

// The first slot of the layers', else the stickers', whose request is
// `request`; 0 for none.
uint32_t MatchingSlot(uint8_t* base, uint32_t request, uint32_t car) {
  const CarModel c(base, car);
  for (uint32_t i = 0; i < 6; i++) {
    if (LoadSlot(base, c.layer_slots_address() + 48 * i).request() == request) {
      return c.layer_slots_address() + 48 * i;
    }
  }
  for (uint32_t j = 0; j < 48; j++) {
    if (LoadSlot(base, c.sticker_slots_address() + 48 * j).request() == request) {
      return c.sticker_slots_address() + 48 * j;
    }
  }
  return 0;
}

// Whether a slot is still loading, the matched one (`reset`) reset by then.
bool AnyLoading(uint8_t* base, uint32_t car, uint32_t reset) {
  const CarModel c(base, car);
  auto loading = [&](uint32_t slot) {
    return slot != reset && int32_t(LoadSlot(base, slot).state()) == 2;
  };
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t record = c.layer_slots_address() + 48 * i;
    if (loading(record) || loading(record + 24)) {
      return true;
    }
  }
  for (uint32_t j = 0; j < 48; j++) {
    const uint32_t record = c.sticker_slots_address() + 48 * j;
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
    const CarModel c(base, car);
    auto drawable = [&](uint32_t slot) {
      const LoadSlot s(base, slot);
      if (slot != matched && !Arrived(int32_t(s.state()))) {
        return true;
      }
      uint32_t pages = 0;
      if (s.buffer() == 0 || !TgaDecodable(base, s.buffer()) ||
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
      if (!drawable(c.layer_slots_address() + 48 * i)) {
        return false;
      }
    }
    for (uint32_t j = 0; j < 48; j++) {
      if (!drawable(c.sticker_slots_address() + 48 * j)) {
        return false;
      }
    }
  }
  return AnyLoading(base, car, matched) ||
         TextureFreeable(base, CarModel(base, car).vinyl_sheet_address());
}

uint32_t OnDecalLoaded(uint8_t* base, uint32_t request, uint32_t car, uint32_t stack,
                       const CompressCall& compress) {
  ITC_NATIVE_RUNNING("sub_820D5750 CarModel::OnDecalLoaded");
  const CarModel c(base, car);
  const uint32_t f = stack - kOnDecalLoadedFrameBytes;
  uint32_t r3 = request;
  if (const uint32_t slot = MatchingSlot(base, request, car); slot != 0) {
    LoadSlot(base, slot).set_state(3);
    DrawDecals(base, car, f);
    LoadSlotReset(base, slot);
    r3 = slot;
  }
  if (AnyLoading(base, car, 0)) {
    return r3;
  }
  ClearCorner(base, car);
  r3 = compress(G::vinyl_sheet_width(base), G::vinyl_sheet_height(base), c.vinyl_sheet_address(),
                c.vinyl_sheet_data_address());
  c.set_decals_state(3);
  return r3;
}

}  // namespace car_models
