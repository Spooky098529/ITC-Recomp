// Logs what changes the pause menu's movie and its sub-menus' arrow
// (--log_pause_movie), to find what keeps the arrow sliding past the
// selected item once the movie is advanced once a tick
// (docs/pause-menu-fix.md, "The sub-menus' arrow").
//
// Every UI movie update (sub_8211D2F8) is counted once a second by movie,
// mode, caller (lr: the caller of sub_8211E1B8, which jumps to it) and pass
// type. While paused, around each update of each movie: the movie object (0xA000 bytes, what the UI movie
// replay keeps) and the objects up to three pointers away from it (0x200 bytes
// each) are compared before and after the update ("in update") and against
// the last update's end ("between": written by other code since). Each
// changed word is logged with the pass type, unless it changed in 60 or more
// updates in the last second (drawing bookkeeping); a summary of the busy
// words is logged once a second. The sub-menus' calls that set the movie's
// nodes (sub_8211C7E8, sub_821BE5A8, sub_8220D040) and their state switch
// (sub_8220BA68) are logged with their arguments. For a few passes after
// each of those, every adding animation track applied (sub_825A0548, the
// arrow's slide among them) is logged with its target word's values, and
// the words they moved are logged at the start and end of each update and
// whenever a setting track writes them.

#include "debug/pause_movie_log.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"

#include "base/flags.h"
#include "base/log.h"

#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <cstring>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

ITC_FLAG_BOOL(log_pause_movie, false, "ITC",
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
constexpr size_t kMaxObjects = 16384;
constexpr int kBusyChanges = 60;     // changes a second that mark a word as bookkeeping
constexpr int kMaxLines = 800;       // change lines per 2 s
constexpr int kMaxCallLines = 400;   // call lines per 2 s
constexpr int kMaxTrackLines = 2000; // track lines per 2 s

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
    active_ = ITC_FLAG(log_pause_movie) && frame_pacing::GameMessages_Paused();
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
      ITC_LOG_INFO("pause movie: tick {} movie {:08X} isn't readable ({} ranges)", Ticks(), movie,
                   ranges_.size());
      return;
    }
    objects_.push_back({movie, kMovieBytes, 0});
    std::unordered_map<uint32_t, bool> seen{{movie, true}};
    // Three levels of pointers: words in 0x01000000-0x7FFFFFFF (the heap, and
    // the movie data at 0x03xxxxxx where its nodes are) that point
    // at readable memory outside the objects already taken.
    for (size_t level = 0, from = 0; level < 3; level++) {
      size_t to = objects_.size();
      for (size_t i = from; i < to && objects_.size() < kMaxObjects; i++) {
        const Object o = objects_[i];
        for (uint32_t off = 0; off + 4 <= o.size && objects_.size() < kMaxObjects; off += 4) {
          uint32_t v = guest::LoadU32(base, o.address + off);
          if (v < 0x01000000 || v >= 0x80000000 || (v & 3) != 0) {
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
    ITC_LOG_INFO("pause movie: tick {} movie {:08X}: watching {} objects, {} bytes", Ticks(),
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
            ITC_LOG_INFO(
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
        ITC_LOG_INFO(
            "pause movie busy: movie {:08X} {:08X}: {} changes (in update {} logic, {} render-only; "
            "between {}), {:08X} -> {:08X}",
            movie_, address, busy.changes, busy.logic, busy.render_only, busy.between, busy.first,
            busy.last);
      }
    }
    if (lines > 40) {
      ITC_LOG_INFO("pause movie busy: {} more", lines - 40);
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
    ITC_LOG_INFO("pause movie updates: tick {} movie {:08X} r4 {} from {:08X} {} {}{}: {}",
                 Ticks(), m, mode, from, was_paused ? "paused" : "running",
                 render_only ? "render-only" : "logic", made_draw_only ? " made draw-only" : "",
                 n);
  }
  g_updates.clear();
}

// Words adding tracks have moved (after != before), watched around updates.
std::vector<uint32_t> g_moving_targets;
constexpr size_t kMaxMovingTargets = 16;
int g_track_lines = 0;
std::chrono::steady_clock::time_point g_track_lines_since{};
constexpr uint64_t kTracePasses = 12;
uint64_t g_trace_until_pass = 0;

void LogMovingTargets(uint8_t* base, uint32_t movie, const char* when) {
  if (g_moving_targets.empty() || !frame_pacing::GameMessages_Paused() ||
      frame_pacing::GameMessages_PassNumber() > g_trace_until_pass ||
      !TakeLine(g_track_lines, g_track_lines_since, kMaxTrackLines)) {
    return;
  }
  std::string line;
  for (uint32_t target : g_moving_targets) {
    line += fmt::format(" {:08X}={:g}", target, guest::LoadFloat(base, target));
  }
  ITC_LOG_INFO("pause movie targets: tick {} pass {} {} movie {:08X} {}:{}", Ticks(),
               frame_pacing::GameMessages_PassNumber(), PassType(), movie, when, line);
}

int g_call_lines = 0;
std::chrono::steady_clock::time_point g_call_lines_since{};

void LogCall(const char* name, PPCContext& ctx) {
  if (ITC_FLAG(log_pause_movie) && frame_pacing::GameMessages_Paused()) {
    g_trace_until_pass = frame_pacing::GameMessages_PassNumber() + kTracePasses;
  }
  if (!ITC_FLAG(log_pause_movie) || !frame_pacing::GameMessages_Paused() ||
      !TakeLine(g_call_lines, g_call_lines_since, kMaxCallLines)) {
    return;
  }
  ITC_LOG_INFO(
      "pause movie call: tick {} pass {} {} {} from {:08X}: r3 {:08X} r4 {:08X} r5 {:08X} "
      "r6 {:08X}",
      Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), name, uint32_t(ctx.lr),
      ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

}  // namespace

void PauseMovieLog_Before(uint8_t* base, uint32_t movie, uint32_t r4, uint32_t caller) {
  constexpr bool draw_only = false;
  if (!ITC_FLAG(log_pause_movie)) {
    return;
  }
  CountUpdate(movie, r4, caller, draw_only);
  g_logs[movie].Before(base, movie, r4, draw_only);
  LogMovingTargets(base, movie, "update start");
}

void PauseMovieLog_After(uint8_t* base, uint32_t movie, uint32_t caller, const char* outcome,
                         int occurrence) {
  if (!ITC_FLAG(log_pause_movie)) {
    return;
  }
  g_logs[movie].After(base);
  LogMovingTargets(base, movie, "update end");
  // Every update for a few passes after an animation call, with the replay's
  // outcome and the sub-menus' movie's animation clocks.
  uint64_t pass = frame_pacing::GameMessages_PassNumber();
  if (frame_pacing::GameMessages_Paused() && pass <= g_trace_until_pass &&
      TakeLine(g_call_lines, g_call_lines_since, kMaxCallLines)) {
    ITC_LOG_INFO(
        "pause movie update: tick {} pass {} {} movie {:08X} from {:08X} occurrence {} {}: "
        "clocks +9154 {:g} +96E0 {:g} +9708 {:g}",
        Ticks(), pass, PassType(), movie, caller, occurrence, outcome,
        guest::LoadFloat(base, movie + 0x9154), guest::LoadFloat(base, movie + 0x96E0),
        guest::LoadFloat(base, movie + 0x9708));
  }
}

bool PauseMovieLog_TracksWanted() {
  return ITC_FLAG(log_pause_movie) && frame_pacing::GameMessages_Paused() &&
         frame_pacing::GameMessages_PassNumber() <= g_trace_until_pass;
}

void PauseMovieLog_Track(uint32_t movie, uint32_t node, uint32_t property, uint32_t type,
                         bool adds, uint32_t target, uint32_t before, uint32_t after,
                         bool skipped) {
  bool moving = std::find(g_moving_targets.begin(), g_moving_targets.end(), target) !=
                g_moving_targets.end();
  if (adds && after != before && !moving && g_moving_targets.size() < kMaxMovingTargets) {
    g_moving_targets.push_back(target);
    moving = true;
  }
  if ((!adds && !moving) || !TakeLine(g_track_lines, g_track_lines_since, kMaxTrackLines)) {
    return;
  }
  // Type 0 is a float, 1 and 2 integers (sub_825A0548).
  float before_f = std::bit_cast<float>(before), after_f = std::bit_cast<float>(after);
  ITC_LOG_INFO(
      "pause movie track: tick {} pass {} {} movie {:08X} {} property {} type {} node {:08X} "
      "target {:08X} (+{:X}) {} -> {} ({:+}){}",
      Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), movie,
      adds ? "adds" : "sets", property, type, node,
      target, target - node, type == 0 ? fmt::format("{:g}", before_f) : fmt::format("{}", int32_t(before)),
      type == 0 ? fmt::format("{:g}", after_f) : fmt::format("{}", int32_t(after)),
      type == 0 ? double(after_f - before_f) : double(int32_t(after - before)),
      skipped ? " skipped" : "");
}

int g_put_back_lines = 0;
std::chrono::steady_clock::time_point g_put_back_lines_since{};

void PauseMovieLog_PutBack(uint32_t movie, uint32_t property, uint32_t target, bool put_back) {
  if (!ITC_FLAG(log_pause_movie) ||
      !TakeLine(g_put_back_lines, g_put_back_lines_since, kMaxCallLines)) {
    return;
  }
  ITC_LOG_INFO("pause movie track target: tick {} movie {:08X} property {} target {:08X} {}",
               Ticks(), movie, property, target,
               put_back ? "put back by the update" : "not put back by the update");
}

std::map<std::tuple<uint32_t, uint32_t, uint32_t>, int> g_skipped;
std::chrono::steady_clock::time_point g_skipped_since{};

void PauseMovieLog_Skipped(uint32_t movie, uint32_t property, uint32_t target) {
  if (!ITC_FLAG(log_pause_movie)) {
    return;
  }
  g_skipped[{movie, property, target}]++;
  auto now = std::chrono::steady_clock::now();
  if (now - g_skipped_since < std::chrono::seconds(2)) {
    return;
  }
  g_skipped_since = now;
  for (const auto& [key, n] : g_skipped) {
    const auto& [m, p, t] = key;
    ITC_LOG_INFO("pause movie skipped tracks: tick {} movie {:08X} property {} target {:08X}: {}",
                 Ticks(), m, p, t, n);
  }
  g_skipped.clear();
}

// Runs `call`, a function the UI movie update calls, and logs which of the
// moved words it changes (to find what puts the arrow's track back after
// the update applies it).
template <typename Call>
void WatchMovingTargets(const char* name, PPCContext& ctx, uint8_t* base, Call&& call) {
  if (g_moving_targets.empty() || !PauseMovieLog_TracksWanted()) {
    call();
    return;
  }
  uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, lr = uint32_t(ctx.lr);
  std::array<uint32_t, kMaxMovingTargets> before{};
  for (size_t i = 0; i < g_moving_targets.size(); i++) {
    before[i] = guest::LoadU32(base, g_moving_targets[i]);
  }
  call();
  for (size_t i = 0; i < g_moving_targets.size(); i++) {
    uint32_t after = guest::LoadU32(base, g_moving_targets[i]);
    if (after != before[i] && TakeLine(g_track_lines, g_track_lines_since, kMaxTrackLines)) {
      ITC_LOG_INFO(
          "pause movie target written: tick {} pass {} {} {} from {:08X} r3 {:08X} r4 {:08X}: "
          "{:08X} {:g} -> {:g}",
          Ticks(), frame_pacing::GameMessages_PassNumber(), PassType(), name, lr, r3, r4,
          g_moving_targets[i], std::bit_cast<float>(before[i]), std::bit_cast<float>(after));
    }
  }
}

}  // namespace debug

// The functions the UI movie update (sub_8211D2F8) calls besides the
// tracks' sub_825A08E0, watched for writes to the moved words.
#define WATCH_MOVIE_CALL(address)                                                       \
  extern "C" REX_FUNC(sub_##address) {                                                  \
    REX_FUNC_PROLOGUE();                                                                \
    if (!ITC_FLAG(log_pause_movie)) {                                                \
      __imp__sub_##address(ctx, base);                                                  \
      return;                                                                           \
    }                                                                                   \
    debug::WatchMovingTargets(#address, ctx, base, [&] { __imp__sub_##address(ctx, base); }); \
  }
WATCH_MOVIE_CALL(82119B80)
WATCH_MOVIE_CALL(8259FA00)
WATCH_MOVIE_CALL(8211CD90)
WATCH_MOVIE_CALL(8259DAC0)
WATCH_MOVIE_CALL(8211B4D0)
WATCH_MOVIE_CALL(8211B080)
#undef WATCH_MOVIE_CALL

// The sub-menus' calls on the movie's nodes, logged for --log_pause_movie.
// sub_8211C7E8 (r3 = movie, r4 = node, r5 = 8, r6 = value, from sub_8220D040)
// may set a node's animation target. sub_821BE5A8 and sub_8220D040 (r3 =
// node, r4 = selection) set the +80 flag of children 1-15 by the selection.
// sub_8220BA68 (r3 = the screen state [0x826D84D8]) switches sub-screens.
extern "C" REX_FUNC(sub_8211C7E8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_8211C7E8(ctx, base);
    return;
  }
  debug::LogCall("8211C7E8 movie node", ctx);
  __imp__sub_8211C7E8(ctx, base);
}

extern "C" REX_FUNC(sub_821BE5A8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_821BE5A8(ctx, base);
    return;
  }
  debug::LogCall("821BE5A8 flags by selection", ctx);
  __imp__sub_821BE5A8(ctx, base);
}

extern "C" REX_FUNC(sub_8220D040) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_8220D040(ctx, base);
    return;
  }
  debug::LogCall("8220D040 node and flags by selection", ctx);
  __imp__sub_8220D040(ctx, base);
}

// The selection-change handler sub_8220DD98 (r3 = its state; runs when +0
// is 0) starts the movie's animations: sub_8211DE38 and sub_8211D978 (r3 =
// movie, r4 = 8, r5 = item + 4: the items'), sub_8211DDD8 and sub_8211DF20
// (r4 = 8, 20, 19), and sub_8211DAC8 (r4 = 20 after a move down, 19 up),
// then updates the movie once (from 8220DF24).
extern "C" REX_FUNC(sub_8211DAC8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_8211DAC8(ctx, base);
    return;
  }
  debug::LogCall("8211DAC8 start animation", ctx);
  __imp__sub_8211DAC8(ctx, base);
}

extern "C" REX_FUNC(sub_8211D978) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_8211D978(ctx, base);
    return;
  }
  debug::LogCall("8211D978 start item animation", ctx);
  __imp__sub_8211D978(ctx, base);
}

extern "C" REX_FUNC(sub_8211DE38) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_8211DE38(ctx, base);
    return;
  }
  debug::LogCall("8211DE38", ctx);
  __imp__sub_8211DE38(ctx, base);
}

extern "C" REX_FUNC(sub_8211DDD8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_8211DDD8(ctx, base);
    return;
  }
  debug::LogCall("8211DDD8", ctx);
  __imp__sub_8211DDD8(ctx, base);
}

extern "C" REX_FUNC(sub_8211DF20) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_8211DF20(ctx, base);
    return;
  }
  debug::LogCall("8211DF20", ctx);
  __imp__sub_8211DF20(ctx, base);
}

extern "C" REX_FUNC(sub_8220BA68) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(log_pause_movie)) {
    __imp__sub_8220BA68(ctx, base);
    return;
  }
  debug::LogCall("8220BA68 screen switch", ctx);
  __imp__sub_8220BA68(ctx, base);
}
