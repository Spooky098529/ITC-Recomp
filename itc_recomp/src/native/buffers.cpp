// The vertex and index buffers draws use, and where their data comes from
// (milestone 6 step 5; see "Buffers" in docs/native-renderer.md).
//
// The game fills its vertex and index buffers with the library's memcpy
// (sub_825A7410), wrapped here: with the native renderer on, each copy into
// the physical views is recorded once it's done, so the mirror only copies
// a range again when memcpy has written it since (Buffers_NewestWrite).
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
//
// With --native_renderer_log_physical_writes, for the hypothesis that what
// the GPU reads only changes on logic passes ("Hypothesis to test: the
// mirrors updated once a tick" in docs/native-renderer.md): each memcpy into
// the physical views, with its thread, pass and caller, checked against the
// vertex and index ranges drawn around it and the textures the cache holds.
// Every 5 s, a summary by caller.

#include "buffers.h"

#include "frame_pacing/game_messages.h"

#include "guest.h"
#include "renderer.h"
#include "state.h"

#include "base/flags.h"
#include "c_runtime/strings.h"
#include "base/log.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstring>
#include <functional>
#include <map>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>

ITC_FLAG_BOOL(native_renderer_log_buffers, false, "ITC",
              "Log the vertex and index buffer objects draws bind, and where their data "
              "comes from (a debugging aid for the native renderer)");
ITC_FLAG_DECLARE(bool, native_renderer_log_texture_writes);
ITC_FLAG_BOOL(native_renderer_log_physical_writes, false, "ITC",
              "Log memcpy's writes into the physical views by thread, pass and caller, and "
              "whether they land in vertex or index data drawn around them (a debugging aid "
              "for the native renderer)");

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

// Any thread (under g_pending_mutex): memcpy's writes into the physical
// views not yet taken by the render thread.
struct PendingWrite {
  uint32_t start, end, caller;
};
std::mutex g_pending_mutex;
std::vector<PendingWrite> g_pending;
std::atomic<bool> g_has_pending{false};

// Render thread: what memcpy wrote into the physical views, as
// non-overlapping segments (start: end, sequence, caller).
struct Write {
  uint32_t end;
  uint64_t sequence;
  uint32_t caller;
};
std::map<uint32_t, Write> g_writes;
uint64_t g_sequence = 0;
std::vector<PendingWrite> g_taken;
// Writes taken since Buffers_TakeNewWrites last ran.
std::vector<std::pair<uint32_t, uint32_t>> g_new_writes;
// With Buffers_CollectMirrorWrites: writes taken since
// Buffers_TakeMirrorWrites last ran.
bool g_collect_mirror_writes = false;
// Whether g_writes is kept (Buffers_KeepSegments).
bool g_keep_segments = true;
std::vector<std::pair<uint32_t, uint32_t>> g_mirror_writes;

// Any thread (under g_mutex), for the logging: the headers of known buffer
// objects (object: end).
std::mutex g_mutex;
std::map<uint32_t, uint32_t> g_headers;
uint64_t g_header_copies = 0, g_physical_copies = 0, g_physical_copy_bytes = 0;
uint32_t g_header_lines = 100;

// Records [start, end) as written now by caller (render thread).
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

// Takes the pending writes into g_writes (render thread).
void TakePending() {
  if (!g_has_pending.load(std::memory_order_acquire)) {
    return;
  }
  {
    std::lock_guard lock(g_pending_mutex);
    g_taken.swap(g_pending);
    g_has_pending.store(false, std::memory_order_relaxed);
  }
  for (const PendingWrite& write : g_taken) {
    if (g_keep_segments) {
      RecordWrite(write.start, write.end, write.caller);
    } else {
      g_sequence++;
    }
    g_new_writes.emplace_back(write.start, write.end);
    if (g_collect_mirror_writes) {
      g_mirror_writes.emplace_back(write.start, write.end);
    }
  }
  g_taken.clear();
}

// The bytes of [start, end) memcpy has written, the newest write's sequence
// and caller, and the first segment's caller (render thread).
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
  ITC_LOG_INFO("native renderer: buffers: {} objects known ({} index), {} new and {} headers "
               "changed in 5 s, {} binds; memcpy into headers {} times, into the physical views "
               "{} times ({:.1f} MB)",
               g_objects.size(), index, g_new, g_changed, g_binds, header_copies, physical_copies,
               physical_copy_bytes / 1048576.0);
  RangeStats& r = g_range_stats;
  ITC_LOG_INFO("native renderer: buffers: mirrored ranges first seen: {} ({:.1f} MB): {} written "
               "wholly by memcpy, {} partly, {} not at all ({:.1f} MB); by first writer:{}",
               r.first, r.first_bytes / 1048576.0, r.covered, r.partly, r.uncovered,
               r.uncovered_bytes / 1048576.0, Callers(r.first_callers));
  ITC_LOG_INFO("native renderer: buffers: mirrored ranges written again by memcpy after first "
               "use: {} ({:.1f} MB); by writer:{}",
               r.rewritten, r.rewritten_bytes / 1048576.0, Callers(r.rewrite_callers));
  g_new = g_changed = g_binds = 0;
  r = RangeStats{};
}


// --native_renderer_log_physical_writes.
//
// Any thread (under g_logged_mutex): each write as made.
struct LoggedWrite {
  uint32_t start, end, caller;
  uint32_t frame;  // the render frame (g_frame_state) when written
  uint64_t pass;
  uint16_t thread;  // 0: the main thread, else a number by first write
  bool render_only, replaying, same;
};
std::mutex g_logged_mutex;
std::vector<LoggedWrite> g_logged;
std::atomic<uint16_t> g_next_thread{1};
// The render frame being recorded << 1 | whether it's being recorded now
// (between Passes::BeginFrame and EndFrame).
std::atomic<uint64_t> g_frame_state{0};

// Render thread: the vertex and index ranges drawn in each of the last few
// frames (by frame number % kRangeFrames).
struct DrawnRange {
  uint32_t start, end;
  bool index;
};
struct FrameRanges {
  uint32_t frame = ~0u;
  std::vector<DrawnRange> ranges;  // sorted by start once the frame is done
  uint32_t longest = 0;
  // 1: overlaps a vertex range, 2: an index range.
  int Overlaps(uint32_t start, uint32_t end) const {
    int kinds = 0;
    auto it = std::lower_bound(ranges.begin(), ranges.end(),
                               start > longest ? start - longest : 0,
                               [](const DrawnRange& r, uint32_t a) { return r.start < a; });
    for (; it != ranges.end() && it->start < end; ++it) {
      if (it->end > start) {
        kinds |= it->index ? 2 : 1;
      }
    }
    return kinds;
  }
};
constexpr uint32_t kRangeFrames = 5;
std::array<FrameRanges, kRangeFrames> g_frame_ranges;
uint32_t g_frame = 0;  // the frame being recorded
std::vector<LoggedWrite> g_logged_taken;

struct WriteStats {
  uint64_t writes[2] = {}, bytes[2] = {};  // [render-only]
  uint64_t same[2] = {};                   // contents the same as before
  uint64_t drawn[2] = {};          // overlapping vertex or index ranges drawn within 2 frames
  uint64_t drawn_changed[2] = {};  // ... and changing their contents
  uint64_t vertex = 0, index = 0;  // overlapping drawn ranges, by kind
  uint64_t drawn_after = 0;        // ... drawn in the frame after the write's
  uint64_t replaying = 0, replaying_changed = 0;  // into the replayed list's ranges, while replayed
  uint64_t textures = 0;           // overlapping a texture the cache holds
  uint64_t first_pass = 0, last_pass = 0;
};
std::map<std::pair<uint32_t, uint16_t>, WriteStats> g_write_stats;  // by (caller, thread)
uint64_t g_unchecked = 0;  // writes too old for the ranges kept
uint32_t g_write_lines = 200;
Clock::time_point g_last_write_log = Clock::now();

std::string ThreadName(uint16_t thread) {
  return thread ? fmt::format("thread {}", thread) : std::string("main");
}

void LogWriteSummary() {
  WriteStats total;
  for (auto& [key, w] : g_write_stats) {
    for (int r = 0; r < 2; r++) {
      total.writes[r] += w.writes[r];
      total.bytes[r] += w.bytes[r];
      total.same[r] += w.same[r];
      total.drawn[r] += w.drawn[r];
      total.drawn_changed[r] += w.drawn_changed[r];
    }
    total.replaying += w.replaying;
    total.replaying_changed += w.replaying_changed;
    total.textures += w.textures;
  }
  ITC_LOG_INFO("native renderer: physical writes: 5 s: logic passes {} ({:.2f} MB, {} same "
               "contents, {} into drawn ranges, {} changing them); render-only passes {} ({:.2f} "
               "MB, {} same, {} into drawn ranges, {} changing them); while the list reading them "
               "was replayed {} ({} changing them); into textures {}; too old to check {}",
               total.writes[0], total.bytes[0] / 1048576.0, total.same[0], total.drawn[0],
               total.drawn_changed[0], total.writes[1], total.bytes[1] / 1048576.0, total.same[1],
               total.drawn[1], total.drawn_changed[1], total.replaying, total.replaying_changed,
               total.textures, g_unchecked);
  for (auto& [key, w] : g_write_stats) {
    ITC_LOG_INFO("native renderer: physical writes: from {:08X} on {}: logic {} ({} bytes, {} "
                 "same, {} drawn, {} changing drawn), render-only {} ({} bytes, {} same, {} "
                 "drawn, {} changing drawn); vertex {} index {} (drawn in the next frame {}); "
                 "while replayed {} ({} "
                 "changing); textures {}; passes {}-{}",
                 key.first, ThreadName(key.second), w.writes[0], w.bytes[0], w.same[0], w.drawn[0],
                 w.drawn_changed[0], w.writes[1], w.bytes[1], w.same[1], w.drawn[1],
                 w.drawn_changed[1], w.vertex, w.index, w.drawn_after, w.replaying, w.replaying_changed,
                 w.textures, w.first_pass, w.last_pass);
  }
  g_write_stats.clear();
  g_unchecked = 0;
}

// Checks the writes made up to two frames ago against the ranges drawn from
// two frames before each to one after, and the replayed frame's ranges for
// writes made while it was recorded.
void CheckLoggedWrites(const std::function<bool(uint32_t, uint32_t)>& is_texture) {
  {
    std::lock_guard lock(g_logged_mutex);
    g_logged_taken.insert(g_logged_taken.end(), g_logged.begin(), g_logged.end());
    g_logged.clear();
  }
  size_t kept = 0;
  for (size_t i = 0; i < g_logged_taken.size(); i++) {
    const LoggedWrite& w = g_logged_taken[i];
    if (w.frame + 2 > g_frame) {
      g_logged_taken[kept++] = w;  // a frame after it isn't done yet
      continue;
    }
    if (w.frame + 2 < g_frame) {
      g_unchecked++;
      continue;
    }
    // Which frames drew it (bit 0: two frames before the write's, bit 3:
    // the one after), and as what.
    int kinds = 0, frames = 0;
    for (uint32_t f = w.frame - 2; f != w.frame + 2; f++) {
      const FrameRanges& ranges = g_frame_ranges[f % kRangeFrames];
      if (ranges.frame == f) {
        if (int k = ranges.Overlaps(w.start, w.end)) {
          kinds |= k;
          frames |= 1 << (f - (w.frame - 2));
        }
      }
    }
    const FrameRanges& replayed = g_frame_ranges[w.frame % kRangeFrames];
    bool into_replayed =
        w.replaying && replayed.frame == w.frame && replayed.Overlaps(w.start, w.end);
    bool texture = is_texture(w.start, w.end);
    auto [it, added] = g_write_stats.try_emplace({w.caller, w.thread});
    WriteStats& s = it->second;
    if (added) {
      s.first_pass = w.pass;
    }
    s.last_pass = w.pass;
    int r = w.render_only;
    s.writes[r]++;
    s.bytes[r] += w.end - w.start;
    s.same[r] += w.same;
    if (kinds) {
      s.drawn[r]++;
      s.drawn_changed[r] += !w.same;
      s.vertex += (kinds & 1) != 0;
      s.index += (kinds & 2) != 0;
      s.drawn_after += (frames & 8) != 0;
    }
    if (into_replayed) {
      s.replaying++;
      s.replaying_changed += !w.same;
    }
    s.textures += texture;
    // What would break the hypothesis: drawn data changed on a render-only
    // pass, or under the replay reading it.
    if (!w.same && ((kinds && w.render_only) || into_replayed) && g_write_lines) {
      g_write_lines--;
      ITC_LOG_INFO("native renderer: physical writes: {:08X}-{:08X} ({} bytes) from {:08X} on {}, "
                   "pass {} ({}), frame {}{}: changes drawn {}{}{}; drawn in frames{}{}{}{}",
                   w.start, w.end, w.end - w.start, w.caller, ThreadName(w.thread), w.pass,
                   w.render_only ? "render-only" : "logic", w.frame,
                   w.replaying ? " while recorded" : "",
                   kinds == 3 ? "vertex and index data" : kinds == 2 ? "index data" : "vertex data",
                   into_replayed ? ", read by the list being replayed" : "",
                   texture ? ", and a texture" : "", frames & 1 ? " -2" : "",
                   frames & 2 ? " -1" : "", frames & 4 ? " 0" : "", frames & 8 ? " +1" : "");
    }
  }
  g_logged_taken.resize(kept);
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

bool Buffers_LogPhysicalWrites() {
  return ITC_FLAG(native_renderer_log_physical_writes);
}

void Buffers_OnFrameStart(const std::function<bool(uint32_t, uint32_t)>& is_texture) {
  // The frame just recorded is done: its ranges are sorted for lookups.
  FrameRanges& done = g_frame_ranges[g_frame % kRangeFrames];
  if (done.frame == g_frame) {
    std::sort(done.ranges.begin(), done.ranges.end(),
              [](const DrawnRange& a, const DrawnRange& b) { return a.start < b.start; });
  }
  g_frame++;
  FrameRanges& next = g_frame_ranges[g_frame % kRangeFrames];
  next.frame = g_frame;
  next.ranges.clear();
  next.longest = 0;
  g_frame_state.store((uint64_t(g_frame) << 1) | 1, std::memory_order_relaxed);
  CheckLoggedWrites(is_texture);
  if (auto now = Clock::now(); now - g_last_write_log >= std::chrono::seconds(5)) {
    g_last_write_log = now;
    LogWriteSummary();
  }
}

void Buffers_OnFrameEnd() {
  g_frame_state.store(uint64_t(g_frame) << 1, std::memory_order_relaxed);
}

void Buffers_OnDrawnRange(uint32_t physical, uint32_t size, bool index) {
  FrameRanges& ranges = g_frame_ranges[g_frame % kRangeFrames];
  ranges.ranges.push_back({physical, physical + size, index});
  ranges.longest = std::max(ranges.longest, size);
}

uint64_t Buffers_NewestWrite(uint32_t physical, uint32_t size) {
  TakePending();
  // Most ranges lie within one segment (a loader's copy): look that up
  // first.
  auto it = g_writes.upper_bound(physical);
  if (it != g_writes.begin()) {
    auto segment = std::prev(it);
    if (segment->second.end >= physical + size) {
      return segment->second.sequence;
    }
  }
  return CoverageOf(physical, physical + size).newest;
}

Buffers_Coverage Buffers_CoverageOf(uint32_t physical, uint32_t size) {
  TakePending();
  Coverage c = CoverageOf(physical, physical + size);
  return {c.bytes, c.newest, c.newest_caller, c.first_caller};
}

void Buffers_TakeNewWrites(std::vector<std::pair<uint32_t, uint32_t>>& out) {
  TakePending();
  out.insert(out.end(), g_new_writes.begin(), g_new_writes.end());
  g_new_writes.clear();
}

void Buffers_KeepSegments(bool keep) {
  // The texture cache's write log asks of them too.
  g_keep_segments = keep || ITC_FLAG(native_renderer_log_texture_writes);
}

void Buffers_CollectMirrorWrites() {
  if (g_collect_mirror_writes) {
    return;
  }
  g_collect_mirror_writes = true;
  // What was taken before (none if called before the first frame).
  for (const auto& [start, write] : g_writes) {
    g_mirror_writes.emplace_back(start, write.end);
  }
}

void Buffers_TakeMirrorWrites(std::vector<std::pair<uint32_t, uint32_t>>& out) {
  TakePending();
  out.insert(out.end(), g_mirror_writes.begin(), g_mirror_writes.end());
  g_mirror_writes.clear();
}

uint64_t Buffers_Sequence() {
  return g_sequence;
}

void Buffers_OnMirror(uint32_t physical, uint32_t size, bool index) {
  uint64_t key = (uint64_t(physical) << 32) | size;
  TakePending();
  Coverage coverage = CoverageOf(physical, physical + size);
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
      ITC_LOG_INFO("native renderer: buffers: {} range {:08X} ({} bytes) first used with {} "
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
      ITC_LOG_INFO("native renderer: buffers: {} range {:08X} ({} bytes) written again by "
                   "memcpy from {:08X}",
                   index ? "index" : "vertex", physical, size, coverage.newest_caller);
    }
  }
}

}  // namespace native

ITC_FLAG_DECLARE(bool, native_memcpy);

namespace native {

namespace {

// The override's work around a copy (`copy()`): the logging, and copies
// into the physical views recorded once done.
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
  bool log_write = size && destination >= 0xA0000000u && Buffers_LogPhysicalWrites();
  bool same = log_write && !std::memcmp(base + destination, base + source, size);
  copy();
  if (log_write) {
    LoggedWrite write;
    write.start = GpuPhysicalAddress(destination);
    write.end = write.start + size;
    write.caller = caller;
    uint64_t frame_state = g_frame_state.load(std::memory_order_relaxed);
    write.frame = uint32_t(frame_state >> 1);
    write.replaying = frame_state & 1;
    bool main_thread;
    frame_pacing::GameMessages_PassFromAnyThread(write.pass, write.render_only, main_thread);
    static thread_local uint16_t thread = 0;
    if (!main_thread && !thread) {
      thread = g_next_thread.fetch_add(1, std::memory_order_relaxed);
    }
    write.thread = main_thread ? 0 : thread;
    write.same = same;
    std::lock_guard lock(g_logged_mutex);
    g_logged.push_back(write);
  }
  // Recorded after the copy, so a range read meanwhile is read again.
  if (size && destination >= 0xA0000000u) {
    uint32_t physical = GpuPhysicalAddress(destination);
    std::lock_guard lock(g_pending_mutex);
    g_pending.push_back({physical, physical + size, caller});
    g_has_pending.store(true, std::memory_order_release);
  }
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

// The library's memcpy (r3 destination, r4 source, r5 size): copies into
// the physical views are recorded once done (see Buffers_NewestWrite); with
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
