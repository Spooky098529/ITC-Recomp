// The car models natively: adapters in front of src/car_models/
// (docs/functions/car-models.md), with the register steps the harness
// shares (hooks/car_models.h).

#include "hooks/car_models.h"

#include "base/flags.h"
#include "car_models/car_model.h"
#include "car_models/decals.h"
#include "car_models/file_loaded.h"
#include "car_models/load_slots.h"
#include "car_models/parts.h"
#include "car_models/plate.h"
#include "car_models/raster.h"
#include "car_models/tga.h"
#include "car_models/vinyl.h"
#include "hooks/archive.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/texture_compression.h"
#include "native/buffers.h"

ITC_FLAG_BOOL(native_car_models, true, "ITC",
              "Run the car models' pieces (docs/functions/car-models.md: LoadSlot::Reset "
              "sub_820D0730, LoadSlot::LoadShared sub_820D0860, with --native_heap LoadSlot::Load "
              "sub_820D07C8 and CarModel::LoadExterior sub_820D5EC8 (with LoadSetA sub_820D59D8 and "
              "LoadSetB sub_820D5AC0), the team "
              "emblem sub_820D3CF8, CarModel::LoadDecals sub_820D5B50, with --native_heap "
              "CarModel::ReloadDecals sub_820D5D20, SetVinylTextures "
              "sub_820D5F88, the wheels' set-up sub_820D9110 (with the rims sub_820D8BA8 and brakes "
              "sub_820D8C50), with --native_heap CarInterior::LoadSetCBody sub_820D67E8, "
              "CarInterior::Load sub_820D79B0 (its accessories' "
              "calls through the guest; CarInterior::LoadParts sub_820D7730 under it, no adapter), "
              "with --native_heap CarModel::MakePlate sub_820ED6D0 (with sub_820C4DD8 and "
              "sub_820ED630, no adapters: only it calls them; Resource::Release a guest call), "
              "with --native_heap and --native_models the file-loaded callbacks "
              "CarModel::OnFileLoaded sub_820D47A8 (with the buffer release sub_820C3CE0, no "
              "adapter) and OnFileLoadedB sub_820D4F80 (the texture providers' slot +16 and the "
              "originals where a callee's plain C++ declines guest calls), "
              "with --native_heap LoadSlot::ReleaseBuffer sub_820D0758, and the "
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

void CarModelsTeamEmblem(PPCContext& ctx, uint8_t* base) {
  // The original's jump table left in r11, r0, r12 and ctr.
  ctx.r11.s64 = ctx.r4.s64 - 18;
  if (ctx.r11.u32 <= 26) {
    ctx.r0.u64 = guest::LoadU8(base, 0x82004F60 + ctx.r11.u32);
    ctx.r12.s64 = int64_t(int32_t(0x820D3D2Cu)) + int64_t(ctx.r0.u64);
    ctx.ctr.u64 = ctx.r12.u64;
  }
  ctx.r3.s64 = cm::TeamEmblem(ctx.r4.u32);
}

void CarModelsLoadDecals(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::CarModelLoadDecals(base, ctx.r3.u32, hooks::NativeArchiveLoad(base));
}

void CarModelsLoadExterior(PPCContext& ctx, uint8_t* base) {
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = cm::CarModelLoadExterior(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32,
                                                 ctx.r13.u32, hooks::NativeArchiveLoad(base))) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820D5EC8(ctx, base);
}

void CarModelsReloadDecals(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || int32_t(guest::LoadU32(base, ctx.r3.u32 + 3724)) == 1) {
    __imp__sub_820D5D20(ctx, base);
    return;
  }
  ctx.r3.u64 = cm::CarModelReloadDecals(base, ctx.r3.u32, ctx.r1.u32,
                                        hooks::NativeArchiveLoad(base));
}

void CarModelsSetVinylTextures(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::CarModelSetVinylTextures(base, ctx.r3.u32);
}

void CarModelsWheelsSetUp(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 =
          cm::WheelsSetUp(base, ctx.r3.u32, ctx.r4.u32, hooks::NativeArchiveLoad(base))) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_820D9110(ctx, base);
}

void CarModelsCockpitLoadSetC(PPCContext& ctx, uint8_t* base) {
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = cm::CockpitLoadSetC(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, ctx.r13.u32,
                                            hooks::NativeArchiveLoad(base))) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820D67E8(ctx, base);
}

void CarModelsInteriorLoad(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> r3;
  // The accessories' slot +4 calls from the original's frame (112 bytes).
  guest_call::InFrame(ctx, base, 112, 0x820D7A40, [&](uint32_t) {
    r3 = cm::InteriorLoad(base, ctx.r3.u32, ctx.r4.u32, hooks::NativeArchiveLoad(base),
                          [&ctx, base](uint32_t set, uint32_t r4, uint32_t r5, int which) {
                            ctx.r3.u64 = set;
                            ctx.r4.u64 = r4;
                            ctx.r5.u64 = r5;
                            ctx.lr = which == 0 ? 0x820D7A40 : 0x820D7A94;
                            REX_CALL_INDIRECT_FUNC(
                                guest::LoadU32(base, guest::LoadU32(base, set) + 4));
                            return ctx.r3.u32;
                          });
  });
  ctx.lr = lr;
  if (!r3) {
    __imp__sub_820D79B0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void CarModelsMakePlate(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_820ED6D0(ctx, base);
    return;
  }
  const uint64_t regs[4] = {ctx.r7.u64, ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  const uint32_t header_a = ctx.r3.u32, data_a = ctx.r4.u32;
  const uint32_t header_b = ctx.r5.u32, data_b = ctx.r6.u32;
  const uint32_t stack = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> r3;
  // Resource::Release from its frame, with the original's return address.
  guest_call::InFrame(ctx, base, cm::kPlateFrameBytes, 0x820ED8E4, [&](uint32_t) {
    r3 = cm::MakePlate(base, header_a, data_a, header_b, data_b, regs, stack,
                       [&ctx, base](uint32_t surface) {
                         ctx.r3.u64 = surface;
                         ctx.lr = 0x820ED8E4;
                         sub_822A67B0(ctx, base);
                       });
  });
  ctx.lr = lr;
  if (!r3) {
    __imp__sub_820ED6D0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void CarModelsFileLoaded(PPCContext& ctx, uint8_t* base, cm::FileLoadedSet set) {
  const bool a = set == cm::FileLoadedSet::kA;
  const uint32_t request = ctx.r3.u32, buffer = ctx.r4.u32, car = ctx.r6.u32;
  if (!hooks::ModelsNativeLifetime() || !native::Buffers_NativeMemcpy() ||
      !cm::FileLoadedNative(base, set, request, car)) {
    (a ? __imp__sub_820D47A8 : __imp__sub_820D4F80)(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t stack = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  // The guest calls from the original's frame (1,552 bytes), with its
  // return addresses.
  guest_call::InFrame(ctx, base, cm::kFileLoadedFrameBytes, a ? 0x820D47F4 : 0x820D4FD0,
                      [&](uint32_t) {
    cm::FileLoadedCalls calls;
    calls.skeleton_bind = [&](uint32_t skeleton, uint32_t file) {
      ctx.r3.u64 = skeleton;
      ctx.r4.u64 = file;
      ctx.r5.u64 = 0;
      ctx.lr = a ? 0x820D483C : 0x820D5018;
      __imp__sub_82599A20(ctx, base);
    };
    calls.update_matrices = [&](uint32_t skeleton) {
      ctx.r3.u64 = skeleton;
      ctx.lr = a ? 0x820D4854 : 0x820D5030;
      __imp__sub_825998A0(ctx, base);
    };
    calls.xpr_create = [&](uint32_t xpr, uint32_t file) {
      ctx.r3.u64 = xpr;
      ctx.r4.u64 = file;
      ctx.lr = a ? 0x820D4DC8 : 0x820D55A0;
      __imp__sub_82596F68(ctx, base);
    };
    calls.provider_set = [&](uint32_t provider, uint32_t xpr) {
      ctx.r3.u64 = provider;
      ctx.r4.u64 = 0;
      ctx.r5.u64 = xpr;
      ctx.lr = a ? 0x820D4DEC : 0x820D55C4;
      REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, provider) + 16));
    };
    calls.plate_release = [&](uint32_t surface) {
      // From MakePlate's frame (944 bytes below this one).
      guest_call::InFrame(ctx, base, cm::kPlateFrameBytes, 0x820ED8E4, [&](uint32_t) {
        ctx.r3.u64 = surface;
        sub_822A67B0(ctx, base);
      });
    };
    cm::FileLoaded(base, set, request, buffer, car, stack, calls);
  });
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
}

void CarModelsOnFileLoaded(PPCContext& ctx, uint8_t* base) {
  CarModelsFileLoaded(ctx, base, cm::FileLoadedSet::kA);
}

void CarModelsOnFileLoadedB(PPCContext& ctx, uint8_t* base) {
  CarModelsFileLoaded(ctx, base, cm::FileLoadedSet::kB);
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
ITC_CAR_MODELS(820D3CF8, CarModelsTeamEmblem)
ITC_CAR_MODELS(820D5B50, CarModelsLoadDecals)
ITC_CAR_MODELS(820D5EC8, CarModelsLoadExterior)
ITC_CAR_MODELS(820D5F88, CarModelsSetVinylTextures)
ITC_CAR_MODELS(820D5D20, CarModelsReloadDecals)
ITC_CAR_MODELS(820D9110, CarModelsWheelsSetUp)
ITC_CAR_MODELS(820D67E8, CarModelsCockpitLoadSetC)
ITC_CAR_MODELS(820D79B0, CarModelsInteriorLoad)
ITC_CAR_MODELS(820ED6D0, CarModelsMakePlate)
ITC_CAR_MODELS(820D47A8, CarModelsOnFileLoaded)
ITC_CAR_MODELS(820D4F80, CarModelsOnFileLoadedB)
ITC_CAR_MODELS(820D4508, CarModelsCarModelRelease)
ITC_CAR_MODELS(820ECF08, CarModelsTextureCreate)
ITC_CAR_MODELS(820ECFC8, CarModelsTextureFree)
ITC_CAR_MODELS(820EEBC0, CarModelsTgaHeaderClear)
ITC_CAR_MODELS(820ED020, CarModelsTgaCreateImage)
ITC_CAR_MODELS(820EE638, CarModelsDrawDecal)
ITC_CAR_MODELS(820D5750, CarModelsOnDecalLoaded)
