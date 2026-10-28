// The vinyl sheet's decals as they arrive, as plain C++
// (docs/functions/car-models.md: CarModel::OnDecalLoaded, Vinyl::DrawDecals
// and Vinyl::ClearCorner): each arrived slot's TGA decoded (tga.h) and drawn
// into the sheet (raster.h), and once nothing is loading, the sheet's corner
// cleared and the sheet compressed (vinyl.h). Over the allocation layer, so
// natively only while --native_heap routes the game's heap to it (the
// adapter's condition). Adapter: src/hooks/car_models.cpp.
//
// The car model's slots: six layers (48 bytes each from +4092) and 48
// stickers (from +4380), each two 24-byte load slots (load_slots.h; the
// first's +20 the state: 0 empty, 1 asked, 2 loading, 3 arrived; the
// second's +44); the layers' records (64 bytes from +224), the stickers'
// eight groups of six (64 bytes each, the groups' offsets in
// kStickerGroups), the sheet's header and data (+6684, +6688; +6692, +6696
// beside them).

#pragma once

#include <cstdint>
#include <functional>

namespace car_models {

constexpr uint32_t kDrawDecalsFrameBytes = 368;
constexpr uint32_t kOnDecalLoadedFrameBytes = 144;
constexpr uint32_t kLayerSlots = 4092, kStickerSlots = 4380;
constexpr uint32_t kSheetHeader = 6684, kSheetData = 6688;
// The floats DrawDecals reads (0.0 and 1.0).
constexpr uint32_t kDecalsZero = 0x82000A34;
constexpr uint32_t kDecalsOne = 0x82000A30;

// sub_820D3C80 Vinyl::ClearCorner: the sheet's bottom left corner
// ([0x82602578] pixels wide, [0x8260257C] rows, the sheet's size at
// 0x82602570) cleared (memset) row by row.
void ClearCorner(uint8_t* base, uint32_t car);

// sub_820D4078 Vinyl::DrawDecals: for each slot that arrived (state other
// than 0, 1, 2), its TGA decoded (TGA::CreateImage into the frame), its file
// released (LoadSlot::ReleaseBuffer) and drawn (Vinyl::DrawDecal): a 1,024 x
// 1,024 layer over the whole sheet (region 9), layers 0 to 2 into the region
// at +216, layers 3 to 5 into regions 1 and 2, stickers into their record's
// region, mirrored by its +4; the pixels freed (VirtualFree). `stack` is the
// guest r1 at entry. Returns 1. Every slot it draws must have passed
// DecalsNative.
uint32_t DrawDecals(uint8_t* base, uint32_t car, uint32_t stack);

// Vinyl::Compress's call, made by the adapter from OnDecalLoaded's frame
// (its guest calls need the context): the size, where the sheet's header
// and data are, and the stack its frame is made below. Returns its r3.
using CompressCall = std::function<uint32_t(uint32_t width, uint32_t height,
                                            uint32_t header_in, uint32_t data_in)>;

// Whether OnDecalLoaded runs natively for `request` on `car` (nothing
// changed): the native heap (the caller's check), every slot DrawDecals
// would draw an uncompressed 24- or 32-bit TGA (TgaDecodable) whose file
// LoadSlot::ReleaseBuffer frees natively (no two the same pages), and, when
// the sheet would be compressed, its header 0 or the layer's
// (TextureFreeable).
bool DecalsNative(uint8_t* base, uint32_t request, uint32_t car);

// sub_820D5750 CarModel::OnDecalLoaded (Archive::Load's callback; r3 the
// request, r6 the car model): the first layer slot, else sticker slot, whose
// +4 is the request marked arrived (+20 = 3), the decals drawn (DrawDecals)
// and the slot reset (LoadSlot::Reset); then, unless a slot is still loading
// (state 2), the corner cleared (ClearCorner), the sheet compressed
// (`compress`) and +4088 = 3. `stack` is the guest r1 at entry. Returns the
// original's r3: the request, the slot, or Vinyl::Compress's.
uint32_t OnDecalLoaded(uint8_t* base, uint32_t request, uint32_t car, uint32_t stack,
                       const CompressCall& compress);

}  // namespace car_models
