// The car models natively: adapters in front of src/car_models/
// (docs/functions/car-models.md), with the register steps the harness
// shares (hooks/car_models.h).

#include "hooks/car_models.h"

#include "base/flags.h"
#include "car_models/car_model.h"
#include "car_models/decals.h"
#include "car_models/load_slots.h"
#include "car_models/raster.h"
#include "car_models/tga.h"
#include "car_models/vinyl.h"
#include "hooks/archive.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/texture_compression.h"

ITC_FLAG_BOOL(native_car_models, true, "ITC",
              "Run the car models' pieces (docs/functions/car-models.md: LoadSlot::Reset "
              "sub_820D0730, LoadSlot::LoadShared sub_820D0860, with --native_heap LoadSlot::Load "
              "sub_820D07C8 and CarModel::LoadSetA sub_820D59D8 and LoadSetB sub_820D5AC0, with --native_heap LoadSlot::ReleaseBuffer sub_820D0758, and the "
              "deferred release sub_820C3F50; with --native_heap and the memory blocks' host lock "
              "CarModel::Release sub_820D4508; with --native_heap the vinyl sheet's texture, "
              "Texture::Create sub_820ECF08 and Texture::Free sub_820ECFC8; the decals' TGA "
              "header's clear sub_820EEBC0 and, with --native_heap, TGA::CreateImage sub_820ED020 "
              "with its reader; Vinyl::DrawDecal sub_820EE638 with the software rasterizer under "
              "it; with --native_heap CarModel::OnDecalLoaded sub_820D5750 with Vinyl::DrawDecals "
              "sub_820D4078, Vinyl::ClearCorner sub_820D3C80 and Vinyl::Compress sub_820ED090, which "
              "calls XGCompressSurface sub_8235AAC0's plain C++) natively "
              "instead of through the translated code (the same writes; checked by "
              "--run_harnesses=car_models)");

namespace hooks {

namespace cm = car_models;

void CarModelsLoadSlotReset(PPCContext& ctx, uint8_t* base) {
  cm::LoadSlotReset(base, ctx.r3.u32);
}

void CarModelsLoadSlotReleaseBuffer(PPCContext& ctx, uint8_t* base) {
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = cm::LoadSlotReleaseBuffer(base, ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820D0758(ctx, base);
}

void CarModelsLoadSlotLoadShared(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::LoadSlotLoadShared(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                      hooks::NativeArchiveLoad(base));
}

void CarModelsLoadSlotLoad(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_820D07C8(ctx, base);
    return;
  }
  ctx.r3.u64 = cm::LoadSlotLoad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                ctx.r13.u32, hooks::NativeArchiveLoad(base));
}

void CarModelsLoadSetA(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_820D59D8(ctx, base);
    return;
  }
  ctx.r3.s64 = cm::CarModelLoadSetA(base, ctx.r3.u32, hooks::NativeArchiveLoad(base));
}

void CarModelsLoadSetB(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_820D5AC0(ctx, base);
    return;
  }
  ctx.r3.s64 = cm::CarModelLoadSetB(base, ctx.r3.u32, ctx.r13.u32, hooks::NativeArchiveLoad(base));
}

void CarModelsDeferRelease(PPCContext& ctx, uint8_t* base) {
  cm::DeferRelease(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}

void CarModelsCarModelRelease(PPCContext& ctx, uint8_t* base) {
  if (hooks::ModelsNativeLifetime()) {
    ctx.fpscr.disableFlushMode();  // as CarModel::Reset
    if (const auto r3 = cm::CarModelRelease(base, ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820D4508(ctx, base);
}

void CarModelsTextureCreate(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_820ECF08(ctx, base);
    return;
  }
  ctx.r3.u64 = cm::TextureCreate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                 ctx.r7.u32, ctx.r1.u32);
}

void CarModelsTextureFree(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !cm::TextureFreeable(base, ctx.r3.u32)) {
    __imp__sub_820ECFC8(ctx, base);
    return;
  }
  ctx.r3.u64 = cm::TextureFree(base, ctx.r3.u32, ctx.r4.u32);
}

uint32_t CarModelsVinylCompressAt(PPCContext& ctx, uint8_t* base, uint32_t width,
                                  uint32_t height, uint32_t header_in, uint32_t data_in) {
  const uint32_t stack = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> r3;
  // Resource::Release from its frame, with the original's return addresses.
  guest_call::InFrame(ctx, base, cm::kVinylCompressFrameBytes, 0x820ED1C0, [&](uint32_t) {
    const cm::VinylCalls calls{TextureCompressionGuestCall(ctx, base),
                               [&ctx, base](uint32_t surface, int which) {
                                 ctx.r3.u64 = surface;
                                 ctx.lr = which == 0 ? 0x820ED1C0 : 0x820ED1C8;
                                 sub_822A67B0(ctx, base);
                               }};
    r3 = cm::VinylCompress(base, width, height, header_in, data_in, stack, calls);
  });
  ctx.lr = lr;
  return *r3;  // the callers checked TextureFreeable
}

void CarModelsOnDecalLoaded(PPCContext& ctx, uint8_t* base) {
  const uint32_t request = ctx.r3.u32, car = ctx.r6.u32;
  if (!hooks::NativeHeapActive() || !cm::DecalsNative(base, request, car)) {
    __imp__sub_820D5750(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();  // as the original's lfs
  const uint32_t stack = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  // Vinyl::Compress from its frame (its guest calls), returning to 0x820D5880.
  guest_call::InFrame(ctx, base, cm::kOnDecalLoadedFrameBytes, 0x820D5880, [&](uint32_t) {
    r3 = cm::OnDecalLoaded(base, request, car, stack,
                           [&](uint32_t width, uint32_t height, uint32_t header_in,
                               uint32_t data_in) {
                             ctx.lr = 0x820D5880;
                             return CarModelsVinylCompressAt(ctx, base, width, height,
                                                             header_in, data_in);
                           });
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void CarModelsTgaHeaderClear(PPCContext& ctx, uint8_t* base) {
  cm::TgaHeaderClear(base, ctx.r3.u32);
  ctx.r11.u64 = 0;
}

void CarModelsTgaCreateImage(PPCContext& ctx, uint8_t* base) {
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = cm::TgaCreateImage(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                           ctx.r6.u32, ctx.r1.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820ED020(ctx, base);
}

void CarModelsDrawDecal(PPCContext& ctx, uint8_t* base) {
  if (!cm::DecalRegionPorted(ctx.r3.u32)) {
    __imp__sub_820EE638(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();  // as the original's lfs
  const cm::DecalArguments arguments{ctx.r3.u32,
                                     ctx.r4.u32,
                                     {ctx.r5.u64, ctx.r6.u64, ctx.r7.u64},
                                     {ctx.r8.u64, ctx.r9.u64},
                                     ctx.f1.f64};
  ctx.r3.u64 = *cm::DrawDecal(base, arguments, ctx.r1.u32);
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off.

#define ITC_CAR_MODELS(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_car_models)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

ITC_CAR_MODELS(820D0730, CarModelsLoadSlotReset)
ITC_CAR_MODELS(820D0758, CarModelsLoadSlotReleaseBuffer)
ITC_CAR_MODELS(820C3F50, CarModelsDeferRelease)
ITC_CAR_MODELS(820D0860, CarModelsLoadSlotLoadShared)
ITC_CAR_MODELS(820D07C8, CarModelsLoadSlotLoad)
ITC_CAR_MODELS(820D59D8, CarModelsLoadSetA)
ITC_CAR_MODELS(820D5AC0, CarModelsLoadSetB)
ITC_CAR_MODELS(820D4508, CarModelsCarModelRelease)
ITC_CAR_MODELS(820ECF08, CarModelsTextureCreate)
ITC_CAR_MODELS(820ECFC8, CarModelsTextureFree)
ITC_CAR_MODELS(820EEBC0, CarModelsTgaHeaderClear)
ITC_CAR_MODELS(820ED020, CarModelsTgaCreateImage)
ITC_CAR_MODELS(820EE638, CarModelsDrawDecal)
ITC_CAR_MODELS(820D5750, CarModelsOnDecalLoaded)
