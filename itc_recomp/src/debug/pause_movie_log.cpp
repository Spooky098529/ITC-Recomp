// Logs what changes the pause menu's movie and its sub-menus' arrow
// (--log_pause_movie), to find what keeps the arrow sliding past the
// selected item once the movie is advanced once a tick
// (docs/pause-menu-fix.md, "The sub-menus' arrow").
//
// Every UI movie update (sub_8211D2F8) is counted once a second by movie,
// mode, caller (lr: the caller of sub_8211E1B8, which jumps to it) and pass
// type. While paused, around each update of each movie: the movie object (0xA000 bytes, what the UI movie
// replay keeps) and the objects up to two pointers away from it (0x200 bytes
// each) are compared before and after the update ("in update") and against
// the last update's end ("between": written by other code since). Each
// changed word is logged with the pass type, unless it changed in 60 or more
// updates in the last second (drawing bookkeeping); a summary of the busy
// words is logged once a second. The sub-menus' calls that set the movie's
// nodes (sub_8211C7E8, sub_821BE5A8, sub_8220D040) and their state switch
// (sub_8220BA68) are logged with their arguments.

#include "debug/pause_movie_log.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <bit>
#include <chrono>
#include <cstring>
#include <map>
#include <tuple>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(log_pause_movie, false, "ITC",
                    "While paused, log the words the pause menu's movie update changes (and "
                    "what changes them between updates), and the sub-menus' calls setting its "
                    "nodes, by pass type");

namespace debug {

struct Range {
  uint32_t start;
  uint32_t size;
};
std::vector<Range> ReadableGuestRanges(uint8_t* base);  // frame_state_diff.cpp

namespace {

constexpr uint32_t kMovieBytes = 0xA000;
constexpr uint32_t kObjectBytes = 0x200;
constexpr size_t kMaxObjects = 8192;
constexpr int kBusyChanges = 60;     // changes a second that mark a word as bookkeeping
constexpr int kMaxLines = 800;       // change lines per 2 s
constexpr int kMaxCallLines = 400;   // call lines per 2 s

const char* PassType() {
  return frame_pacing::GameMessages_IsRenderOnlyPass() ? "render-only" : "logic";
}

uint64_t Ticks() {
  uint64_t logic = 0, render_only = 0;
  frame_pacing::GameMessages_PassCounts(logic, render_only);
  return logic;
}

bool TakeLine(int& lines, std::chrono::steady_clock::time_point& last, int max) {
  auto now = std::chrono::steady_clock::now();
  if (now - last >= std::chrono::seconds(2)) {
    last = now;
    lines = 0;
  }
  return lines++ < max;
}

class PauseMovieLog {
 public:
  void Before(uint8_t* base, uint32_t movie, uint32_t r4, bool draw_only) {
    active_ = REXCVAR_GET(log_pause_movie) && frame_pacing::GameMessages_Paused();
    if (!active_) {
      last_after_.clear();
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (movie != movie_ || now - last_rebuild_ >= std::chrono::seconds(1)) {
      Rebuild(base, movie);
      last_rebuild_ = now;
      last_after_.clear();
    }
    if (now - last_summary_ >= std::chrono::seconds(1)) {
      Summarize();
      last_summary_ = now;
    }
    r4_ = r4;
    draw_only_ = draw_only;
    Snapshot(base, before_);
    if (last_after_.size() == before_.size()) {
      Compare(last_after_, before_, "between");
    }
  }

  void After(uint8_t* base) {
    if (!active_) {
      return;
    }
    Snapshot(base, after_);
    Compare(before_, after_, "in update");
    last_after_.swap(after_);
  }

 private:
  struct Object {
    uint32_t address;
    uint32_t size;
    uint32_t parent;  // 0 for the movie itself
  };
  struct Busy {
    int changes = 0;
    int logic = 0, render_only = 0, between = 0;
    uint32_t first = 0, last = 0;
  };

  bool Readable(uint32_t address, uint32_t size) const {
    auto it = std::upper_bound(ranges_.begin(), ranges_.end(), address,
                               [](uint32_t a, const Range& r) { return a < r.start; });
    if (it == ranges_.begin()) {
      return false;
    }
    --it;
    return uint64_t(address) + size <= uint64_t(it->start) + it->size;
  }

  void Rebuild(uint8_t* base, uint32_t movie) {
    movie_ = movie;
    ranges_ = ReadableGuestRanges(base);
    std::sort(ranges_.begin(), ranges_.end(),
              [](const Range& a, const Range& b) { return a.start < b.start; });
    // Adjacent mappings are one range here.
    std::vector<Range> merged;
    for (const Range& r : ranges_) {
      if (!merged.empty() && uint64_t(merged.back().start) + merged.back().size == r.start) {
        merged.back().size += r.size;
      } else {
        merged.push_back(r);
      }
    }
    ranges_.swap(merged);
    objects_.clear();
    if (!Readable(movie, kMovieBytes)) {
      REXLOG_INFO("pause movie: tick {} movie {:08X} isn't readable ({} ranges)", Ticks(), movie,
                  ranges_.size());
      return;
    }
    objects_.push_back({movie, kMovieBytes, 0});
    std::unordered_map<uint32_t, bool> seen{{movie, true}};
    // Two levels of pointers: heap words (0x40000000-0x7FFFFFFF) that point
    // at readable memory outside the objects already taken.
    for (size_t level = 0, from = 0; level < 2; level++) {
      size_t to = objects_.size();
      for (size_t i = from; i < to && objects_.size() < kMaxObjects; i++) {
        const Object o = objects_[i];
        for (uint32_t off = 0; off + 4 <= o.size && objects_.size() < kMaxObjects; off += 4) {
          uint32_t v = guest::LoadU32(base, o.address + off);
          if (v < 0x40000000 || v >= 0x80000000 || (v & 3) != 0) {
            continue;
          }
          if (v >= movie && v < movie + kMovieBytes) {
            continue;
          }
          if (seen.count(v) || !Readable(v, kObjectBytes)) {
            continue;
          }
          seen[v] = true;
          objects_.push_back({v, kObjectBytes, o.address});
        }
      }
      from = to;
    }
    size_t bytes = 0;
    for (const Object& o : objects_) {
      bytes += o.size;
    }
    REXLOG_INFO("pause movie: tick {} movie {:08X}: watching {} objects, {} bytes", Ticks(),
                movie, objects_.size(), bytes);
  }

  void Snapshot(uint8_t* base, std::vector<uint8_t>& out) const {
    out.clear();
    for (const Object& o : objects_) {
      out.insert(out.end(), base + o.address, base + o.address + o.size);
    }
  }

  static uint32_t WordAt(const std::vector<uint8_t>& bytes, size_t at) {
    uint32_t v;
    std::memcpy(&v, bytes.data() + at, 4);
    return __builtin_bswap32(v);
  }

  void Compare(const std::vector<uint8_t>& from, const std::vector<uint8_t>& to,
               const char* where) {
    bool between = where[0] == 'b';
    bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
    size_t at = 0;
    for (const Object& o : objects_) {
      if (std::memcmp(from.data() + at, to.data() + at, o.size) != 0) {
        for (uint32_t off = 0; off < o.size; off += 4) {
          uint32_t a = WordAt(from, at + off), b = WordAt(to, at + off);
          if (a == b) {
            continue;
          }
          uint32_t address = o.address + off;
          Busy& busy = busy_[address];
          if (busy.changes == 0) {
            busy.first = a;
          }
          busy.last = b;
          (between ? busy.between : render_only ? busy.render_only : busy.logic)++;
          bool quiet = busy_last_second_.count(address) == 0;
          if (busy.changes++ < kBusyChanges && quiet &&
              TakeLine(lines_, lines_since_, kMaxLines)) {
            REXLOG_INFO(
                "pause movie change: movie {:08X} tick {} pass {} {} r4 {}{} {} {:08X} (object "
                "{:08X} +{:X}, parent {:08X}): {:08X} -> {:08X} ({:g} -> {:g})",
                movie_, Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), r4_,
                draw_only_ ? " made draw-only" : "", where, address, o.address, off, o.parent,
                a, b, std::bit_cast<float>(a), std::bit_cast<float>(b));
          }
        }
      }
      at += o.size;
    }
  }

  // Words that changed in kBusyChanges or more updates in the last second
  // are logged here only, and left out of the change lines in the next.
  void Summarize() {
    busy_last_second_.clear();
    int lines = 0;
    for (const auto& [address, busy] : busy_) {
      if (busy.changes < kBusyChanges) {
        continue;
      }
      busy_last_second_[address] = true;
      if (lines++ < 40) {
        REXLOG_INFO(
            "pause movie busy: movie {:08X} {:08X}: {} changes (in update {} logic, {} render-only; "
            "between {}), {:08X} -> {:08X}",
            movie_, address, busy.changes, busy.logic, busy.render_only, busy.between, busy.first,
            busy.last);
      }
    }
    if (lines > 40) {
      REXLOG_INFO("pause movie busy: {} more", lines - 40);
    }
    busy_.clear();
  }

  bool active_ = false;
  uint32_t movie_ = 0;
  uint32_t r4_ = 0;
  bool draw_only_ = false;
  std::vector<Range> ranges_;
  std::vector<Object> objects_;
  std::vector<uint8_t> before_, after_, last_after_;
  std::map<uint32_t, Busy> busy_;
  std::unordered_map<uint32_t, bool> busy_last_second_;
  std::chrono::steady_clock::time_point last_rebuild_{}, last_summary_{}, lines_since_{};
  int lines_ = 0;
};

std::unordered_map<uint32_t, PauseMovieLog> g_logs;  // by movie

// Every UI movie update, counted once a second by movie, mode, caller, pass
// type, pause and whether it was made draw-only.
using CallKey = std::tuple<uint32_t, uint32_t, uint32_t, bool, bool, bool>;
std::map<CallKey, int> g_updates;
std::chrono::steady_clock::time_point g_updates_since{};

void CountUpdate(uint32_t movie, uint32_t r4, uint32_t caller, bool draw_only) {
  bool paused = frame_pacing::GameMessages_Paused();
  g_updates[{movie, r4, caller, paused, frame_pacing::GameMessages_IsRenderOnlyPass(),
             draw_only}]++;
  auto now = std::chrono::steady_clock::now();
  if (now - g_updates_since < std::chrono::seconds(1)) {
    return;
  }
  g_updates_since = now;
  for (const auto& [key, n] : g_updates) {
    const auto& [m, mode, from, was_paused, render_only, made_draw_only] = key;
    REXLOG_INFO("pause movie updates: tick {} movie {:08X} r4 {} from {:08X} {} {}{}: {}",
                Ticks(), m, mode, from, was_paused ? "paused" : "running",
                render_only ? "render-only" : "logic", made_draw_only ? " made draw-only" : "",
                n);
  }
  g_updates.clear();
}

int g_call_lines = 0;
std::chrono::steady_clock::time_point g_call_lines_since{};

void LogCall(const char* name, PPCContext& ctx) {
  if (!REXCVAR_GET(log_pause_movie) || !frame_pacing::GameMessages_Paused() ||
      !TakeLine(g_call_lines, g_call_lines_since, kMaxCallLines)) {
    return;
  }
  REXLOG_INFO(
      "pause movie call: tick {} pass {} {} {} from {:08X}: r3 {:08X} r4 {:08X} r5 {:08X} "
      "r6 {:08X}",
      Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), name, uint32_t(ctx.lr),
      ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

}  // namespace

void PauseMovieLog_Before(uint8_t* base, uint32_t movie, uint32_t r4, uint32_t caller,
                          bool draw_only) {
  if (!REXCVAR_GET(log_pause_movie)) {
    return;
  }
  CountUpdate(movie, r4, caller, draw_only);
  g_logs[movie].Before(base, movie, r4, draw_only);
}

void PauseMovieLog_After(uint8_t* base, uint32_t movie) {
  if (!REXCVAR_GET(log_pause_movie)) {
    return;
  }
  g_logs[movie].After(base);
}

}  // namespace debug

// The sub-menus' calls on the movie's nodes, logged for --log_pause_movie.
// sub_8211C7E8 (r3 = movie, r4 = node, r5 = 8, r6 = value, from sub_8220D040)
// may set a node's animation target. sub_821BE5A8 and sub_8220D040 (r3 =
// node, r4 = selection) set the +80 flag of children 1-15 by the selection.
// sub_8220BA68 (r3 = the screen state [0x826D84D8]) switches sub-screens.
extern "C" REX_FUNC(sub_8211C7E8) {
  REX_FUNC_PROLOGUE();
  debug::LogCall("8211C7E8 movie node", ctx);
  __imp__sub_8211C7E8(ctx, base);
}

extern "C" REX_FUNC(sub_821BE5A8) {
  REX_FUNC_PROLOGUE();
  debug::LogCall("821BE5A8 flags by selection", ctx);
  __imp__sub_821BE5A8(ctx, base);
}

extern "C" REX_FUNC(sub_8220D040) {
  REX_FUNC_PROLOGUE();
  debug::LogCall("8220D040 node and flags by selection", ctx);
  __imp__sub_8220D040(ctx, base);
}

extern "C" REX_FUNC(sub_8220BA68) {
  REX_FUNC_PROLOGUE();
  debug::LogCall("8220BA68 screen switch", ctx);
  __imp__sub_8220BA68(ctx, base);
}
