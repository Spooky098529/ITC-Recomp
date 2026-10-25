// The game's CreateFile (sub_82295508, the only caller of NtCreateFile).
//
// --log_file_opens logs every file the game opens through it (the movies'
// opens are logged by their native OpenFile, src/hooks/movie_files.cpp).
// (Failing the Ubisoft logo movie's open to skip it doesn't work: the game
// shows the dirty disc error.)

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"

#include <string>

ITC_FLAG_BOOL(log_file_opens, false, "ITC", "Log every file the game opens");

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
  if (ITC_FLAG(log_file_opens)) {
    ITC_LOG_INFO("file open: {} (from {:08X})", path, ctx.lr);
  }
  __imp__sub_82295508(ctx, base);
}
