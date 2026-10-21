// The archives' loader thread (sub_8209E118) with the reads done natively:
// an archive request's bytes are copied from BUILD.DAT or BUILD_CRS.DAT
// (src/game_data/) straight into its buffer, with no ReadFileEx, APC or
// alertable wait. The rest is the game's, called as the recompiled loop calls
// it: the request queue (sub_8209DD98, which also finishes requests and runs
// their callbacks on this thread), the loose reads (sub_8209DFA8, on the
// native file class), Ucl::Unpack (sub_82294610) and the loop's locks
// (sub_82592670 waits, sub_825926E8 signals). See B4 in
// docs/rexglue-removal.md.

#include "guest.h"
#include "guest_call.h"

#include "game_data/game_data.h"

#include <rex/logging.h>

#include <mutex>
#include <optional>

using guest_call::InFrame;

namespace {

// The loop's objects (0x826C0000 - 21832, - 21912, - 21872) and the flag that
// asks for r29 (0x826A0000 + 20744).
constexpr uint32_t kWork = 0x826BAAB8;      // r27: waited at the top
constexpr uint32_t kRequestDone = 0x826BAA68;  // r28: signaled once a request, then waited
constexpr uint32_t kIdle = 0x826BAA90;      // r29
constexpr uint32_t kIdleWanted = 0x826A5108;
// The read's overlapped (0x826A0000 + 20716), as ReadFileEx's completion left
// it: status +0, bytes +4, offset +8/+12.
constexpr uint32_t kOverlapped = 0x826A50EC;
constexpr uint32_t kFirstCrsId = 23011;
constexpr uint32_t kFrame = 128;

// The two archives, opened on first use and kept open.
std::optional<uint32_t> Archive(bool crs) {
  static std::mutex mutex;
  static std::optional<uint32_t> handles[2];
  std::lock_guard lock(mutex);
  auto& handle = handles[crs];
  if (!handle) {
    handle = game_data::Open(crs ? "DATA\\BUILD\\TXR\\BUILD_CRS.DAT" : "DATA\\BUILD\\TXR\\BUILD.DAT");
    if (!handle) {
      REXLOG_ERROR("archive streaming: can't open {}", crs ? "BUILD_CRS.DAT" : "BUILD.DAT");
    }
  }
  return handle;
}

// An archive request's read (sub_8209DD00's job): +16 the id, +56 the byte
// offset, +60 the sectors (2,048 bytes), +76 the buffer. False if it fails.
bool ReadRequest(uint8_t* base, uint32_t request) {
  const uint32_t id = guest::LoadU32(base, request + 16);
  const uint32_t offset = guest::LoadU32(base, request + 56);
  const uint32_t bytes = guest::LoadU32(base, request + 60) << 11;
  const uint32_t buffer = guest::LoadU32(base, request + 76);
  guest::StoreU32(base, kOverlapped + 8, offset);
  guest::StoreU32(base, kOverlapped + 12, 0);
  auto archive = Archive(id >= kFirstCrsId);
  if (!archive) {
    return false;
  }
  bool ok = true;
  uint32_t total = 0;
  guest::ForEachHostPart(base, buffer, bytes, [&](uint8_t* host, uint32_t part, uint32_t size) {
    if (!ok) {
      return;
    }
    auto read = game_data::ReadAt(*archive, uint64_t(offset) + part, host, size);
    ok = read.has_value();
    total += read.value_or(0);
  });
  if (!ok) {
    REXLOG_ERROR("archive streaming: can't read id {} ({} bytes at {:08X})", id, bytes, offset);
    return false;
  }
  // A read past the end gives fewer bytes, as ReadFileEx did.
  guest::StoreU32(base, kOverlapped, 0);
  guest::StoreU32(base, kOverlapped + 4, total);
  return true;
}

}  // namespace

// The loader thread's loop (no direct callers; never returns). Each time work
// is signaled, takes the requests in turn: state 5 reads from an archive,
// state 7 a loose file, states 1-2 read nothing; a compressed request chained
// to the current one (+84) is unpacked into its destination meanwhile; then
// the request-done signal is waited for (given here for state 5, as the read's
// APC did; by the loop for 1-2 and by the loose read for 7).
extern "C" REX_FUNC(sub_8209E118) {
  REX_FUNC_PROLOGUE();
  auto call = [&](uint32_t return_address, auto&& function) {
    InFrame(ctx, base, kFrame, return_address, [&](uint32_t) { function(); });
  };
  auto wait = [&](uint32_t object, uint32_t return_address) {
    ctx.r3.u64 = object;
    call(return_address, [&] { sub_82592670(ctx, base); });
  };
  auto signal = [&](uint32_t object, uint32_t return_address) {
    ctx.r3.u64 = object;
    call(return_address, [&] { sub_825926E8(ctx, base); });
  };
  for (;;) {
    wait(kWork, 0x8209E148);
    ctx.r3.u64 = 0;
    call(0x8209E1E4, [&] { sub_8209DD98(ctx, base); });
    uint32_t request = ctx.r3.u32;
    while (request) {
      const int32_t state = int32_t(guest::LoadU32(base, request + 52));
      if (state > 0 && state <= 2) {
        signal(kRequestDone, 0x8209E194);
      } else if (state == 5) {
        if (!ReadRequest(base, request)) {
          call(0x8209DD6C, [&] { sub_8209AC78(ctx, base); });
        }
      } else if (state == 7) {
        ctx.r3.u64 = request;
        call(0x8209E17C, [&] { sub_8209DFA8(ctx, base); });
      }
      if (uint32_t chained = guest::LoadU32(base, request + 84)) {
        ctx.r3.u64 = guest::LoadU32(base, chained + 88);
        ctx.r4.u64 = guest::LoadU32(base, chained + 76);
        ctx.r5.u64 = guest::LoadU32(base, chained + 72);
        call(0x8209E1B4, [&] { sub_82294610(ctx, base); });
      }
      if (int32_t(guest::LoadU32(base, request + 52)) == 5) {
        // The read is done: what the APC's routine (sub_8209D808) did.
        signal(kRequestDone, 0x8209E1D4);
      }
      wait(kRequestDone, 0x8209E1DC);
      ctx.r3.u64 = request;
      call(0x8209E1E4, [&] { sub_8209DD98(ctx, base); });
      request = ctx.r3.u32;
    }
    if (guest::LoadU32(base, kIdleWanted)) {
      guest::StoreU32(base, kIdleWanted, 0);
      signal(kIdle, 0x8209E208);
    }
  }
}
