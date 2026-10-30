// Voice chat (XHV, Xbox-only) isn't made (the user's call, 2026-10-30): the
// network manager's voice set-up (sub_8214F6E8) leaves out the engine
// (XHVCreateEngine sub_82578C78 -> sub_82578BF8 -> Voice::Init sub_8257B7A8:
// its talkers, XAudio voices, listener, critical section, event and thread),
// so the engine pointer (the manager's +0, 0x826C74A0) stays 0, as the
// manager's constructor (sub_8214F660) leaves it. Its readers in play see no
// engine: Network::Update (hooks/network.cpp) no longer calls the per-frame
// chat data read (sub_8214FA78) or the Live users' registration
// (sub_8214F810); the UI's headset checks (+52, sub_8226BCB8 and
// sub_8226EEC8) run only in a session (sub_82260908: the state at 826C6AA8 in
// 0x1000..0x100C, set only by system link and Live code, 0 in play); the rest
// is system link code (no mode for it); the release (sub_8214F690) checks for
// null. The voice thread's entry (sub_8257B5D8), whose override signaled its
// start and ended (so Voice::Poll sub_8257AF30 never ran), went with the
// engine: nothing makes the thread. See docs/functions/app-start-up.md and
// system-services.md, and B5 in docs/rexglue-removal.md.

#include "guest.h"

#include "base/log.h"
#include "helpers/clock.h"

namespace voice {

// The network manager's voice set-up without the engine: r4 (the machine id,
// 0 in play) to the manager's +456 and the tick count to +444, as the
// original stores them around the engine's creation (read only by the chat
// data read and system link code). Returns the tick count (the original's
// r3).
uint32_t SetUpWithoutEngine(uint8_t* base, uint32_t manager, uint32_t machine_id) {
  ITC_NATIVE_RUNNING("sub_8214F6E8 Network::SetUpVoice (no engine)");
  guest::StoreU32(base, manager + 456, machine_id);
  const uint32_t ticks = helpers::TickCount();
  guest::StoreU32(base, manager + 444, ticks);
  return ticks;
}

}  // namespace voice

// r3 = the network manager (826C74A0), r4 = the machine id. From the
// manager's set-up (sub_8214E190) once at boot.
extern "C" REX_FUNC(sub_8214F6E8) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = voice::SetUpWithoutEngine(base, ctx.r3.u32, ctx.r4.u32);
}
