// The shader load's adapter: see shader_load.h.

#include "hooks/shader_load.h"

#include "graphics/shader_load.h"
#include "guest.h"
#include "hooks/lifted_call.h"
#include "lifted/functions.h"

namespace hooks {

uint64_t LoadShaders(uint32_t device, uint64_t mask, uint32_t sp, uint64_t r13) {
  // A 256-byte frame: the remap's scratch at +80 (128 bytes) and the program
  // control word at +208; the calls' own frames below it.
  const uint32_t frame = (sp - 256) & ~15u;
  guest::StoreU32(frame, sp);
  graphics::shader_load::Calls calls;
  // Through its entry, whose wrap keeps the patched copy for the renderer
  // (src/native/replay_hooks.cpp).
  calls.patch = [&](uint32_t shader, uint32_t microcode, uint32_t declaration,
                    uint32_t streams) {
    lifted_call::Call(0x822AE340, frame, r13, {shader, microcode, declaration, streams});
  };
  calls.remap_exports = [&](uint32_t scratch, uint32_t vertex_header, uint32_t copy,
                            uint32_t program_control, uint32_t pixel_header) {
    lifted::f822AEAA0(frame, r13, scratch, vertex_header, copy, program_control, pixel_header);
  };
  return graphics::shader_load::LoadShaders(device, mask, frame + 80, calls);
}

}  // namespace hooks
