// The archives' start-up (Archive::Init, sub_8209E210) and loader thread
// (sub_8209E118) with the file work done natively:
// an archive request's bytes are copied from BUILD.DAT or BUILD_CRS.DAT
// (src/game_data/) straight into its buffer, with no ReadFileEx, APC or
// alertable wait. The rest is the game's, called as the recompiled loop calls
// it: the request queue (sub_8209DD98, which also finishes requests and runs
// their callbacks on this thread), the loose reads (sub_8209DFA8, on the
// native file class), Ucl::Unpack (sub_82294610) and the loop's locks
// (sub_82592670 waits, sub_825926E8 signals). With --native_archive the next
// request is archive::NextRequest and the locks the archive's host
// semaphores (src/archive/semaphores.h), taken directly. See B4 in
// docs/rexglue-removal.md.

#include "guest.h"
#include "lifted/names.h"
#include "lifted/functions.h"
#include "lifted/entries.h"
#include "hooks/native_entry.h"
#include "guest_call.h"

#include "game_data/game_data.h"
#include "hooks/app_start_up.h"
#include "hooks/archive.h"
#include "schema/archive.h"
#include "schema/c_runtime_core.h"
#include "schema/files_and_saves.h"

#include "base/log.h"

#include <optional>
#include <string>
#include <utility>
#include <vector>
#include "lifted/sdk.h"

using guest_call::InFrame;

namespace {

using G = schema::ArchiveGlobals;

// The loop's objects (0x826C0000 - 21832, - 21912, - 21872).
constexpr uint32_t kWork = G::work_semaphore_address();  // r27: waited at the top
constexpr uint32_t kRequestDone = G::request_done_semaphore_address();  // r28: signaled once a request, then waited
constexpr uint32_t kIdle = G::idle_semaphore_address();  // r29
constexpr uint32_t kFirstCrsId = 23011;
constexpr uint32_t kFrame = 128;

// The read's overlapped (0x826A0000 + 20716), as ReadFileEx's completion left
// it: status +0, bytes +4, offset +8/+12.
schema::CrtOverlapped Overlapped() {
  return schema::CrtOverlapped(G::overlapped_address());
}

// The archives' handles, as Archive::Init keeps them: BUILD.DAT, BUILD_CRS.DAT.
uint32_t ArchiveHandleAddress(bool crs) { return G::archive_handles_address() + (crs ? 4 : 0); }

// An archive's handle (ours, from Archive::Init), or nothing.
std::optional<uint32_t> Archive(bool crs) {
  uint32_t handle = guest::LoadU32(ArchiveHandleAddress(crs));
  if (!game_data::IsHandle(handle)) {
    ITC_LOG_ERROR("archive streaming: {} isn't open", crs ? "BUILD_CRS.DAT" : "BUILD.DAT");
    return std::nullopt;
  }
  return handle;
}

// The bytes of a read of `bytes` at `buffer` that fit: all of them in guest
// memory, where a read past an allocation's end went on into what follows;
// a blob's (E5 step 5) end at its size, its host block's end, so a read past
// it is cut there (logged): the rest isn't stored.
uint32_t BlobRoom(uint32_t buffer, uint32_t bytes, const char* what) {
  uint32_t left = 0;
  if (memory::BlobPointer(buffer, &left) && bytes > left) {
    ITC_LOG_WARN("archive streaming: {} reads {} bytes into a blob at {:08X} with {} left: "
                 "the rest not stored",
                 what, bytes, buffer, left);
    return left;
  }
  return bytes;
}

// The bytes past a blob's end (BlobRoom), read from the file and dropped, so
// the count read is the original's.
uint32_t ReadPastBlob(uint32_t handle, uint64_t offset, uint32_t bytes) {
  if (!bytes) {
    return 0;
  }
  std::vector<uint8_t> scratch(bytes);
  return game_data::ReadAt(handle, offset, scratch.data(), bytes).value_or(0);
}

// An archive request's read (sub_8209DD00's job): +16 the id, +56 the byte
// offset, +60 the sectors (2,048 bytes), +76 the buffer. False if it fails.
bool ReadRequest(uint32_t request) {
  const schema::ArchiveRequestSlot slot(request);
  const uint32_t id = slot.id();
  const uint32_t offset = slot.read_offset();
  const uint32_t bytes = slot.sectors() << 11;
  const uint32_t buffer = slot.data();
  const schema::CrtOverlapped overlapped = Overlapped();
  overlapped.set_offset(offset);
  overlapped.set_offset_high(0);
  auto archive = Archive(id >= kFirstCrsId);
  if (!archive) {
    return false;
  }
  bool ok = true;
  uint32_t total = 0;
  const uint32_t room = BlobRoom(buffer, bytes, "a request");
  guest::ForEachHostPart(buffer, room, [&](uint8_t* host, uint32_t part, uint32_t size) {
    if (!ok) {
      return;
    }
    auto read = game_data::ReadAt(*archive, uint64_t(offset) + part, host, size);
    ok = read.has_value();
    total += read.value_or(0);
  });
  if (ok && room < bytes && total == room) {
    total += ReadPastBlob(*archive, uint64_t(offset) + room, bytes - room);
  }
  if (!ok) {
    ITC_LOG_ERROR("archive streaming: can't read id {} ({} bytes at {:08X})", id, bytes, offset);
    return false;
  }
  // A read past the end gives fewer bytes, as ReadFileEx did.
  overlapped.set_internal(0);
  overlapped.set_internal_high(total);
  return true;
}

std::string GuestString(uint32_t address, uint32_t max = 260) {
  std::string s;
  for (uint32_t i = 0; i < max; i++) {
    char c = char(guest::LoadU8(address + i));
    if (!c) {
      break;
    }
    s += c;
  }
  return s;
}

// Opens the game data file a path in the image names (game:\...).
std::optional<uint32_t> OpenImagePath(uint32_t path) {
  std::string name = GuestString(path);
  auto relative = game_data::RelativePath(name);
  auto handle = relative ? game_data::Open(*relative) : std::nullopt;
  if (!handle) {
    ITC_LOG_ERROR("archive streaming: can't open \"{}\"", name);
  }
  return handle;
}

// Reads a whole file (at most `size` bytes) into guest memory; false if it
// can't be opened or read.
bool ReadWhole(uint32_t path, uint32_t buffer, uint32_t size) {
  auto handle = OpenImagePath(path);
  if (!handle) {
    return false;
  }
  bool ok = true;
  guest::ForEachHostPart(buffer, BlobRoom(buffer, size, "a whole-file read"),
                         [&](uint8_t* host, uint32_t part, uint32_t bytes) {
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
ITC_NATIVE_ENTRY_ALWAYS(8209E210) {
  
  if (hooks::StartUpRecorded(ctx, 0x8209E210)) {
    return;
  }
  // Whether the archive's semaphores are host ones is settled here, before
  // anything waits on them (and before the harnesses switch the flag).
  hooks::ArchiveHostSemaphores();
  const uint32_t object = ctx.r3.u32, extra_size = ctx.r4.u32;
  // A word the recompiled one copies from its own frame without setting it
  // (r1 - 208 + 116): whatever is there.
  const uint32_t stale = guest::LoadU32(ctx.r1.u32 - 92);
  auto call = [&](uint32_t return_address, auto&& function) {
    InFrame(ctx, 208, return_address, [&](uint32_t) { function(); });
  };
  auto allocate = [&](uint32_t size, uint32_t type, uint32_t return_address) {
    ctx.r3.u64 = 0;
    ctx.r4.u64 = size;
    ctx.r5.u64 = type;
    ctx.r6.u64 = 4;
    call(return_address, [&] { lifted::Enter(ctx, game::VirtualAlloc); });
    return ctx.r3.u32;
  };
  auto report_error = [&](uint32_t return_address) {
    call(return_address, [&] { lifted::Enter(ctx, lifted::e8209AC78); });
  };
  // The TOCs' sizes: the entry counts built into the image, plus a header.
  const uint32_t toc_size[2] = {(G::build_toc_count() + 1) << 4,
                                (G::crs_toc_count() + 1) << 4};
  G::set_build_toc_size(toc_size[0]);
  G::set_streaming(object);
  G::set_crs_toc_size(toc_size[1]);
  // The loader thread's parameters (no type of their own: a bytes field).
  constexpr uint32_t kThread = G::loader_thread_address();
  guest::StoreU32(kThread + 8, 0);
  guest::StoreU32(kThread + 12, 0);
  guest::StoreU8(kThread + 16, 0);
  guest::StoreU8(kThread + 17, 1);
  guest::StoreU8(kThread + 18, 0);
  guest::StoreU32(kThread + 20, stale);
  guest::StoreU32(kThread + 28, 0x8209E118);
  G::set_serial(0);  // the request serials
  for (uint32_t i = 0; i < 960; i++) {    // the request slots, free
    schema::ArchiveRequestSlot(G::slots_address() + schema::ArchiveRequestSlot::kSize * i)
        .set_serial(0xFFFFFFFF);
  }
  // The TOCs' buffers: blobs in PC memory (E5 step 5) where the original
  // VirtualAllocs them (returning at 0x8209E2D8 and 0x8209E2F4); read whole
  // below through guest::ForEachHostPart, then read by address.
  const uint32_t toc[2] = {schema::ArchiveTocEntry::New(toc_size[0]).address,
                           schema::ArchiveTocEntry::New(toc_size[1]).address};
  G::set_build_toc(toc[0]);
  G::set_load_buffer_size(extra_size);
  G::set_crs_toc(toc[1]);
  G::set_load_buffer(0);
  if (extra_size) {
    G::set_load_buffer(allocate(extra_size, 0x801000, 0x8209E32C));
  }
  G::set_staging_in_use(0);
  G::set_callback_serial(0xFFFFFFFF);
  G::set_staging_half_size(5242880);  // half the staging ring
  G::set_staging_count(0);
  // The staging ring (two 5 MiB halves the compressed requests are read into,
  // then unpacked from by Ucl::Unpack's guest loads): a blob in PC memory
  // (E5 step 5) where the original VirtualAllocs it (returning at
  // 0x8209E37C). The load buffer above stays in guest memory: the shader
  // pairs' microcode is read into it (the renderer's).
  const uint32_t staging = schema::ArchiveStaging::New(10485760).address;
  G::set_staging_halves(staging);
  G::set_staging_half_1(staging + 5242880);
  G::set_idle_wanted(0);
  for (auto [object_address, return_address] :
       {std::pair{kRequestDone, 0x8209E3A8u}, std::pair{kWork, 0x8209E3B8u},
        std::pair{kIdle, 0x8209E3C8u}}) {
    if (archive::HostSemaphore* s = hooks::ArchiveSemaphore(object_address)) {
      s->Wait();
      continue;
    }
    ctx.r3.u64 = object_address;
    call(return_address, [&] { lifted::Enter(ctx, game::Semaphore::Wait); });
  }
  // BUILD.TOC and BUILD_CRS.TOC, then BUILD.DAT and BUILD_CRS.DAT.
  if (!ReadWhole(0x82001E54, toc[0], toc_size[0])) {
    report_error(0x8209E3F0);
  }
  if (!ReadWhole(0x82001E34, toc[1], toc_size[1])) {
    report_error(0x8209E438);
  }
  for (int crs = 0; crs < 2; crs++) {
    auto handle = OpenImagePath(crs ? 0x82001DF0 : 0x82001E14);
    guest::StoreU32(ArchiveHandleAddress(crs), handle.value_or(0xFFFFFFFF));
    if (!handle) {
      report_error(crs ? 0x8209E4CC : 0x8209E494);
    }
  }
  // The loader thread's start (a tail call): object->vtable[1](object, the
  // parameters, 1).
  ctx.r3.u64 = object;
  ctx.r4.u64 = kThread;
  ctx.r5.u64 = 1;
  lifted::CallFromContext(ctx, schema::ArchiveStreamer(object).vtable().on_init());
}

// The loader thread's loop (no direct callers; never returns). Each time work
// is signaled, takes the requests in turn: state 5 reads from an archive,
// state 7 a loose file, states 1-2 read nothing; a compressed request chained
// to the current one (+84) is unpacked into its destination meanwhile; then
// the request-done signal is waited for (given here for state 5, as the read's
// APC did; by the loop for 1-2 and by the loose read for 7).
ITC_NATIVE_ENTRY_ALWAYS(8209E118) {
  
  auto call = [&](uint32_t return_address, auto&& function) {
    InFrame(ctx, kFrame, return_address, [&](uint32_t) { function(); });
  };
  auto wait = [&](uint32_t object, uint32_t return_address) {
    if (archive::HostSemaphore* s = hooks::ArchiveSemaphore(object)) {
      s->Wait();
      return;
    }
    ctx.r3.u64 = object;
    call(return_address, [&] { lifted::Enter(ctx, game::Semaphore::Wait); });
  };
  auto signal = [&](uint32_t object, uint32_t return_address) {
    if (archive::HostSemaphore* s = hooks::ArchiveSemaphore(object)) {
      s->Signal();
      return;
    }
    ctx.r3.u64 = object;
    call(return_address, [&] { lifted::Enter(ctx, game::Semaphore::Signal); });
  };
  // The next request (r3 the current one or 0; r3 the next).
  auto next = [&](uint32_t current) {
    ctx.r3.u64 = current;
    call(0x8209E1E4, [&] {
      if (hooks::NativeArchiveQueue()) {
        hooks::ArchiveNextRequest(ctx);
      } else {
        lifted::Enter(ctx, game::Archive::NextRequest);
      }
    });
    return ctx.r3.u32;
  };
  for (;;) {
    wait(kWork, 0x8209E148);
    uint32_t request = next(0);
    while (request) {
      const schema::ArchiveRequestSlot slot(request);
      const int32_t state = int32_t(slot.state());
      if (state > 0 && state <= 2) {
        signal(kRequestDone, 0x8209E194);
      } else if (state == 5) {
        if (!ReadRequest(request)) {
          call(0x8209DD6C, [&] { lifted::Enter(ctx, lifted::e8209AC78); });
        }
      } else if (state == 7) {
        ctx.r3.u64 = request;
        call(0x8209E17C, [&] { lifted::Enter(ctx, lifted::entry::e8209DFA8); });
      }
      if (const auto chained = slot.chained(); chained.address) {
        ctx.r3.u64 = chained.unpack_destination();
        ctx.r4.u64 = chained.data();
        ctx.r5.u64 = chained.read_unpacked_size();
        call(0x8209E1B4, [&] { lifted::Enter(ctx, game::Ucl::Unpack); });
      }
      if (int32_t(slot.state()) == 5) {
        // The read is done: what the APC's routine (sub_8209D808) did.
        signal(kRequestDone, 0x8209E1D4);
      }
      wait(kRequestDone, 0x8209E1DC);
      request = next(request);
    }
    if (G::idle_wanted()) {
      G::set_idle_wanted(0);
      signal(kIdle, 0x8209E208);
    }
  }
}
