// TEMPORARY (rexglue-removal B4, docs/rexglue-removal.md): counts the game's
// functions that call file and save imports (the Win32-style file calls, the
// game's file layer, the C runtime's low-level I/O, the save slots and their
// XContent wrappers; docs/functions/files-and-saves.md), to find which run,
// how often, from where and what the SDK answers, before any is made native
// or dropped. Removed when the survey is done (a kept wrapper would block a
// real override).
//
// --log_files_and_saves logs each function's first calls and every power of
// two (caller, r3-r10 in, r3 out, and what the function answers through
// memory where that's known), and every 10 s the counts of those called.
// CreateFile and the movie player's open already have overrides
// (create_file.cpp): run with --log_file_opens for their paths. The save slot
// steps made native (hooks/saves.cpp), XContentClose and the file class made
// native (hooks/file_class.cpp) and Archive::Init (hooks/archive_streaming.cpp)
// are no longer wrapped.

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <atomic>
#include <bit>
#include <chrono>
#include <mutex>
#include <string>

REXCVAR_DEFINE_BOOL(log_files_and_saves, false, "ITC",
                    "Log calls to the game's functions that call file and save kernel imports "
                    "(temporary, for rexglue-removal B4)");

namespace {

struct In {
  uint32_t lr, r1, r[8];  // r3..r10
};

using Extra = std::string (*)(uint8_t* base, const In& in, uint32_t result);

std::string None(uint8_t*, const In&, uint32_t) {
  return {};
}

std::string String(uint8_t* base, uint32_t address, uint32_t max = 260) {
  if (!address) {
    return "(null)";
  }
  std::string s;
  for (uint32_t i = 0; i < max; i++) {
    char c = char(guest::LoadU8(base, address + i));
    if (!c) {
      break;
    }
    s += c;
  }
  return "\"" + s + "\"";
}

std::string WideString(uint8_t* base, uint32_t address, uint32_t max = 128) {
  if (!address) {
    return "(null)";
  }
  std::string s;
  for (uint32_t i = 0; i < max; i++) {
    uint16_t c = guest::LoadU16(base, address + 2 * i);
    if (!c) {
      break;
    }
    s += c < 0x80 ? char(c) : '?';
  }
  return "\"" + s + "\"";
}

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return uint64_t(guest::LoadU32(base, address)) << 32 | guest::LoadU32(base, address + 4);
}

// An optional 32- or 64-bit out value.
std::string Out32(uint8_t* base, const char* name, uint32_t address) {
  return address ? fmt::format("; {} {:08X}", name, guest::LoadU32(base, address)) : "";
}

std::string Out64(uint8_t* base, const char* name, uint32_t address) {
  return address ? fmt::format("; {} {}", name, LoadU64(base, address)) : "";
}

// An overlapped (28 bytes): status +0, information +4, offset +8/+12, event +16.
std::string Overlapped(uint8_t* base, uint32_t address) {
  if (!address) {
    return "";
  }
  return fmt::format("; overlapped status {} information {:08X} event {:08X}",
                     guest::LoadU32(base, address), guest::LoadU32(base, address + 4),
                     guest::LoadU32(base, address + 16));
}

// An XCONTENT_DATA (308 bytes): device +0, content type +4, display name +8
// (UTF-16, 128), file name +264 (42).
std::string ContentData(uint8_t* base, uint32_t address) {
  if (!address) {
    return "; content (null)";
  }
  return fmt::format("; content device {:08X} type {} display {} file {}",
                     guest::LoadU32(base, address), guest::LoadU32(base, address + 4),
                     WideString(base, address + 8), String(base, address + 264, 42));
}

// r3 = a path.
std::string PathArg(uint8_t* base, const In& in, uint32_t) {
  return "; path " + String(base, in.r[0]);
}

// GetDiskFreeSpaceEx (path, free for the caller, total, free).
std::string DiskFree(uint8_t* base, const In& in, uint32_t result) {
  return PathArg(base, in, result) + Out64(base, "caller free", in.r[1]) +
         Out64(base, "total", in.r[2]) + Out64(base, "free", in.r[3]);
}

// ReadFile and WriteFile (handle, buffer, size, done, overlapped).
std::string ReadWrite(uint8_t* base, const In& in, uint32_t) {
  return Out32(base, "done", in.r[3]) + Overlapped(base, in.r[4]);
}

// ReadFileEx (handle, buffer, size, overlapped, routine).
std::string ReadEx(uint8_t* base, const In& in, uint32_t) {
  return Overlapped(base, in.r[3]);
}

// ReadFileEx's APC (routine, IO_STATUS_BLOCK).
std::string Apc(uint8_t* base, const In& in, uint32_t) {
  return fmt::format("; block status {:08X} information {:08X}", guest::LoadU32(base, in.r[1]),
                     guest::LoadU32(base, in.r[1] + 4));
}

// GetFileSize (handle, high).
std::string FileSize(uint8_t* base, const In& in, uint32_t) {
  return Out32(base, "high", in.r[1]);
}

// GetFileSizeEx (handle, size).
std::string FileSizeEx(uint8_t* base, const In& in, uint32_t) {
  return Out64(base, "size", in.r[1]);
}

// SetFilePointer (handle, low, high, method).
std::string FilePointer(uint8_t* base, const In& in, uint32_t) {
  return Out32(base, "high", in.r[2]);
}

// GetFileTime (handle, creation, last access, last write).
std::string FileTime(uint8_t* base, const In& in, uint32_t) {
  return Out64(base, "creation", in.r[1]) + Out64(base, "access", in.r[2]) +
         Out64(base, "write", in.r[3]);
}

// XShowDeviceSelectorUI (user, content type, flags, bytes, device, overlapped).
std::string DeviceSelector(uint8_t* base, const In& in, uint32_t) {
  return Out32(base, "device", in.r[4]) + Overlapped(base, in.r[5]);
}

// XContentCreate (user, root name, data, flags, disposition, license, overlapped).
std::string ContentCreate(uint8_t* base, const In& in, uint32_t) {
  return "; root " + String(base, in.r[1], 64) + ContentData(base, in.r[2]) +
         Out32(base, "disposition", in.r[4]) + Out32(base, "license", in.r[5]) +
         Overlapped(base, in.r[6]);
}

// XContentDelete (user, data, overlapped).
std::string ContentDelete(uint8_t* base, const In& in, uint32_t) {
  return ContentData(base, in.r[1]) + Overlapped(base, in.r[2]);
}


// XContentSetThumbnail (user, data, image, size, overlapped).
std::string Thumbnail(uint8_t* base, const In& in, uint32_t) {
  return ContentData(base, in.r[1]) + Overlapped(base, in.r[4]);
}

// XContentGetCreator (user, data, is creator, XUID, overlapped).
std::string Creator(uint8_t* base, const In& in, uint32_t) {
  return ContentData(base, in.r[1]) + Out32(base, "is creator", in.r[2]) +
         (in.r[3] ? fmt::format("; xuid {:016X}", LoadU64(base, in.r[3])) : "");
}

// XContentCreateEnumerator (user, device, type, flags, count, size, handle).
std::string CreateEnumerator(uint8_t* base, const In& in, uint32_t) {
  return Out32(base, "size", in.r[5]) + Out32(base, "handle", in.r[6]);
}

// XEnumerate (handle, buffer, size, items, overlapped): the items found, as
// the SDK answers at once.
std::string Enumerate(uint8_t* base, const In& in, uint32_t) {
  uint32_t items = in.r[3] ? guest::LoadU32(base, in.r[3]) : guest::LoadU32(base, in.r[4] + 4);
  std::string s = Out32(base, "items", in.r[3]) + Overlapped(base, in.r[4]);
  for (uint32_t i = 0; i < std::min(items, 8u) && 308 * (i + 1) <= in.r[2]; i++) {
    s += ContentData(base, in.r[1] + 308 * i);
  }
  return s;
}

// XContentGetDeviceData (device, data): type +4, bytes +8, free bytes +16.
std::string DeviceData(uint8_t* base, const In& in, uint32_t) {
  return fmt::format("; type {} bytes {} free {}", guest::LoadU32(base, in.r[1] + 4),
                     LoadU64(base, in.r[1] + 8), LoadU64(base, in.r[1] + 16));
}

// XGetOverlappedResult (overlapped, result, wait).
std::string OverlappedResult(uint8_t* base, const In& in, uint32_t) {
  return Out32(base, "result", in.r[1]) + Overlapped(base, in.r[0]);
}

// A save slot step (r3 owner, r4 slot): the slot's device and names.
std::string Slot(uint8_t* base, const In& in, uint32_t) {
  uint32_t slot = in.r[1];
  return fmt::format("; slot device {:08X} display {} file {}", guest::LoadU32(base, slot + 36),
                     WideString(base, slot + 504), String(base, slot + 760, 42));
}

// The movie player's file data source (r3): after the call, its position
// +88, end flag +48, start +56 and size +64, block size +104 and count +108,
// flags +120, handle +44; and r6's bytes read for the reads.
std::string Stream(uint8_t* base, const In& in, uint32_t) {
  uint32_t s = in.r[0];
  return fmt::format("; position {} end {} start {} size {} block {} x {} flags {:X} handle {:08X}",
                     LoadU64(base, s + 88), guest::LoadU32(base, s + 48), LoadU64(base, s + 56),
                     LoadU64(base, s + 64), guest::LoadU32(base, s + 104),
                     guest::LoadU32(base, s + 108), guest::LoadU32(base, s + 120),
                     guest::LoadU32(base, s + 44));
}

std::string StreamRead(uint8_t* base, const In& in, uint32_t result) {
  return Out32(base, "read", in.r[3]) + Stream(base, in, result);
}

// GetOverlappedResult (handle, overlapped, bytes, wait).
std::string WaitResult(uint8_t* base, const In& in, uint32_t) {
  return Out32(base, "bytes", in.r[2]) + Overlapped(base, in.r[1]);
}

struct Function {
  uint32_t address;
  const char* name;
  Extra extra;
  std::atomic<uint64_t> calls{0};
};

// Names are from docs/functions/files-and-saves.md ("probably" there).
#define FILES_AND_SAVES(X)                                                  \
  X(8209AC78, "file layer error report?", None)                             \
  X(820A55A8, "XContentGetCreator caller", None)                            \
  X(820A5BE8, "SaveSlot::SaveStep", Slot)                                   \
  X(820A5E68, "SaveSlot::LoadStep", Slot)                                   \
  X(822952E0, "XShowDeviceSelectorUI", DeviceSelector)                      \
  X(822953E8, "ReadFileEx's APC", Apc)                                      \
  X(82295450, "ReadFileEx", ReadEx)                                         \
  X(82295BC8, "XContentCreate", ContentCreate)                              \
  X(82295BE8, "XContentDelete", ContentDelete)                              \
  X(82295BF8, "XContentSetThumbnail", Thumbnail)                            \
  X(82295C00, "XContentGetCreator", Creator)                                \
  X(82295C08, "XContentCreateEnumerator", CreateEnumerator)                 \
  X(82295C10, "XContentGetDeviceData", DeviceData)                          \
  X(82295C18, "XContentCalculateSize?", None)                               \
  X(82295C98, "XGetOverlappedResult", OverlappedResult)                     \
  X(82295D30, "XEnumerate", Enumerate)                                      \
  X(82295D48, "wcscpy?", None)                                              \
  X(82295D50, "WriteFile", ReadWrite)                                       \
  X(82295F20, "GetOverlappedResult", WaitResult)                            \
  X(82295FC8, "ReadFile", ReadWrite)                                        \
  X(822965E0, "GetFileSize", FileSize)                                      \
  X(822972E0, "GetFileSizeEx", FileSizeEx)                                  \
  X(822B4760, "debug capture free space check", None)                       \
  X(822B58F0, "debug capture mounting", None)                               \
  X(822BC588, "GetFileAttributes caller 2", None)                           \
  X(822E1838, "whole file load", None)                                      \
  X(8235E878, "GetFileTime caller", None)                                   \
  X(82487D58, "movie source open (wide path)", Stream)                      \
  X(82487DD8, "movie source open (handle)", Stream)                         \
  X(82487E68, "movie source open (parameters)", Stream)                     \
  X(824881D0, "movie source prime", Stream)                                 \
  X(824883C8, "movie source cancel", Stream)                                \
  X(82488508, "movie source skip", Stream)                                  \
  X(82488548, "movie source seek", Stream)                                  \
  X(8258C608, "File::OpenWrite", PathArg)                                   \
  X(8258C898, "File::Write", None)                                          \
  X(82593B10, "open and size", None)                                        \
  X(825B0F48, "_write", None)                                               \
  X(825B4828, "_commit", None)                                              \
  X(825B6500, "_lseek", None)                                               \
  X(825B81A0, "_read", None)                                                \
  X(825B9C20, "GetDiskFreeSpaceEx", DiskFree)                               \
  X(825B9D30, "SetEndOfFile", None)                                         \
  X(825B9DD8, "SetFilePointer", FilePointer)                                \
  X(825B9F20, "DeleteFile", PathArg)                                        \
  X(825B9FD8, "GetFileAttributes", PathArg)                                 \
  X(825BA040, "GetFileTime", FileTime)                                      \
  X(825BA510, "FlushFileBuffers", None)

#define ENTRY(address, name, extra) {0x##address, #address " " name, extra},
Function g_functions[] = {FILES_AND_SAVES(ENTRY)};
#undef ENTRY

#define INDEX(address, name, extra) k##address,
enum Index { FILES_AND_SAVES(INDEX) };
#undef INDEX

std::mutex g_summary_mutex;
std::chrono::steady_clock::time_point g_last_summary = std::chrono::steady_clock::now();

void Summary() {
  auto now = std::chrono::steady_clock::now();
  {
    std::lock_guard lock(g_summary_mutex);
    if (now - g_last_summary < std::chrono::seconds(10)) {
      return;
    }
    g_last_summary = now;
  }
  std::string counts;
  for (auto& f : g_functions) {
    if (uint64_t n = f.calls.load()) {
      counts += fmt::format(" [{:08X}: {}]", f.address, n);
    }
  }
  REXLOG_INFO("files and saves: counts:{}", counts);
}

// Counts the call; true when it's one to log (the first 3 and powers of 2).
bool Count(Index i) {
  if (!REXCVAR_GET(log_files_and_saves)) {
    return false;
  }
  uint64_t n = ++g_functions[i].calls;
  Summary();
  return n <= 3 || std::has_single_bit(n);
}

In Capture(PPCContext& ctx) {
  return {uint32_t(ctx.lr), ctx.r1.u32,
          {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
           ctx.r10.u32}};
}

std::string Registers(const In& in) {
  return fmt::format("r3 {:08X} r4 {:08X} r5 {:08X} r6 {:08X} r7 {:08X} r8 {:08X} r9 {:08X} "
                     "r10 {:08X}",
                     in.r[0], in.r[1], in.r[2], in.r[3], in.r[4], in.r[5], in.r[6], in.r[7]);
}

void Log(Index i, uint8_t* base, const In& in, uint32_t result) {
  REXLOG_INFO("files and saves: {} call {} from {:08X}: {} -> {:08X}{}", g_functions[i].name,
              g_functions[i].calls.load(), in.lr, Registers(in), result,
              g_functions[i].extra(base, in, result));
}

}  // namespace

#define WRAPPER(address, name, extra)        \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    bool log = Count(k##address);            \
    const In in = Capture(ctx);              \
    __imp__sub_##address(ctx, base);         \
    if (log) {                               \
      Log(k##address, base, in, ctx.r3.u32); \
    }                                        \
  }

FILES_AND_SAVES(WRAPPER)
#undef WRAPPER

