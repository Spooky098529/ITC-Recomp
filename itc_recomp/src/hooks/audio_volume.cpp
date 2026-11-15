// The audio library's mixer pass that reads the voice category volumes
// (sub_8245B808; B2 in docs/rexglue-removal.md, docs/functions/audio.md)
// without its two XAudio imports: XAudioGetVoiceCategoryVolumeChangeMask
// answered 0 (no category's volume changed) every time, as the SDK did, so
// XAudioGetVoiceCategoryVolume was never asked. The rest are guest calls, in
// the recompiled function's order.

#include "guest.h"
#include "hooks/native_entry.h"
#include "hooks/guest_call.h"
#include "lifted/sdk.h"

using guest_call::InFrame;

// r3 the mixer object, r4 (low byte) whether to read the volumes.
ITC_NATIVE_ENTRY_ALWAYS(8245B808) {
  
  const uint32_t object = ctx.r3.u32;
  const bool read_volumes = (ctx.r4.u32 & 0xFF) != 0;
  InFrame(ctx, base, 144, 0x8245B810, [&](uint32_t) {
    if (read_volumes) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = 0;
      ctx.lr = 0x8245B830;
      lifted::CallFromContext(ctx, base, 0x8245ACE0);
      // The change mask: none.
      guest::StoreU32(base, object + 140, 0);
      // The pending count at [82954F08]+304, less one, released to the
      // semaphore at 82954ED4.
      const uint32_t count = guest::LoadU32(base, guest::LoadU32(base, 0x82954F08) + 304);
      if (int32_t(count) > 1) {
        ctx.r3.u64 = 0x82954ED4;
        ctx.r4.u64 = 1;
        ctx.r5.u64 = count - 1;
        ctx.r6.u64 = 0;
        ctx.lr = 0x8245B8B4;
        lifted::Sdk(ctx, base, __imp__KeReleaseSemaphore);
      }
    }
    ctx.r3.u64 = object;
    ctx.r4.u64 = object + 80;
    ctx.lr = 0x8245B8C0;
    lifted::CallFromContext(ctx, base, 0x8245B480);
    ctx.r3.u64 = object;
    ctx.r4.u64 = object + 356;
    ctx.lr = 0x8245B8CC;
    lifted::CallFromContext(ctx, base, 0x8245A940);
    uint32_t which = 1;
    for (uint32_t i = 0; i < guest::LoadU8(base, object + 128); i++) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = guest::LoadU32(base, object + 124) + 44 * i;
      ctx.lr = 0x8245B8F0;
      lifted::CallFromContext(ctx, base, 0x8245B480);
      ctx.r3.u64 = object;
      ctx.r4.u64 = object + 8 * which + 356;
      ctx.lr = 0x8245B904;
      lifted::CallFromContext(ctx, base, 0x8245A940);
      which ^= 1;
    }
    if (read_volumes) {
      guest::StoreU32(base, object + 140, 0);
      ctx.r3.u64 = object;
      ctx.r4.u64 = 1;
      ctx.lr = 0x8245B938;
      lifted::CallFromContext(ctx, base, 0x8245ACE0);
    }
  });
}
