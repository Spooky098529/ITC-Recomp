// The game's CreateFile (sub_82295508, the only caller of NtCreateFile).
//
// --log_file_opens logs every file the game opens through it (the movies'
// opens are logged by their native OpenFile, src/hooks/movie_files.cpp).
// (Failing the Ubisoft logo movie's open to skip it doesn't work: the game
// shows the dirty disc error.)

#include "guest.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_data/create_file.h"
#include "hooks/create_file.h"
#include "hooks/lifted_call.h"

#include <string>

ITC_FLAG_BOOL(log_file_opens, false, "ITC", "Log every file the game opens");
ITC_FLAG_BOOL(native_create_file, true, "ITC",
              "Run the game's CreateFile (sub_82295508, docs/functions/files-and-saves.md) "
              "natively inside its --log_file_opens wrap instead of through the translated "
              "code (NtCreateFile through its import as the original; checked by "
              "--run_harnesses=create_file)");

namespace hooks {

// CreateFile's register steps (the adapter's and the harness's): r3 the
// path, r4 the access, r5 the sharing, r7 the disposition, r8 the flags.
ITC_REGS_STEP(CreateFileSteps) {
  lifted::Enter(ctx, base, [](uint8_t* base, const lifted::Args& a) -> lifted::Result {
    return {game_data::CreateFile(base, uint32_t(a.r1), a.r13, uint32_t(a.r[0]),
                                  uint32_t(a.r[1]), uint32_t(a.r[2]), uint32_t(a.r[4]),
                                  uint32_t(a.r[5])),
            a.f[0]};
  });
}

}  // namespace hooks

// CreateFile: r3 = path; returns the handle, or -1.
ITC_NATIVE_ENTRY_ALWAYS(82295508) {
  
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
  if (ITC_FLAG(native_create_file)) {
    hooks::CreateFileSteps(ctx, base);
  } else {
    lifted::Sdk(ctx, base, __imp__sub_82295508);
  }
}
