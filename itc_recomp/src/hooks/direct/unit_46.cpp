// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 46's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include "lifted/originals.h"

#include "boot_mode/boot_mode.h"
#include "hooks/boot_mode.h"
#include "racer_controllers/racer_controllers.h"

// CarInputs::Create and Destroy: no arguments; r3 = the last record, as
// operator new (operator delete) leaves it. The original where the port
// declines (the native heap off; a record not the layer's).
ITC_DIRECT_ENTRY(82102258) {
  uint32_t last = 0;
  if (racer_controllers::CreateInputs(&last)) {
    ctx.r3.u64 = last;
    return;
  }
  lifted::original::c82102258(ctx);
}
ITC_DIRECT_ENTRY(82102300) {
  uint32_t last = 0;
  if (racer_controllers::DestroyInputs(&last)) {
    ctx.r3.u64 = last;
    return;
  }
  lifted::original::c82102300(ctx);
}

// Movies::Start: no arguments; slot 2 as a tail call from the caller's frame
// (r1), returning to the caller (lr); r3 as the slot leaves it.
ITC_DIRECT_ENTRY(820A43D8) {
  const uint64_t lr = ctx.lr;
  const boot_mode::Calls calls = hooks::BootModeCalls(ctx);
  ctx.r3.u64 = boot_mode::MoviesStart(ctx.r1.u32, uint32_t(lr), calls);
  ctx.lr = lr;
}
