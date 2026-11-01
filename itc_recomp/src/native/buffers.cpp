// The vertex and index buffers draws use, and where their data comes from
// (milestone 6 step 5; see "Buffers" in docs/native-renderer.md).
//
// The game fills its vertex and index buffers with the library's memcpy
// (sub_825A7410), wrapped here. The renderer doesn't learn of writes from
// it: the code that writes a buffer after it's made says so
// (render::BufferWritten; docs/conventional-renderer.md, "Step 2b"). Its
// records of copies into the physical views, which fed the guest memory
// mirrors and then the buffer arena, went in step 2b.
//
// With --native_renderer_log_buffers:
// - each buffer object the first time SetStreamSource (command 11) or
//   SetIndices (command 12) binds it: its header (+0 type and flags, +24
//   data address, +28 size), and later changes to that header;
// - the library's memcpy (sub_825A7410) into the physical views is recorded
//   (range, caller, a sequence number), and copies into a known header are
//   logged.
// Every 5 s, a summary. (Its check of the ranges copied into the mirrors
// against memcpy's copies, and --native_renderer_log_physical_writes, went
// with the mirrors in step 2b.)

#include "buffers.h"

#include "guest.h"
#include "state.h"

#include "base/flags.h"
#include "c_runtime/strings.h"
#include "base/log.h"

#include <chrono>
#include <cstring>
#include <map>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>

ITC_FLAG_BOOL(native_renderer_log_buffers, false, "ITC",
              "Log the vertex and index buffer objects draws bind, and where their data "
              "comes from (a debugging aid for the native renderer)");

namespace native {
namespace {

using Clock = std::chrono::steady_clock;

// Render thread only.
struct BufferObject {
  uint32_t type, address, size;
  bool index;
  uint64_t binds;
};
std::unordered_map<uint32_t, BufferObject> g_objects;  // by object address
Clock::time_point g_start = Clock::now(), g_last_log = Clock::now();
uint64_t g_new = 0, g_changed = 0, g_binds = 0;
uint32_t g_lines = 400;  // new objects and changes logged one by one, up to this

// Any thread (under g_mutex), for the logging: the headers of known buffer
// objects (object: end).
std::mutex g_mutex;
std::map<uint32_t, uint32_t> g_headers;
uint64_t g_header_copies = 0, g_physical_copies = 0, g_physical_copy_bytes = 0;
uint32_t g_header_lines = 100;

void Seen(uint8_t* base, uint32_t object, bool index, uint32_t stream, uint32_t offset,
          uint32_t stride) {
  if (!object) {
    return;
  }
  g_binds++;
  BufferObject now{guest::LoadU32(base, object), guest::LoadU32(base, object + 24),
                   guest::LoadU32(base, object + 28), index, 1};
  double seconds = std::chrono::duration<double>(Clock::now() - g_start).count();
  auto [it, added] = g_objects.try_emplace(object, now);
  if (added) {
    g_new++;
    {
      std::lock_guard lock(g_mutex);
      g_headers[object] = object + 32;
    }
    if (g_lines) {
      g_lines--;
      ITC_LOG_INFO("native renderer: buffers: {:.2f} s new {} buffer {:08X}: +0 {:08X} data "
                   "{:08X} size {}{}",
                   seconds, index ? "index" : "vertex", object, now.type, now.address, now.size,
                   index ? std::string()
                        : fmt::format(" (stream {}, offset {}, stride {})", stream, offset,
                                      stride));
    }
    return;
  }
  BufferObject& was = it->second;
  was.binds++;
  if (was.type != now.type || was.address != now.address || was.size != now.size) {
    g_changed++;
    if (g_lines) {
      g_lines--;
      ITC_LOG_INFO("native renderer: buffers: {:.2f} s {} buffer {:08X} changed: +0 {:08X} -> "
                   "{:08X}, data {:08X} -> {:08X}, size {} -> {} (after {} binds)",
                   seconds, index ? "index" : "vertex", object, was.type, now.type, was.address,
                   now.address, was.size, now.size, was.binds);
    }
    was = BufferObject{now.type, now.address, now.size, index, was.binds};
  }
}

void LogSummary() {
  uint64_t header_copies, physical_copies, physical_copy_bytes;
  {
    std::lock_guard lock(g_mutex);
    header_copies = g_header_copies;
    physical_copies = g_physical_copies;
    physical_copy_bytes = g_physical_copy_bytes;
    g_header_copies = g_physical_copies = g_physical_copy_bytes = 0;
  }
  uint64_t index = 0;
  for (auto& [object, buffer] : g_objects) {
    index += buffer.index;
  }
  ITC_LOG_INFO("native renderer: buffers: {} objects known ({} index), {} new and {} headers "
               "changed in 5 s, {} binds; memcpy into headers {} times, into the physical views "
               "{} times ({:.1f} MB)",
               g_objects.size(), index, g_new, g_changed, g_binds, header_copies, physical_copies,
               physical_copy_bytes / 1048576.0);
  g_new = g_changed = g_binds = 0;
}

}  // namespace

bool Buffers_Logging() {
  return ITC_FLAG(native_renderer_log_buffers);
}

// Kept out of line: inlined, its stack frame (Seen's and the summary's) was
// set up on every replayed command before the flag was checked, ~75 thousand
// cycles a frame with the log off.
[[gnu::noinline]] static void OnCommandLogged(uint8_t* base, uint32_t id, uint32_t payload,
                                             uint32_t count) {
  if (id == 11 || id == 12) {
    for (uint32_t i = 0; i < count; i++) {
      uint32_t item = payload + 16 * i;
      if (id == 11) {  // [stream, buffer, offset, stride]
        Seen(base, guest::LoadU32(base, item + 4), false, guest::LoadU32(base, item),
             guest::LoadU32(base, item + 8), guest::LoadU32(base, item + 12));
      } else {  // [buffer]
        Seen(base, guest::LoadU32(base, item), true, 0, 0, 0);
      }
    }
  }
  if (auto now = Clock::now(); now - g_last_log >= std::chrono::seconds(5)) {
    g_last_log = now;
    LogSummary();
  }
}

void Buffers_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count) {
  if (Buffers_Logging()) {
    OnCommandLogged(base, id, payload, count);
  }
}

}  // namespace native

ITC_FLAG_DECLARE(bool, native_memcpy);

namespace native {

namespace {

// The override's work around a copy (`copy()`): the logging.
template <typename Copy>
void CopyWithRecords(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size,
                     uint32_t caller, Copy copy) {
  if (Buffers_Logging() && size) {
    uint32_t header_object = 0;
    {
      std::lock_guard lock(g_mutex);
      auto header = g_headers.lower_bound(destination > 31 ? destination - 31 : 0);
      if (header != g_headers.end() && header->first < destination + size &&
          header->second > destination) {
        g_header_copies++;
        if (g_header_lines) {
          g_header_lines--;
          header_object = header->first;
        }
      }
      if (destination >= 0xA0000000u) {  // the physical views
        g_physical_copies++;
        g_physical_copy_bytes += size;
      }
    }
    if (header_object) {
      ITC_LOG_INFO("native renderer: buffers: memcpy {:08X} <- {:08X}, {} bytes, from {:08X}: "
                   "into the header of buffer {:08X}",
                   destination, source, size, caller, header_object);
    }
  }
  copy();
}

}  // namespace

bool Buffers_NativeMemcpy() {
  return ITC_FLAG(native_memcpy);
}

bool Buffers_Memcpy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size,
                    uint32_t caller) {
  if (!ITC_FLAG(native_memcpy) ||
      (size != 0 && c_runtime::MayShare(destination, size, source, size))) {
    return false;
  }
  CopyWithRecords(base, destination, source, size, caller,
                  [&] { c_runtime::Memcpy(base, destination, source, size); });
  return true;
}

}  // namespace native

// The library's memcpy (r3 destination, r4 source, r5 size): with
// --native_renderer_log_buffers, copies into a known buffer header logged.
// The copy itself is native (c_runtime::Memcpy, --native_memcpy) where the
// ranges don't share memory; r3 stays the destination either way, and the
// other registers the original changes are read by no caller.
extern "C" REX_FUNC(sub_825A7410) {
  REX_FUNC_PROLOGUE();
  native::CopyWithRecords(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, uint32_t(ctx.lr), [&] {
    if (!ITC_FLAG(native_memcpy) ||
        !c_runtime::Memcpy(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
      __imp__sub_825A7410(ctx, base);
    }
  });
}
