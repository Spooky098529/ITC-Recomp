// The game's file class on our own game data module (src/game_data/): open,
// read, seek and close read files from the data root directly, with handles of
// our own. The object: +4 the handle (-1 closed), +8 the game's position, +12
// the size. In play every whole-file load goes through it (the archives'
// TOCs, DEBFONT.XPR, TXR_XACT.xgs) and the loader thread's loose files (the
// audio banks). A path on another device than game:\ (none in play) goes to
// the recompiled open, and its handle to the recompiled read, seek and close.
// See B4 in docs/rexglue-removal.md.

#include "guest.h"
#include "lifted/functions.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"
#include "guest_call.h"
#include "schema/files_and_saves.h"

#include "game_data/game_data.h"

#include "base/log.h"

#include <string>
#include <vector>
#include "lifted/sdk.h"

using guest_call::InFrame;
using schema::File;

namespace {

constexpr uint32_t kClosed = 0xFFFFFFFF;

std::string GuestString(uint8_t* base, uint32_t address, uint32_t max = 260) {
  std::string s;
  for (uint32_t i = 0; address && i < max; i++) {
    char c = char(guest::LoadU8(base, address + i));
    if (!c) {
      break;
    }
    s += c;
  }
  return s;
}

// The read error report (sub_8209AC78), as the recompiled class calls it
// (from its own frame).
void ReportError(lifted::Regs& ctx, uint8_t* base, uint32_t return_address) {
  InFrame(ctx, base, 112, return_address, [&](uint32_t) { lifted::Enter(ctx, base, lifted::e8209AC78); });
}

}  // namespace

// File::OpenRead: r3 = file, r4 = path. Fails (the error report, 0) when the
// file is already open or isn't found; else +4 = the handle, +12 = the size
// (the low word, as GetFileSize gave), 1.
ITC_NATIVE_ENTRY_ALWAYS(8258C688) {
  
  const File file(base, ctx.r3.u32);
  const std::string path = GuestString(base, ctx.r4.u32);
  auto relative = game_data::RelativePath(path);
  if (!relative) {
    ITC_LOG_WARN("file class: \"{}\" isn't game data: opened by the recompiled File::OpenRead",
                 path);
    lifted::original::c8258C688(ctx, base);
    return;
  }
  if (file.handle() != kClosed) {
    ReportError(ctx, base, 0x8258C700);
    ctx.r3.u64 = 0;
    return;
  }
  auto handle = game_data::Open(*relative);
  if (!handle) {
    ITC_LOG_WARN("file class: no file \"{}\"", path);
    file.set_handle(kClosed);
    ReportError(ctx, base, 0x8258C700);
    ctx.r3.u64 = 0;
    return;
  }
  file.set_handle(*handle);
  file.set_size(uint32_t(game_data::Size(*handle).value_or(0)));
  ctx.r3.u64 = 1;
}

// File::Read: r3 = file, r4 = buffer, r5 = the size (a pointer). Reads at the
// handle's position, adds the bytes read to +8 and returns them; the error
// report and 0 when the read fails.
ITC_NATIVE_ENTRY_ALWAYS(8258C820) {
  
  const File file(base, ctx.r3.u32);
  const uint32_t buffer = ctx.r4.u32;
  const uint32_t handle = file.handle();
  if (!game_data::IsHandle(handle)) {
    lifted::original::c8258C820(ctx, base);
    return;
  }
  const uint32_t size = schema::FileSizeArg(base, ctx.r5.u32).size();
  // A blob (E5 step 5: a file's data in PC memory) ends at its size, its host
  // block's end, where guest memory went on: bytes past it are read and
  // dropped (logged), so the position and the count are the original's.
  uint32_t room = size;
  if (uint32_t left = 0; memory::BlobPointer(buffer, &left) && size > left) {
    ITC_LOG_WARN("file class: a read of {} bytes into a blob at {:08X} with {} left: the rest "
                 "not stored",
                 size, buffer, left);
    room = left;
  }
  uint32_t total = 0;
  bool failed = false;
  guest::ForEachHostPart(base, buffer, room, [&](uint8_t* host, uint32_t, uint32_t bytes) {
    if (failed) {
      return;
    }
    auto read = game_data::Read(handle, host, bytes);
    failed = !read;
    total += read.value_or(0);
  });
  if (!failed && room < size && total == room) {
    std::vector<uint8_t> scratch(size - room);
    auto read = game_data::Read(handle, scratch.data(), size - room);
    failed = !read;
    total += read.value_or(0);
  }
  if (failed) {
    ReportError(ctx, base, 0x8258C880);
    ctx.r3.u64 = 0;
    return;
  }
  file.set_position(file.position() + total);
  ctx.r3.u64 = total;
}

// File::Seek: r3 = file, r4 = the offset (a pointer), r5 = the method (a
// pointer: 0 the start, 1 the current position, 2 the end). Moves the handle's
// position; the game's own (+8) is set for 0 and moved by the offset for 1
// and 2, as the recompiled one does. The error report and 0 when it fails.
ITC_NATIVE_ENTRY_ALWAYS(8258C768) {
  
  const File file(base, ctx.r3.u32);
  const uint32_t handle = file.handle();
  if (!game_data::IsHandle(handle)) {
    lifted::original::c8258C768(ctx, base);
    return;
  }
  const int32_t offset = schema::FileSeekOffsetArg(base, ctx.r4.u32).offset();
  const uint32_t method = schema::FileSeekMethodArg(base, ctx.r5.u32).method();
  if (!game_data::Seek(handle, offset, method > 2 ? 0 : method)) {
    ReportError(ctx, base, 0x8258C810);
    ctx.r3.u64 = 0;
    return;
  }
  if (method == 0) {
    file.set_position(uint32_t(offset));
  } else if (method < 3) {
    file.set_position(file.position() + uint32_t(offset));
  }
  ctx.r3.u64 = 1;
}

// File::Close: r3 = file. Closes the handle (+4 = -1) and clears +8 and +12.
ITC_NATIVE_ENTRY_ALWAYS(8258C718) {
  
  const File file(base, ctx.r3.u32);
  const uint32_t handle = file.handle();
  if (!game_data::IsHandle(handle)) {
    lifted::original::c8258C718(ctx, base);
    return;
  }
  game_data::Close(handle);
  file.set_handle(kClosed);
  file.set_position(0);
  file.set_size(0);
}
