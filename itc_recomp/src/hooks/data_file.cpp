// The game's container files natively: adapters in front of
// src/game_data/data_file.cpp (docs/functions/files-and-saves.md,
// DataFile::). DataFile::GetEntry has many recompiled callers (the light
// sets, the course and car data, the font), so its adapter stays.

#include "guest.h"

#include "base/flags.h"
#include "game_data/data_file.h"

ITC_FLAG_BOOL(native_data_files, true, "ITC",
              "Run the game's container files (docs/functions/files-and-saves.md: "
              "DataFile::GetEntry sub_82294730) natively instead of through the translated "
              "code (the same results; checked by --run_harnesses=data_file). Native callers "
              "call it whatever this says");

// DataFile::GetEntry: r3 = the container, r4 = the index, r5's low byte =
// whether an entry whose first word is 0 counts as none; r3 = the entry or
// 0. The original's scratch registers (r8 to r11) aren't read by its callers.
extern "C" REX_FUNC(sub_82294730) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_data_files)) {
    __imp__sub_82294730(ctx, base);
    return;
  }
  ctx.r3.u64 = game_data::GetEntry(base, ctx.r3.u32, ctx.r4.u32, (ctx.r5.u32 & 0xFF) != 0);
}
