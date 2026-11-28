// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 3's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "car_models/raster.h"
#include "car_models/tga.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "hooks/heap.h"
#include "hooks/scene.h"
#include "hooks/stand_models.h"
#include "hooks/type2_screens.h"
#include "hooks/type67_modes.h"

// --- The car models: the decals' rasterizer and TGA reader ---

// SoftRaster::DrawSpan: r3 the canvas, r4 the region, r5 xa, r6 xb, r7 y,
// r8 and r9 the ends' vertices (their texels the floats at +0 and +4), r10
// the image; the flags the word at the caller's frame + 84 (the original's
// 272-byte frame + 356).
ITC_DIRECT_ENTRY(820EDE00) {
  ctx.fpscr.disableFlushMode();  // as the original's lfs
  const car_models::SpanEnd a{double(guest::LoadFloat(ctx.r8.u32)),
                              double(guest::LoadFloat(ctx.r8.u32 + 4))};
  const car_models::SpanEnd b{double(guest::LoadFloat(ctx.r9.u32)),
                              double(guest::LoadFloat(ctx.r9.u32 + 4))};
  car_models::DrawSpan(ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r6.s32, ctx.r7.s32, a, b,
                       guest::LoadU32(ctx.r1.u32 + 84), ctx.r10.u32);
}
// TGA::ParseHeader: r3 the header, r4 the file.
ITC_DIRECT_ENTRY(820EEBF8) { ctx.r3.u64 = car_models::TgaParseHeader(ctx.r3.u32, ctx.r4.u32); }
// The image cleared and freed: r3 the image (left as it came).
ITC_DIRECT_ENTRY(820EED38) { car_models::TgaImageClear(ctx.r3.u32); }
ITC_DIRECT_ENTRY(820EED58) { car_models::TgaImageFree(ctx.r3.u32); }
// TGA::Decode: r3 the image, r4 the file, r5 the header. The port takes only
// uncompressed 24- and 32-bit files (TgaDecodable); the others (the RLE
// type 10, the reason the fallback originals run) the original.
ITC_DIRECT_ENTRY(820EF168) {
  if (!car_models::TgaDecodable(ctx.r4.u32)) {
    lifted::original::c820EF168(ctx);
    return;
  }
  ctx.r3.u64 = car_models::TgaDecode(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// --- The car specs ---

// Tire::InchUp: r3 the tire, r4 the inches (r3 left as it came).
ITC_DIRECT_ENTRY(820C46B8) {
  ctx.fpscr.disableFlushMode();
  car_specs::InchUp(ctx.r3.u32, ctx.r4.s32);
}
// CarSpec::GetTire: r3 the spec, r4 the type, r5 the grade, r6 rear.
ITC_DIRECT_ENTRY(820C9928) {
  ctx.r3.u64 = car_specs::GetTire(ctx.r3.u32, ctx.r4.s32, ctx.r5.u32, ctx.r6.s32);
}
// The wheel's and the car spec's swaps: r3 the record, r3 left past it.
ITC_DIRECT_ENTRY(820CB588) { ctx.r3.u64 = car_specs::SwapWheel(ctx.r3.u32); }
ITC_DIRECT_ENTRY(820CC168) { ctx.r3.u64 = car_specs::SwapCarSpec(ctx.r3.u32); }

// --- The type 6 and 7 modes ---

ITC_DIRECT_ENTRY(8217BE08) { hooks::CameraPresetConstruct(ctx); }
// GarageCamera::FreePresets: the original where the step refuses (a block
// not the allocation layer's pages), as its old adapter.
ITC_DIRECT_ENTRY(8217D760) {
  if (!hooks::NativeHeapActive() || !hooks::GarageCameraFreePresets(ctx)) {
    lifted::original::c8217D760(ctx);
  }
}
ITC_DIRECT_ENTRY(82180968) { hooks::GarageStageInit(ctx); }

// --- The type 2 mode's screens ---

ITC_DIRECT_ENTRY(8218F3F8) { hooks::HubShowNoticeImage(ctx); }
ITC_DIRECT_ENTRY(82199350) { hooks::PlateShowDigitCells(ctx); }

// --- The scene models and the scene ---

ITC_DIRECT_ENTRY(820D9E00) { hooks::StandModelsSceneAddSecondWheels(ctx); }
ITC_DIRECT_ENTRY(82593FF8) { hooks::SceneListDeleteAll(ctx); }
