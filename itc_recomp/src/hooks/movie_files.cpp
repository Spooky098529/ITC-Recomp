// The movie player's file data source (the XDK media library's; vtable
// 8204DD50) on our own game data module (src/game_data/): the movie decoder's
// reads (64 KB a Read, on its two decoder threads, one source per stream) are
// copied from the file at the source's position straight into guest memory,
// with no read-ahead blocks, overlapped ReadFiles or waits. The source: +44
// the handle (-1 closed), +48 the end flag, +56 the start, +64 the size, +88
// the position, +104 the block size, +108 the block count, +116 the blocks to
// keep ahead, +120 flags (4: the handle was the caller's), +124 the blocks'
// memory, +132 the block list. A source on a handle that isn't ours (a path
// on another device, none in play) goes to the recompiled methods, which
// then read ahead through the SDK. Its Seek (+56) and Skip (+52) only set +88
// and +48, and stay recompiled. See B4 in docs/rexglue-removal.md.

#include "guest.h"

#include "game_data/game_data.h"

#include "base/flags.h"
#include "base/log.h"

#include <string>

ITC_FLAG_DECLARE(bool, log_file_opens);

namespace {

constexpr uint32_t kClosed = 0xFFFFFFFF;
constexpr uint32_t kFileNotFound = 0x80070002;  // HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
constexpr uint32_t kReadFault = 0x8007001E;     // HRESULT_FROM_WIN32(ERROR_READ_FAULT)

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return uint64_t(guest::LoadU32(base, address)) << 32 | guest::LoadU32(base, address + 4);
}

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

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

bool Ours(uint8_t* base, uint32_t source) {
  return game_data::IsHandle(guest::LoadU32(base, source + 44));
}

// The source's Reset (sub_82487BF8, its vtable +112): closed, at the end,
// every other field 0.
void Reset(uint8_t* base, uint32_t source) {
  for (uint32_t offset = 48; offset < 136; offset += 4) {
    guest::StoreU32(base, source + offset, 0);
  }
  guest::StoreU32(base, source + 44, kClosed);
  guest::StoreU32(base, source + 48, 1);
}

// Read and the looping Read: r3 = source, r4 = destination, r5 = size, r6 =
// the bytes read (out). Reads at +88, clamped to the end (+56 + +64), and
// moves +88 on; +48 = 1 when fewer bytes than asked were read (or none, at
// the end). 0, or an error when the file can't be read. The looping Read
// differs only in reading ahead past the end from the start.
void Read(PPCContext& ctx, uint8_t* base) {
  const uint32_t source = ctx.r3.u32, destination = ctx.r4.u32, size = ctx.r5.u32,
                 read_out = ctx.r6.u32;
  const uint32_t handle = guest::LoadU32(base, source + 44);
  guest::StoreU32(base, source + 48, 0);
  guest::StoreU32(base, read_out, 0);
  const uint64_t position = LoadU64(base, source + 88);
  const uint64_t end = LoadU64(base, source + 56) + LoadU64(base, source + 64);
  ctx.r3.u64 = 0;
  if (position >= end) {
    guest::StoreU32(base, source + 48, 1);
    return;
  }
  const uint32_t want = end - position < size ? uint32_t(end - position) : size;
  uint32_t total = 0;
  bool failed = false;
  guest::ForEachHostPart(base, destination, want, [&](uint8_t* host, uint32_t offset,
                                                      uint32_t bytes) {
    if (failed) {
      return;
    }
    auto read = game_data::ReadAt(handle, position + offset, host, bytes);
    failed = !read || *read != bytes;
    total += read.value_or(0);
  });
  StoreU64(base, source + 88, position + total);
  guest::StoreU32(base, read_out, total);
  if (total < size) {
    guest::StoreU32(base, source + 48, 1);
  }
  if (failed) {
    ITC_LOG_WARN("movie source {:08X}: read of {} bytes at {} failed", source, want, position);
    ctx.r3.u64 = kReadFault;
  }
}

}  // namespace

// OpenFile (vtable +108): r3 = source, r4 = path, r6-r9 = the set-up's
// arguments (the blocks' memory, block size, count, blocks ahead). Closes
// what the source had open (+48), opens the file, +64 = its size, then the
// set-up (+116) and its result. A missing file: 0x80070002 (as the SDK's
// failure gave through the source's +32), +44 = -1.
extern "C" REX_FUNC(sub_82487C48) {
  REX_FUNC_PROLOGUE();
  const uint32_t source = ctx.r3.u32;
  const std::string path = GuestString(base, ctx.r4.u32);
  if (ITC_FLAG(log_file_opens)) {
    ITC_LOG_INFO("movie open: source {:08X} \"{}\" from {:08X}", source, path, ctx.lr);
  }
  auto relative = game_data::RelativePath(path);
  if (!relative) {
    ITC_LOG_WARN("movie source: \"{}\" isn't game data: opened by the recompiled OpenFile", path);
    __imp__sub_82487C48(ctx, base);
    return;
  }
  const uint32_t blocks = ctx.r6.u32, block_size = ctx.r7.u32, count = ctx.r8.u32,
                 ahead = ctx.r9.u32;
  sub_824880D8(ctx, base);
  if (ctx.r3.s32 < 0) {
    return;
  }
  auto handle = game_data::Open(*relative);
  if (!handle) {
    ITC_LOG_WARN("movie source: no file \"{}\"", path);
    guest::StoreU32(base, source + 44, kClosed);
    ctx.r3.u64 = kFileNotFound;
    return;
  }
  guest::StoreU32(base, source + 44, *handle);
  StoreU64(base, source + 64, game_data::Size(*handle).value_or(0));
  ctx.r3.u64 = source;
  ctx.r4.u64 = blocks;
  ctx.r5.u64 = block_size;
  ctx.r6.u64 = count;
  ctx.r7.u64 = ahead;
  sub_82487F68(ctx, base);
}

// The set-up (vtable +116): r3 = source, r4 = the blocks' memory (0: allocate
// it), r5 = block size, r6 = count, r7 = blocks ahead. The recompiled one
// makes the blocks (a buffer, an overlapped and an event each) and primes
// them with reads; ours reads straight from the file, so only the fields the
// reads and seeks use are set: +48 = 0, +104 the block size, +116 the blocks
// ahead (1 for 0); no blocks (+108, +124 and +132 stay 0). 0.
extern "C" REX_FUNC(sub_82487F68) {
  REX_FUNC_PROLOGUE();
  const uint32_t source = ctx.r3.u32;
  if (!Ours(base, source)) {
    __imp__sub_82487F68(ctx, base);
    return;
  }
  guest::StoreU32(base, source + 48, 0);
  guest::StoreU32(base, source + 104, ctx.r5.u32);
  guest::StoreU32(base, source + 116, ctx.r7.u32 ? ctx.r7.u32 : 1);
  ctx.r3.u64 = 0;
}

// Close (vtable +48; from OpenFile and the destructor sub_82488FD8): r3 =
// source. Closes our handle (one the caller gave, flag 4, isn't ours) and
// resets the source; 0. A source that's closed with no blocks is reset as
// the recompiled one does; one on another handle, or with blocks, goes to it.
extern "C" REX_FUNC(sub_824880D8) {
  REX_FUNC_PROLOGUE();
  const uint32_t source = ctx.r3.u32;
  const uint32_t handle = guest::LoadU32(base, source + 44);
  const bool no_blocks =
      !guest::LoadU32(base, source + 124) && !guest::LoadU32(base, source + 132);
  if (!(game_data::IsHandle(handle) || handle == kClosed) || !no_blocks) {
    __imp__sub_824880D8(ctx, base);
    return;
  }
  if (handle != kClosed && !(guest::LoadU32(base, source + 120) & 4)) {
    game_data::Close(handle);
  }
  Reset(base, source);
  ctx.r3.u64 = 0;
}

// Read (vtable +60).
extern "C" REX_FUNC(sub_82488598) {
  REX_FUNC_PROLOGUE();
  if (!Ours(base, ctx.r3.u32)) {
    __imp__sub_82488598(ctx, base);
    return;
  }
  Read(ctx, base);
}

// The looping Read (vtable +128, from Read when flag 8 is set; never in
// play).
extern "C" REX_FUNC(sub_82488A10) {
  REX_FUNC_PROLOGUE();
  if (!Ours(base, ctx.r3.u32)) {
    __imp__sub_82488A10(ctx, base);
    return;
  }
  Read(ctx, base);
}
