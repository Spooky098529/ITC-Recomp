// The game's CreateFile (sub_82295508, the only caller of NtCreateFile).
//
// --log_file_opens logs every file the game opens. (Failing the Ubisoft logo
// movie's open to skip it doesn't work: the game shows the dirty disc error.)

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <string>

REXCVAR_DEFINE_BOOL(log_file_opens, false, "ITC", "Log every file the game opens");

// CreateFile: r3 = path; returns the handle, or -1.
extern "C" REX_FUNC(sub_82295508) {
  REX_FUNC_PROLOGUE();
  std::string path;
  for (uint32_t p = ctx.r3.u32; p && path.size() < 260; p++) {
    char c = char(guest::LoadU8(base, p));
    if (!c) {
      break;
    }
    path += c;
  }
  if (REXCVAR_GET(log_file_opens)) {
    REXLOG_INFO("file open: {} (from {:08X})", path, ctx.lr);
  }
  __imp__sub_82295508(ctx, base);
}

// Movie file open (the movie player's, through a vtable): r3 = player.
extern "C" REX_FUNC(sub_82487C48) {
  REX_FUNC_PROLOGUE();
  if (REXCVAR_GET(log_file_opens)) {
    uint32_t vtable = guest::LoadU32(base, ctx.r3.u32);
    // Return addresses up the guest stack: each frame's back chain at +0,
    // the caller's saved LR 8 bytes below the frame it points to.
    std::string callers;
    uint32_t frame = ctx.r1.u32;
    for (int i = 0; i < 8 && frame; i++) {
      uint32_t up = guest::LoadU32(base, frame);
      if (!up) {
        break;
      }
      callers += fmt::format(" {:08X}", guest::LoadU32(base, up - 8));
      frame = up;
    }
    REXLOG_INFO("movie open: player {:08X} vtable {:08X} from {:08X}, then{}", ctx.r3.u32,
                vtable, ctx.lr, callers);
  }
  __imp__sub_82487C48(ctx, base);
}
