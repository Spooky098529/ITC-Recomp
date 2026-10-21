// The archives' start-up (Archive::Init, sub_8209E210) and loader thread
// (sub_8209E118) with the file work done natively:
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

#include <optional>
#include <string>
#include <utility>

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

// The archives' handles, as Archive::Init keeps them: BUILD.DAT, BUILD_CRS.DAT.
constexpr uint32_t kArchiveHandles[2] = {0x82600A9C, 0x82600AA0};

// An archive's handle (ours, from Archive::Init), or nothing.
std::optional<uint32_t> Archive(uint8_t* base, bool crs) {
  uint32_t handle = guest::LoadU32(base, kArchiveHandles[crs]);
  if (!game_data::IsHandle(handle)) {
    REXLOG_ERROR("archive streaming: {} isn't open", crs ? "BUILD_CRS.DAT" : "BUILD.DAT");
    return std::nullopt;
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
  auto archive = Archive(base, id >= kFirstCrsId);
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

std::string GuestString(uint8_t* base, uint32_t address, uint32_t max = 260) {
  std::string s;
  for (uint32_t i = 0; i < max; i++) {
    char c = char(guest::LoadU8(base, address + i));
    if (!c) {
      break;
    }
    s += c;
  }
  return s;
}

// Opens the game data file a path in the image names (game:\...).
std::optional<uint32_t> OpenImagePath(uint8_t* base, uint32_t path) {
  std::string name = GuestString(base, path);
  auto relative = game_data::RelativePath(name);
  auto handle = relative ? game_data::Open(*relative) : std::nullopt;
  if (!handle) {
    REXLOG_ERROR("archive streaming: can't open \"{}\"", name);
  }
  return handle;
}

// Reads a whole file (at most `size` bytes) into guest memory; false if it
// can't be opened or read.
bool ReadWhole(uint8_t* base, uint32_t path, uint32_t buffer, uint32_t size) {
  auto handle = OpenImagePath(base, path);
  if (!handle) {
    return false;
  }
  bool ok = true;
  guest::ForEachHostPart(base, buffer, size, [&](uint8_t* host, uint32_t part, uint32_t bytes) {
    ok = ok && game_data::ReadAt(*handle, part, host, bytes).has_value();
  });
  game_data::Close(*handle);
  return ok;
}

}  // namespace

// Archive::Init: r3 = the streaming object, r4 = the size of an extra buffer
// (0 for none). Once at boot. As the recompiled one, but the TOCs are read
// straight into their buffers and the archives opened as game data (their
// handles at 0x82600A9C, 0x82600AA0), with no file class or CreateFile. The
// allocations (sub_82295398, VirtualAlloc), the locks taken and the loader
// thread's start (the object's vtable +4, with the thread's parameters at
// 0x82600B50) are the game's.
extern "C" REX_FUNC(sub_8209E210) {
  REX_FUNC_PROLOGUE();
  const uint32_t object = ctx.r3.u32, extra_size = ctx.r4.u32;
  // A word the recompiled one copies from its own frame without setting it
  // (r1 - 208 + 116): whatever is there.
  const uint32_t stale = guest::LoadU32(base, ctx.r1.u32 - 92);
  auto call = [&](uint32_t return_address, auto&& function) {
    InFrame(ctx, base, 208, return_address, [&](uint32_t) { function(); });
  };
  auto allocate = [&](uint32_t size, uint32_t type, uint32_t return_address) {
    ctx.r3.u64 = 0;
    ctx.r4.u64 = size;
    ctx.r5.u64 = type;
    ctx.r6.u64 = 4;
    call(return_address, [&] { sub_82295398(ctx, base); });
    return ctx.r3.u32;
  };
  auto report_error = [&](uint32_t return_address) {
    call(return_address, [&] { sub_8209AC78(ctx, base); });
  };
  // The TOCs' sizes: the entry counts built into the image, plus a header.
  const uint32_t toc_size[2] = {(guest::LoadU32(base, 0x82600AA8) + 1) << 4,
                                (guest::LoadU32(base, 0x82600AB8) + 1) << 4};
  guest::StoreU32(base, 0x826A5124, toc_size[0]);
  guest::StoreU32(base, 0x826A5100, object);
  guest::StoreU32(base, 0x826A512C, toc_size[1]);
  // The loader thread's parameters.
  constexpr uint32_t kThread = 0x82600B50;
  guest::StoreU32(base, kThread + 8, 0);
  guest::StoreU32(base, kThread + 12, 0);
  guest::StoreU8(base, kThread + 16, 0);
  guest::StoreU8(base, kThread + 17, 1);
  guest::StoreU8(base, kThread + 18, 0);
  guest::StoreU32(base, kThread + 20, stale);
  guest::StoreU32(base, kThread + 28, 0x8209E118);
  guest::StoreU32(base, 0x826A5104, 0);  // the request serials
  for (uint32_t i = 0; i < 960; i++) {    // the request slots, free
    guest::StoreU32(base, 0x826A5170 + 92 * i, 0xFFFFFFFF);
  }
  const uint32_t toc[2] = {allocate(toc_size[0], 4096, 0x8209E2D8),
                           allocate(toc_size[1], 4096, 0x8209E2F4)};
  guest::StoreU32(base, 0x826A5120, toc[0]);
  guest::StoreU32(base, 0x826A5130, extra_size);
  guest::StoreU32(base, 0x826A5128, toc[1]);
  guest::StoreU32(base, 0x826A5134, 0);
  if (extra_size) {
    guest::StoreU32(base, 0x826A5134, allocate(extra_size, 0x801000, 0x8209E32C));
  }
  guest::StoreU32(base, 0x826A511C, 0);
  guest::StoreU32(base, 0x82600AA4, 0xFFFFFFFF);
  guest::StoreU32(base, 0x826A510C, 5242880);  // half the staging ring
  guest::StoreU32(base, 0x826A5110, 0);
  const uint32_t staging = allocate(10485760, 4096, 0x8209E37C);
  guest::StoreU32(base, 0x826A5114, staging);
  guest::StoreU32(base, 0x826A5118, staging + 5242880);
  guest::StoreU32(base, kIdleWanted, 0);
  for (auto [object_address, return_address] :
       {std::pair{kRequestDone, 0x8209E3A8u}, std::pair{kWork, 0x8209E3B8u},
        std::pair{kIdle, 0x8209E3C8u}}) {
    ctx.r3.u64 = object_address;
    call(return_address, [&] { sub_82592670(ctx, base); });
  }
  // BUILD.TOC and BUILD_CRS.TOC, then BUILD.DAT and BUILD_CRS.DAT.
  if (!ReadWhole(base, 0x82001E54, toc[0], toc_size[0])) {
    report_error(0x8209E3F0);
  }
  if (!ReadWhole(base, 0x82001E34, toc[1], toc_size[1])) {
    report_error(0x8209E438);
  }
  for (int crs = 0; crs < 2; crs++) {
    auto handle = OpenImagePath(base, crs ? 0x82001DF0 : 0x82001E14);
    guest::StoreU32(base, kArchiveHandles[crs], handle.value_or(0xFFFFFFFF));
    if (!handle) {
      report_error(crs ? 0x8209E4CC : 0x8209E494);
    }
  }
  // The loader thread's start (a tail call): object->vtable[1](object, the
  // parameters, 1).
  ctx.r3.u64 = object;
  ctx.r4.u64 = kThread;
  ctx.r5.u64 = 1;
  REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, object) + 4));
}

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
