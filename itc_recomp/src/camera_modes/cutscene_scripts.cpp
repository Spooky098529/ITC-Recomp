// The cutscene's scripts (docs/functions/camera-modes.md): Cutscene::
// Cutscene's big switch (the jump table 0x82009340, 0x821572A4 to
// 0x82158B48), each script's fade cues and shots. Plain C++ over guest
// memory, the original's loads and stores in its order, in its stack frame.

#include "camera_modes/camera_modes.h"

#include "base/log.h"

namespace camera_modes {

// A stub for now: part 2 of Cutscene::Cutscene's migration fills it (its
// cues by List16Insert at `frame` + 120, its shots by CutsceneInit and
// List16Insert into the cutscene's +952). Nothing calls CutsceneConstruct
// until part 3 writes its adapter.
uint32_t CutsceneScripts(uint8_t* base, uint32_t cutscene, uint32_t script, uint32_t vehicle,
                         uint32_t frame, const CutsceneScriptCalls& calls) {
  (void)base;
  (void)vehicle;
  (void)frame;
  (void)calls;
  ITC_LOG_ERROR("CutsceneScripts: not written yet (script {}); the cutscene's shots are missing",
                script);
  return cutscene;
}

}  // namespace camera_modes
