// Native callers' guest calls through the render manager's and the texture
// provider's vtables, made natively where the slot holds the game's function
// (hooks::RenderManagerSlotDirect, hooks::TextureProviderSlotDirect).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "hooks/app_start_up.h"
#include "hooks/models.h"

namespace hooks {

// A guest call of `function` with the registers set (lr the caller's, r1 its
// frame): true when it was one of those slots, made natively with the
// registers its adapter leaves; false where the caller makes the guest call.
inline bool DirectSlotCall(lifted::Regs& ctx, uint32_t function) {
  return RenderManagerSlotDirect(ctx, function) ||
         TextureProviderSlotDirect(ctx, function);
}

}  // namespace hooks
