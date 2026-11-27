// The shader load's adapter: see shader_load.h.

#include "hooks/shader_load.h"

#include "graphics/shader_load.h"
#include "guest.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

namespace hooks {

uint64_t LoadShaders(uint32_t device, uint64_t mask, uint32_t sp, uint64_t r13) {
  // A 256-byte frame: the program control word at +208.
  const uint32_t frame = (sp - 256) & ~15u;
  guest::StoreU32(frame, sp);
  return graphics::shader_load::LoadShaders(device, mask, frame + 80);
}

}  // namespace hooks

// The vertex shader patcher (sub_822AE340): no longer called (the shader
// load above doesn't patch, docs/pc-renderer.md, step 2); only the lifted
// shader load, which no longer runs, names it. Its entry runs the lifted
// original.
ITC_NATIVE_ENTRY_ALWAYS(822AE340) {
  lifted::original::c822AE340(ctx);
}
