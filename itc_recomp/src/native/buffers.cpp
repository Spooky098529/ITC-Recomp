// The vertex and index buffers draws use, and where their data comes from
// (milestone 6 step 5: finding where the game creates and fills its
// buffers, to upload them once instead of mirroring guest memory every
// frame; see "Buffers" in docs/native-renderer.md).
//
// With --native_renderer_log_buffers:
// - each buffer object the first time SetStreamSource (command 11) or
//   SetIndices (command 12) binds it: its header (+0 type and flags, +24
//   data address, +28 size), and later changes to that header;
// - the library's memcpy (sub_825A7410) into the physical views is recorded
//   (range, caller, a sequence number), and copies into a known header are
//   logged;
// - each vertex and index range copied into the mirror is checked against
//   those copies: the first time it's seen, whether memcpy wrote all of it
//   (and from where); afterwards, whether memcpy wrote it again.
// Every 5 s, a summary.

#include "buffers.h"

#include "guest.h"
#include "state.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <chrono>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>

REXCVAR_DEFINE_BOOL(native_renderer_log_buffers, false, "ITC",
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

struct RangeState {
  uint64_t checked_sequence;  // the write sequence when last checked
};
std::unordered_map<uint64_t, RangeState> g_ranges;  // physical << 32 | size
struct RangeStats {
  uint64_t first = 0, first_bytes = 0, covered = 0, partly = 0, uncovered = 0,
           uncovered_bytes = 0, rewritten = 0, rewritten_bytes = 0;
  std::map<uint32_t, uint64_t> first_callers, rewrite_callers;  // caller: ranges
} g_range_stats;
uint32_t g_range_lines = 200;

// Any thread (under g_mutex): what memcpy wrote into the physical views, as
// non-overlapping segments (start: end, sequence, caller), and the headers
// of known buffer objects (object: end).
struct Write {
  uint32_t end;
  uint64_t sequence;
  uint32_t caller;
};
std::mutex g_mutex;
std::map<uint32_t, Write> g_writes;
uint64_t g_sequence = 0;
std::map<uint32_t, uint32_t> g_headers;
uint64_t g_header_copies = 0, g_physical_copies = 0, g_physical_copy_bytes = 0;
uint32_t g_header_lines = 100;

// Records [start, end) as written now by caller (g_mutex held).
void RecordWrite(uint32_t start, uint32_t end, uint32_t caller) {
  uint64_t sequence = ++g_sequence;
  // Trim or split segments overlapping the new one.
  auto it = g_writes.lower_bound(start);
  if (it != g_writes.begin()) {
    auto before = std::prev(it);
    if (before->second.end > start) {
      Write tail = before->second;
      before->second.end = start;
      if (tail.end > end) {
        g_writes[end] = tail;
      }
    }
  }
  it = g_writes.lower_bound(start);
  while (it != g_writes.end() && it->first < end) {
    if (it->second.end > end) {
      Write tail = it->second;
      g_writes.erase(it);
      g_writes[end] = tail;
      break;
    }
    it = g_writes.erase(it);
  }
  g_writes[start] = Write{end, sequence, caller};
}

// The bytes of [start, end) memcpy has written, the newest write's sequence
// and caller, and the first segment's caller (g_mutex held).
struct Coverage {
  uint64_t bytes = 0, newest = 0;
  uint32_t newest_caller = 0, first_caller = 0;
};
Coverage CoverageOf(uint32_t start, uint32_t end) {
  Coverage coverage;
  auto it = g_writes.upper_bound(start);
  if (it != g_writes.begin()) {
    --it;
  }
  for (; it != g_writes.end() && it->first < end; ++it) {
    uint32_t lo = std::max(start, it->first), hi = std::min(end, it->second.end);
    if (lo >= hi) {
      continue;
    }
    coverage.bytes += hi - lo;
    if (!coverage.first_caller) {
      coverage.first_caller = it->second.caller;
    }
    if (it->second.sequence > coverage.newest) {
      coverage.newest = it->second.sequence;
      coverage.newest_caller = it->second.caller;
    }
  }
  return coverage;
}

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
      REXLOG_INFO("native renderer: buffers: {:.2f} s new {} buffer {:08X}: +0 {:08X} data "
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
      REXLOG_INFO("native renderer: buffers: {:.2f} s {} buffer {:08X} changed: +0 {:08X} -> "
                  "{:08X}, data {:08X} -> {:08X}, size {} -> {} (after {} binds)",
                  seconds, index ? "index" : "vertex", object, was.type, now.type, was.address,
                  now.address, was.size, now.size, was.binds);
    }
    was = BufferObject{now.type, now.address, now.size, index, was.binds};
  }
}

std::string Callers(const std::map<uint32_t, uint64_t>& callers) {
  std::string text;
  for (auto& [caller, count] : callers) {
    text += fmt::format(" {:08X}={}", caller, count);
  }
  return text.empty() ? " none" : text;
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
  REXLOG_INFO("native renderer: buffers: {} objects known ({} index), {} new and {} headers "
              "changed in 5 s, {} binds; memcpy into headers {} times, into the physical views "
              "{} times ({:.1f} MB)",
              g_objects.size(), index, g_new, g_changed, g_binds, header_copies, physical_copies,
              physical_copy_bytes / 1048576.0);
  RangeStats& r = g_range_stats;
  REXLOG_INFO("native renderer: buffers: mirrored ranges first seen: {} ({:.1f} MB): {} written "
              "wholly by memcpy, {} partly, {} not at all ({:.1f} MB); by first writer:{}",
              r.first, r.first_bytes / 1048576.0, r.covered, r.partly, r.uncovered,
              r.uncovered_bytes / 1048576.0, Callers(r.first_callers));
  REXLOG_INFO("native renderer: buffers: mirrored ranges written again by memcpy after first "
              "use: {} ({:.1f} MB); by writer:{}",
              r.rewritten, r.rewritten_bytes / 1048576.0, Callers(r.rewrite_callers));
  g_new = g_changed = g_binds = 0;
  r = RangeStats{};
}

}  // namespace

bool Buffers_Logging() {
  return REXCVAR_GET(native_renderer_log_buffers);
}

void Buffers_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count) {
  if (!Buffers_Logging()) {
    return;
  }
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

void Buffers_OnMirror(uint32_t physical, uint32_t size, bool index) {
  uint64_t key = (uint64_t(physical) << 32) | size;
  Coverage coverage;
  {
    std::lock_guard lock(g_mutex);
    coverage = CoverageOf(physical, physical + size);
  }
  RangeStats& r = g_range_stats;
  auto [it, added] = g_ranges.try_emplace(key, RangeState{coverage.newest});
  if (added) {
    r.first++;
    r.first_bytes += size;
    if (coverage.bytes >= size) {
      r.covered++;
    } else if (coverage.bytes) {
      r.partly++;
    } else {
      r.uncovered++;
      r.uncovered_bytes += size;
    }
    if (coverage.bytes) {
      r.first_callers[coverage.first_caller]++;
    }
    if (coverage.bytes < size && g_range_lines) {
      g_range_lines--;
      REXLOG_INFO("native renderer: buffers: {} range {:08X} ({} bytes) first used with {} "
                  "bytes written by memcpy",
                  index ? "index" : "vertex", physical, size, coverage.bytes);
    }
    return;
  }
  if (coverage.newest > it->second.checked_sequence) {
    it->second.checked_sequence = coverage.newest;
    r.rewritten++;
    r.rewritten_bytes += size;
    r.rewrite_callers[coverage.newest_caller]++;
    if (g_range_lines) {
      g_range_lines--;
      REXLOG_INFO("native renderer: buffers: {} range {:08X} ({} bytes) written again by "
                  "memcpy from {:08X}",
                  index ? "index" : "vertex", physical, size, coverage.newest_caller);
    }
  }
}

}  // namespace native

// The library's memcpy (r3 destination, r4 source, r5 size): with
// --native_renderer_log_buffers, copies into the physical views are recorded
// and copies into a known buffer header logged.
extern "C" REX_FUNC(sub_825A7410) {
  REX_FUNC_PROLOGUE();
  if (native::Buffers_Logging() && ctx.r5.u32) {
    using namespace native;
    uint32_t destination = ctx.r3.u32, source = ctx.r4.u32, size = ctx.r5.u32;
    uint32_t caller = uint32_t(ctx.lr);
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
        uint32_t physical = GpuPhysicalAddress(destination);
        g_physical_copies++;
        g_physical_copy_bytes += size;
        RecordWrite(physical, physical + size, caller);
      }
    }
    if (header_object) {
      REXLOG_INFO("native renderer: buffers: memcpy {:08X} <- {:08X}, {} bytes, from {:08X}: "
                  "into the header of buffer {:08X}",
                  destination, source, size, caller, header_object);
    }
  }
  __imp__sub_825A7410(ctx, base);
}
