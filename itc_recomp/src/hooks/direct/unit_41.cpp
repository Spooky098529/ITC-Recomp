// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 41's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "hooks/heap.h"
#include "hooks/race_hud_machine.h"
#include "lifted/originals.h"
#include "memory/c_runtime_heap.h"

// The race HUD machine's helpers and handlers through the register steps
// the harness had (src/hooks/race_hud_machine.cpp; the steps run the
// originals where the plain C++ declines).
// OpponentPrize sub_8216C820 and ItemBonus sub_8216C938: r4 the item id;
// r3 the answer.
ITC_DIRECT_ENTRY(8216C820) { hooks::RaceHudMachineOpponentPrize(ctx); }
ITC_DIRECT_ENTRY(8216C938) { hooks::RaceHudMachineItemBonus(ctx); }
// IsFirstDay sub_8216D3F0: no arguments; r3 the answer.
ITC_DIRECT_ENTRY(8216D3F0) { hooks::RaceHudMachineIsFirstDay(ctx); }
// OpenCareerPause sub_8216D8C8: r3 = the machine; r3 PauseMenu::Open's.
ITC_DIRECT_ENTRY(8216D8C8) { hooks::RaceHudMachineOpenCareerPause(ctx); }
// AwardPrize sub_8216E150: r3 = the machine; r3 as the original leaves it.
ITC_DIRECT_ENTRY(8216E150) { hooks::RaceHudMachineAwardPrize(ctx); }

// The C runtime heap (src/memory/c_runtime_heap.cpp, over the allocation
// layer): natively while --native_heap routes the game's heap to the layer
// (the removed adapters' condition), else the originals.
// LocalAlloc sub_82297F48: r3 = flags, r4 = size; r3 the block.
ITC_DIRECT_ENTRY(82297F48) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c82297F48(ctx);
    return;
  }
  ctx.r3.u64 = memory::crt::LocalAlloc(ctx.r3.u32, ctx.r4.u32);
}
// LocalFree sub_82297F90: r3 = the block; r3 0 when freed, else the block.
ITC_DIRECT_ENTRY(82297F90) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c82297F90(ctx);
    return;
  }
  ctx.r3.u64 = memory::crt::LocalFree(ctx.r3.u32);
}
// _calloc_impl sub_825AC248: r3 = count, r4 = size, r5 = where errno goes;
// r3 the block, or the original (its errno and new handler paths) where the
// plain C++ declines.
ITC_DIRECT_ENTRY(825AC248) {
  if (hooks::NativeHeapActive()) {
    if (const auto block = memory::crt::CallocImpl(ctx.r3.u32, ctx.r4.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  lifted::original::c825AC248(ctx);
}
