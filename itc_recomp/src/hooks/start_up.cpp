// The C runtime's start-up checks (from xstart), without their kernel calls:
// the PAL-50 check and the heap check. Both run once at boot. See B5 in
// docs/rexglue-removal.md.

#include "guest.h"
#include "lifted/functions.h"
#include "hooks/native_entry.h"
#include "guest_call.h"

#include "base/log.h"
#include "schema/system_services.h"

#include <cstdlib>
#include "lifted/sdk.h"

using guest_call::InFrame;

namespace {

// The heap xstart makes (sub_822997B0's handle, at 0x8297149C).
using G = schema::SystemServicesGlobals;
// Its size: the image header's default heap size (field 0x20401) when there
// is one, else 1 MB. default.xex has none.
constexpr uint32_t kHeapBytes = 0x100000;

// sub_82297C08's work: makes the heap once (sub_822997B0, kept at G::heap)
// with a 48-byte attributes block (its size, then zeros), in a frame like
// the original's. True when the heap exists.
bool MakeHeap(lifted::Regs& ctx) {
  if (!G::heap()) {
    InFrame(ctx, 160, 0x82297CB0, [&](uint32_t frame) {
      const uint32_t attributes = frame + 80;
      for (uint32_t i = 0; i < 48; i += 4) {
        guest::StoreU32(attributes + i, 0);
      }
      guest::StoreU32(attributes, 48);
      ctx.r3.u64 = 2;
      ctx.r4.u64 = 0;
      ctx.r5.u64 = kHeapBytes;
      ctx.r6.u64 = 4096;
      ctx.r7.u64 = 0;
      ctx.r8.u64 = attributes;
      lifted::Enter(ctx, lifted::e822997B0);
      G::set_heap(ctx.r3.u32);
    });
  }
  return G::heap() != 0;
}

}  // namespace

// The XDK's PAL-50 check: 1 when it showed the 50 Hz console warning (then
// xstart ends the title). The SDK's AV pack (XGetAVPack) is 6, which passes
// before any console setting is read, so it's always 0.
ITC_NATIVE_ENTRY_ALWAYS(82296AB0) {
  
  ctx.r3.u64 = 0;
}

// Makes the heap (RtlImageXexHeaderField for its size left out: kHeapBytes);
// returns 1 when it exists.
ITC_NATIVE_ENTRY_ALWAYS(82297C08) {
  
  ctx.r3.u64 = MakeHeap(ctx) ? 1 : 0;
}

// The heap check: the game can't run without the heap, so the original
// returned to the firmware (HalReturnToFirmware, which the SDK answered by
// exiting); ours exits.
ITC_NATIVE_ENTRY_ALWAYS(82297CE0) {
  
  if (!MakeHeap(ctx)) {
    ITC_LOG_ERROR("The game's heap couldn't be made");
    std::exit(EXIT_FAILURE);
  }
  ctx.r3.u64 = 1;
}
