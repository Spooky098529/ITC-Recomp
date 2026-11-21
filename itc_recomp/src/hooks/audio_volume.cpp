// The audio library's mixer pass that reads the voice category volumes
// (sub_8245B808; B2 in docs/rexglue-removal.md, docs/functions/audio.md)
// without its two XAudio imports: XAudioGetVoiceCategoryVolumeChangeMask
// answered 0 (no category's volume changed) every time, as the SDK did, so
// XAudioGetVoiceCategoryVolume was never asked. The rest are guest calls, in
// the recompiled function's order.

#include "guest.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "hooks/native_entry.h"
#include "hooks/guest_call.h"
#include "lifted/sdk.h"
#include "schema/xaudio.h"
#include "schema/xaudio_engine.h"

using guest_call::InFrame;
using Engine = schema::XAudioEngine;

// r3 the mixer object, r4 (low byte) whether to read the volumes.
ITC_NATIVE_ENTRY_ALWAYS(8245B808) {
  
  const uint32_t object = ctx.r3.u32;
  const Engine engine(base, object);
  const bool read_volumes = (ctx.r4.u32 & 0xFF) != 0;
  InFrame(ctx, base, 144, 0x8245B810, [&](uint32_t) {
    if (read_volumes) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = 0;
      ctx.lr = 0x8245B830;
      lifted::Enter(ctx, base, game::XAudioEngine::RunCallbacks);
      // The change mask: none.
      engine.set_changed_categories(0);
      // The engine's processor count ([82954F08]+304), less one, released to the
      // workers' semaphore at 82954ED4.
      const uint32_t count = schema::XaudioGlobals::engine(base).processor_count();
      if (int32_t(count) > 1) {
        ctx.r3.u64 = schema::XaudioEngineGlobals::worker_semaphore_address();
        ctx.r4.u64 = 1;
        ctx.r5.u64 = count - 1;
        ctx.r6.u64 = 0;
        ctx.lr = 0x8245B8B4;
        lifted::Sdk(ctx, base, __imp__KeReleaseSemaphore);
      }
    }
    ctx.r3.u64 = object;
    ctx.r4.u64 = engine.source_stage_address();
    ctx.lr = 0x8245B8C0;
    lifted::Enter(ctx, base, game::XAudioEngine::RunStage);
    ctx.r3.u64 = object;
    ctx.r4.u64 = engine.stage_barriers_address();
    ctx.lr = 0x8245B8CC;
    lifted::Enter(ctx, base, game::XAudioEngine::Barrier);
    uint32_t which = 1;
    for (uint32_t i = 0; i < engine.submix_stage_count(); i++) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = engine.submix_stages() + 44 * i;
      ctx.lr = 0x8245B8F0;
      lifted::Enter(ctx, base, game::XAudioEngine::RunStage);
      ctx.r3.u64 = object;
      ctx.r4.u64 = engine.stage_barriers_address() + 8 * which;
      ctx.lr = 0x8245B904;
      lifted::Enter(ctx, base, game::XAudioEngine::Barrier);
      which ^= 1;
    }
    if (read_volumes) {
      engine.set_changed_categories(0);
      ctx.r3.u64 = object;
      ctx.r4.u64 = 1;
      ctx.lr = 0x8245B938;
      lifted::Enter(ctx, base, game::XAudioEngine::RunCallbacks);
    }
  });
}
