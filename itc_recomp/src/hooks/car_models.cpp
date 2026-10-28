// The car models natively: adapters in front of src/car_models/
// (docs/functions/car-models.md), with the register steps the harness
// shares (hooks/car_models.h).

#include "hooks/car_models.h"

#include "base/flags.h"
#include "car_models/car_model.h"
#include "car_models/load_slots.h"
#include "car_models/tga.h"
#include "car_models/vinyl.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/texture_compression.h"

ITC_FLAG_BOOL(native_car_models, true, "ITC",
              "Run the car models' pieces (docs/functions/car-models.md: LoadSlot::Reset "
              "sub_820D0730, with --native_heap LoadSlot::ReleaseBuffer sub_820D0758, and the "
              "deferred release sub_820C3F50; with --native_heap and the memory blocks' host lock "
              "CarModel::Release sub_820D4508; with --native_heap the vinyl sheet's texture, "
              "Texture::Create sub_820ECF08, Texture::Free sub_820ECFC8 and Vinyl::Compress "
              "sub_820ED090, which calls XGCompressSurface sub_8235AAC0's plain C++; the decals' TGA "
              "header's clear sub_820EEBC0 and, with --native_heap, TGA::CreateImage sub_820ED020 "
              "with its reader) natively "
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

void CarModelsVinylCompress(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !cm::TextureFreeable(base, ctx.r5.u32)) {
    __imp__sub_820ED090(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();  // as the original's lfs
  const uint32_t width = ctx.r3.u32, height = ctx.r4.u32;
  const uint32_t header_in = ctx.r5.u32, data_in = ctx.r6.u32;
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
  ctx.r3.u64 = *r3;  // the checks above are VinylCompress's
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
ITC_CAR_MODELS(820D4508, CarModelsCarModelRelease)
ITC_CAR_MODELS(820ECF08, CarModelsTextureCreate)
ITC_CAR_MODELS(820ECFC8, CarModelsTextureFree)
ITC_CAR_MODELS(820ED090, CarModelsVinylCompress)
ITC_CAR_MODELS(820EEBC0, CarModelsTgaHeaderClear)
ITC_CAR_MODELS(820ED020, CarModelsTgaCreateImage)
