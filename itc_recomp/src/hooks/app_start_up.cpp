// The app's start-up natively: adapters in front of src/app_start_up/
// (docs/functions/app-start-up.md).

#include "app_start_up/start_up.h"

#include "base/flags.h"
#include "guest.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"

using guest_call::InFrame;

ITC_FLAG_BOOL(native_app_start_up, true, "ITC",
              "Run the app's start-up (docs/functions/app-start-up.md: ReadError::SetApp "
              "sub_8209AC50, sub_8209AC30, sub_8209F580, sub_8225FB70, Movies::Set "
              "sub_820A4360 and, with --native_heap, CreateVertexDeclaration sub_822A8980) "
              "natively instead of through the translated code (the same reads and writes; "
              "checked by --run_harnesses=app_start_up)");

namespace asu = app_start_up;

extern "C" REX_FUNC(sub_8209AC50) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8209AC50(ctx, base);
    return;
  }
  asu::ReadErrorSetApp(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_8209AC30) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8209AC30(ctx, base);
    return;
  }
  asu::SetAppInterface84(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_8209F580) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8209F580(ctx, base);
    return;
  }
  asu::SetRenderApp(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_8225FB70) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8225FB70(ctx, base);
    return;
  }
  asu::ClearWords8110(base);
}

extern "C" REX_FUNC(sub_820A4360) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_app_start_up)) {
    if (const auto movie = asu::MoviesSet(base, ctx.r3.u32)) {
      ctx.r3.u64 = *movie;
      return;
    }
  }
  __imp__sub_820A4360(ctx, base);
}

extern "C" REX_FUNC(sub_822A8980) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_app_start_up) && hooks::NativeHeapActive()) {
    const auto declaration =
        asu::CreateVertexDeclaration(base, ctx.r3.u32, [&](uint32_t to, uint32_t from, uint32_t size) {
          InFrame(ctx, base, 160, 0x822A8A54, [&](uint32_t) {
            ctx.r3.u64 = to;
            ctx.r4.u64 = from;
            ctx.r5.u64 = size;
            sub_825A7410(ctx, base);  // memcpy
          });
        });
    if (declaration) {
      ctx.r3.u64 = *declaration;
      return;
    }
  }
  __imp__sub_822A8980(ctx, base);
}
