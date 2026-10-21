#include "frame_state_diff.h"

#include "frame_pacing/game_messages.h"
#include "guest.h"

#include "base/flags.h"
#include "base/log.h"

#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>

ITC_FLAG_BOOL(find_per_frame_state, false, "ITC",
              "Find game state that advances on render-only passes, and the code that "
              "advances it");
ITC_FLAG_INT32(find_per_frame_state_interval_ms, 2000, "ITC",
               "How often --find_per_frame_state samples a render-only pass (0 = every "
               "one; slow)");
ITC_FLAG_STRING(debug_hide, "", "ITC",
                "Testing: wrapped draw functions (hex, comma-separated) never to run, "
                "to see what they draw");
ITC_FLAG_STRING(watch_addresses, "", "ITC",
                "With --find_per_frame_state, guest addresses (hex, comma-separated) to "
                "watch from the start, logging every change with the code making it");
ITC_FLAG_BOOL(find_any_steps, false, "ITC",
              "With --find_per_frame_state, look for any steady step (a float changing "
              "by less than 1, or an integer by up to 4, by the same amount in two "
              "samples) instead of timer steps only");
ITC_FLAG_BOOL(watch_lights, false, "ITC",
              "With --find_per_frame_state, watch the light billboards' lights (the "
              "first 2 of each list drawn by sub_8209CC38, up to 64 lists), logging every change");
ITC_FLAG_BOOL(find_countdowns, false, "ITC",
              "With --find_per_frame_state, look for values counting down (a float in "
              "0..10 going down, or a small integer going down by one) instead of any "
              "timer step");

namespace debug {

struct Range {
  uint32_t start;
  uint32_t size;
};
std::vector<Range> ReadableGuestRanges(uint8_t* base);

namespace {

// Guest addresses that can hold game state: below the physical memory views.
constexpr uint64_t kGuestScanEnd = 0xA0000000;
constexpr size_t kMaxWatched = 200;

}  // namespace

std::vector<Range> ReadableGuestRanges(uint8_t* base) {
  std::vector<Range> ranges;
#ifdef _WIN32
  // Reads the Linux memory map; the scans aren't supported on Windows.
  (void)base;
  return ranges;
#endif
  std::ifstream maps("/proc/self/maps");
  std::string line;
  uint64_t guest_start = reinterpret_cast<uint64_t>(base);
  while (std::getline(maps, line)) {
    std::istringstream fields(line);
    std::string span, perms;
    fields >> span >> perms;
    if (perms.size() < 2 || perms[0] != 'r' || perms[1] != 'w') {
      continue;
    }
    uint64_t start = std::stoull(span.substr(0, span.find('-')), nullptr, 16);
    uint64_t end = std::stoull(span.substr(span.find('-') + 1), nullptr, 16);
    if (end <= guest_start || start >= guest_start + kGuestScanEnd) {
      continue;
    }
    start = std::max(start, guest_start);
    end = std::min(end, guest_start + kGuestScanEnd);
    ranges.push_back({uint32_t(start - guest_start), uint32_t(end - start)});
  }
  return ranges;
}

namespace {

float AsFloat(uint32_t bits) {
  float f;
  std::memcpy(&f, &bits, 4);
  return f;
}

// Whether a word went down like a countdown: a float in 0..10 going down by
// a small step, or a small integer going down by one.
const char* CountdownStep(uint32_t before, uint32_t after) {
  float a = AsFloat(before), b = AsFloat(after);
  if (std::isfinite(a) && std::isfinite(b) && a > 0 && a <= 10 && b < a && a - b < 0.5f &&
      a - b > 1e-4f) {
    return "float";
  }
  if (int64_t(before) - int64_t(after) == 1 && before <= 300) {
    return "int";
  }
  return nullptr;
}

// Whether a word changed by a small amount: a normal float by less than 1,
// or a small integer by up to 4. Sets `step` to the change.
const char* AnyStep(uint32_t before, uint32_t after, double& step) {
  float a = AsFloat(before), b = AsFloat(after);
  auto plain = [](float f) {
    return f == 0 || (std::abs(f) >= 1e-6f && std::abs(f) < 1e5f);
  };
  if (plain(a) && plain(b) && a != b && std::abs(b - a) < 1) {
    step = double(b) - double(a);
    return "float";
  }
  int64_t di = int64_t(after) - int64_t(before);
  if (di != 0 && di >= -4 && di <= 4 && before < 100000 && after < 100000) {
    step = double(di);
    return "int";
  }
  return nullptr;
}

// Whether a word stepped like a frame timer: a float by +-1/30 or +-1/60 s,
// or a small integer by +-1.
const char* TimerStep(uint32_t before, uint32_t after) {
  if (ITC_FLAG(find_countdowns)) {
    return CountdownStep(before, after);
  }
  float a = AsFloat(before), b = AsFloat(after);
  if (std::isfinite(a) && std::isfinite(b) && a != b) {
    float d = std::abs(b - a);
    if (std::abs(d - 1.0f / 30) < 1e-5f || std::abs(d - 1.0f / 60) < 1e-5f) {
      return "float";
    }
  }
  int64_t di = int64_t(after) - int64_t(before);
  if ((di == 1 || di == -1) && before < 100000 && after < 100000) {
    return "int";
  }
  return nullptr;
}

class FrameStateDiff {
 public:
  void OnPassStart(uint8_t* base, bool render_only) {
    if (!ITC_FLAG(find_per_frame_state)) {
      return;
    }
    if (snapshot_taken_) {
      Compare(base);
      snapshot_taken_ = false;
    }
    render_only_ = render_only;
    before_.clear();
    auto now = std::chrono::steady_clock::now();
    if (render_only && now - last_snapshot_ >= std::chrono::milliseconds(
                           ITC_FLAG(find_per_frame_state_interval_ms))) {
      last_snapshot_ = now;
      ranges_ = ReadableGuestRanges(base);
      snapshot_.clear();
      for (const Range& r : ranges_) {
        snapshot_.insert(snapshot_.end(), base + r.start, base + r.start + r.size);
      }
      snapshot_taken_ = true;
    }
    LogChanges(now);
  }

  void Before(uint8_t* base) {
    if (!ITC_FLAG(find_per_frame_state) || !render_only_) {
      return;
    }
    AddForcedWatches();
    if (watched_.empty()) {
      return;
    }
    std::vector<uint32_t> values(watched_.size());
    for (size_t i = 0; i < watched_.size(); i++) {
      values[i] = guest::LoadU32(base, watched_[i]);
    }
    before_.push_back(std::move(values));
  }

  void After(uint8_t* base, const char* kind, uint32_t id) {
    if (!ITC_FLAG(find_per_frame_state) || !render_only_ || before_.empty()) {
      return;
    }
    std::vector<uint32_t> values = std::move(before_.back());
    before_.pop_back();
    for (size_t i = 0; i < values.size(); i++) {
      uint32_t now = guest::LoadU32(base, watched_[i]);
      if (now != values[i]) {
        changes_[{std::string(kind), id, watched_[i]}]++;
        if (forced_.count(watched_[i]) && logged_values_ < kMaxLoggedValues) {
          logged_values_++;
          ITC_LOG_INFO("per-frame state: pass {} {} {:08X} wrote {:08X}: {} -> {}",
                       frame_pacing::GameMessages_PassNumber(), kind, id, watched_[i], values[i],
                       now);
        }
      }
    }
  }

 private:
  void Compare(uint8_t* base) {
    // Words are compared as stored (big endian), converted to values for the
    // timer test.
    size_t offset = 0;
    int found = 0;
    std::string sample;
    std::vector<uint32_t> stepped_now;
    for (const Range& r : ranges_) {
      const uint8_t* now = base + r.start;
      const uint8_t* then = snapshot_.data() + offset;
      for (uint32_t i = 0; i + 4 <= r.size; i += 4) {
        if (std::memcmp(now + i, then + i, 4) == 0) {
          continue;
        }
        uint32_t address = r.start + i;
        uint32_t before = __builtin_bswap32(*reinterpret_cast<const uint32_t*>(then + i));
        uint32_t after = guest::LoadU32(base, address);
        const char* step;
        if (ITC_FLAG(find_any_steps)) {
          // Only steps equal to the word's step in an earlier sample: a
          // steady rate, not a buffer being rewritten.
          double change = 0;
          step = AnyStep(before, after, change);
          if (!step) {
            continue;
          }
          auto last = last_step_.find(address);
          bool steady = last != last_step_.end() &&
                        std::abs(last->second - change) <= 0.01 * std::abs(change);
          last_step_[address] = change;
          if (!steady) {
            continue;
          }
        } else {
          step = TimerStep(before, after);
          if (!step) {
            continue;
          }
        }
        found++;
        if (sample.size() < 3000) {
          sample += std::string(step) == "float"
                        ? fmt::format(" {:08X}:{}->{}", address, AsFloat(before), AsFloat(after))
                        : fmt::format(" {:08X}:{}->{}", address, before, after);
        }
        stepped_now.push_back(address);
      }
      offset += r.size;
    }
    ITC_LOG_INFO("per-frame state: {} words stepped like timers over a render-only pass:{}", found,
                 sample);
    // Watch words that stepped in more than one sampled pass: steady
    // per-frame counters, not one-off changes (a load finishing).
    for (uint32_t address : stepped_now) {
      hits_[address]++;
    }
    watched_.clear();
    forced_added_ = false;
    for (const auto& [address, hits] : hits_) {
      if (hits >= 2 && watched_.size() < kMaxWatched) {
        watched_.push_back(address);
      }
    }
    if (hits_.size() > 200000) {
      hits_.clear();
    }
    if (last_step_.size() > 2000000) {
      last_step_.clear();
    }
  }

  void LogChanges(std::chrono::steady_clock::time_point now) {
    if (now - last_log_ < std::chrono::seconds(2) || changes_.empty()) {
      return;
    }
    last_log_ = now;
    logged_values_ = 0;
    for (const auto& [key, n] : changes_) {
      ITC_LOG_INFO("per-frame state: {} {:08X} changed {:08X} ({} times)", std::get<0>(key),
                   std::get<1>(key), std::get<2>(key), n);
    }
    changes_.clear();
  }

  void AddForcedWatches() {
    if (forced_added_) {
      return;
    }
    forced_added_ = true;
    if (!forced_parsed_) {
      forced_parsed_ = true;
      std::string list = ITC_FLAG(watch_addresses);
      size_t start = 0;
      while (start < list.size()) {
        size_t end = list.find(',', start);
        std::string item =
            list.substr(start, end == std::string::npos ? std::string::npos : end - start);
        if (!item.empty()) {
          forced_.insert(uint32_t(std::stoul(item, nullptr, 16)));
        }
        if (end == std::string::npos) {
          break;
        }
        start = end + 1;
      }
    }
    for (uint32_t address : forced_) {
      if (std::find(watched_.begin(), watched_.end(), address) == watched_.end()) {
        watched_.insert(watched_.begin(), address);
      }
    }
  }

 public:
  // Watches a word from now on, as --watch_addresses does.
  void Watch(uint32_t address) {
    if (forced_.insert(address).second) {
      forced_added_ = false;
    }
  }

 private:

  // Every change to a --watch_addresses word is logged with its values, up
  // to this many per 2 s.
  static constexpr int kMaxLoggedValues = 200;
  int logged_values_ = 0;
  std::set<uint32_t> forced_;
  bool forced_added_ = false;
  bool forced_parsed_ = false;
  bool render_only_ = false;
  bool snapshot_taken_ = false;
  std::vector<Range> ranges_;
  std::vector<uint8_t> snapshot_;
  std::chrono::steady_clock::time_point last_snapshot_{}, last_log_{};
  std::vector<uint32_t> watched_;
  std::map<uint32_t, int> hits_;  // sampled passes each word stepped in
  std::unordered_map<uint32_t, double> last_step_;  // --find_any_steps
  std::vector<std::vector<uint32_t>> before_;  // nested: messages within handlers
  std::map<std::tuple<std::string, uint32_t, uint32_t>, int> changes_;
};

FrameStateDiff& Get() {
  static FrameStateDiff diff;
  return diff;
}

}  // namespace

void FrameStateDiff_OnPassStart(uint8_t* base, bool render_only) {
  Get().OnPassStart(base, render_only);
}

void FrameStateDiff_Before(uint8_t* base) {
  Get().Before(base);
}

bool IsHidden(uint32_t function) {
  std::string list = ITC_FLAG(debug_hide);
  size_t start = 0;
  while (start < list.size()) {
    size_t end = list.find(',', start);
    std::string item = list.substr(start, end == std::string::npos ? std::string::npos : end - start);
    if (!item.empty() && uint32_t(std::stoul(item, nullptr, 16)) == function) {
      return true;
    }
    if (end == std::string::npos) {
      break;
    }
    start = end + 1;
  }
  return false;
}

void FrameStateDiff_After(uint8_t* base, const char* kind, uint32_t id) {
  Get().After(base, kind, id);
}

void FrameStateDiff_Watch(uint32_t address) {
  Get().Watch(address);
}

}  // namespace debug

// The race draw step's parts (sub_8215B4F8 calls these), wrapped only to
// name which one advances per-frame state (logged as "draw step part").
#define ITC_FRAME_STATE_WRAP(address)                       \
  extern "C" REX_FUNC(sub_##address) {                      \
    REX_FUNC_PROLOGUE();                                    \
    if (debug::IsHidden(0x##address)) {                     \
      return;                                               \
    }                                                       \
    debug::FrameStateDiff_Before(base);                     \
    __imp__sub_##address(ctx, base);                        \
    debug::FrameStateDiff_After(base, "draw step part", 0x##address); \
  }
ITC_FRAME_STATE_WRAP(8209F8A0)
ITC_FRAME_STATE_WRAP(8209F900)
ITC_FRAME_STATE_WRAP(820C5DE8)
ITC_FRAME_STATE_WRAP(820F6068)
ITC_FRAME_STATE_WRAP(82130978)
// The render orchestrator's (sub_820C5DE8) callees.
ITC_FRAME_STATE_WRAP(8209F7A8)
ITC_FRAME_STATE_WRAP(820A0958)
ITC_FRAME_STATE_WRAP(820AA1A8)
ITC_FRAME_STATE_WRAP(820AB560)
ITC_FRAME_STATE_WRAP(820C4EB8)
ITC_FRAME_STATE_WRAP(820C4FB8)
ITC_FRAME_STATE_WRAP(820DAA38)
ITC_FRAME_STATE_WRAP(820DAAC8)
ITC_FRAME_STATE_WRAP(820DAB10)
ITC_FRAME_STATE_WRAP(820FFD88)
ITC_FRAME_STATE_WRAP(82593FC0)
ITC_FRAME_STATE_WRAP(82597118)
ITC_FRAME_STATE_WRAP(82597518)
ITC_FRAME_STATE_WRAP(82597610)
ITC_FRAME_STATE_WRAP(82598768)
// The light billboards' batch (calls sub_8209C1A0 and sub_8209C6E0); r4 is
// the light list: count at +32, 48-byte lights at +36 (position at +0, size
// at +32 and +36; a light with neither size above 0 is skipped).
// --watch_lights watches every word of its first lights.
extern "C" REX_FUNC(sub_8209CC38) {
  REX_FUNC_PROLOGUE();
  if (debug::IsHidden(0x8209CC38)) {
    return;
  }
  if (ITC_FLAG(watch_lights) && ITC_FLAG(find_per_frame_state)) {
    static std::set<uint32_t> seen;
    uint32_t list = ctx.r4.u32;
    uint32_t lights = guest::LoadU32(base, list + 36);
    uint32_t count = guest::LoadU32(base, list + 32);
    if (seen.insert(lights).second && seen.size() <= 64) {
      ITC_LOG_INFO("per-frame state: watching light list {:08X}: {} lights at {:08X}, type {}",
                   list, count, lights, guest::LoadU32(base, list));
      for (uint32_t i = 0; i < std::min<uint32_t>(count, 2) * 48; i += 4) {
        debug::FrameStateDiff_Watch(lights + i);
      }
    }
  }
  debug::FrameStateDiff_Before(base);
  __imp__sub_8209CC38(ctx, base);
  debug::FrameStateDiff_After(base, "draw step part", 0x8209CC38);
}
// The per-car update called from Draw (sub_820DAC68), and its callees.
ITC_FRAME_STATE_WRAP(820DAC68)
ITC_FRAME_STATE_WRAP(820C5C88)
ITC_FRAME_STATE_WRAP(820D4048)
ITC_FRAME_STATE_WRAP(820D72D8)
ITC_FRAME_STATE_WRAP(820D92A0)
ITC_FRAME_STATE_WRAP(820D9B68)
ITC_FRAME_STATE_WRAP(820D9C50)
ITC_FRAME_STATE_WRAP(820D9EA0)
ITC_FRAME_STATE_WRAP(820DAB58)
ITC_FRAME_STATE_WRAP(820DBFF8)
ITC_FRAME_STATE_WRAP(820DC498)
ITC_FRAME_STATE_WRAP(820DCBC0)
ITC_FRAME_STATE_WRAP(820DDA60)
ITC_FRAME_STATE_WRAP(820DDE58)
ITC_FRAME_STATE_WRAP(820E2E68)
ITC_FRAME_STATE_WRAP(820E2EE8)
ITC_FRAME_STATE_WRAP(820E4140)
ITC_FRAME_STATE_WRAP(820E53F0)
ITC_FRAME_STATE_WRAP(820E5A68)
ITC_FRAME_STATE_WRAP(820E6A40)
ITC_FRAME_STATE_WRAP(820E7328)
ITC_FRAME_STATE_WRAP(820E8908)
ITC_FRAME_STATE_WRAP(820ECE28)
#undef ITC_FRAME_STATE_WRAP

// Draw step part 0x82133960, rewritten to also name each view's draw: for
// the 15 per-view objects at 0x826C5D80 whose camera pass (index at
// 0x826174C8 + 36 * i) is active and whose +52 is clear, calls vtable slot 0,
// then clears +8056 and +48 and flips +8060 (logged as "view draw").
extern "C" REX_FUNC(sub_82133960) {
  REX_FUNC_PROLOGUE();
  if (debug::IsHidden(0x82133960)) {
    return;
  }
  constexpr uint32_t kViews = 0x826C5D80;
  constexpr uint32_t kViewPassIndices = 0x826174C8;
  constexpr uint32_t kCameraPasses = 0x826BAC00;
  debug::FrameStateDiff_Before(base);
  {
    guest::StackFrame frame(ctx, 128);  // the original's stwu r1,-128(r1)
    for (uint32_t i = 0; i < 15; i++) {
      uint32_t view = guest::LoadU32(base, kViews + 4 * i);
      if (view == 0) {
        continue;
      }
      uint32_t pass = kCameraPasses + 592 * guest::LoadU32(base, kViewPassIndices + 36 * i);
      if (guest::LoadU32(base, pass + 588) == 0 || guest::LoadU32(base, view + 52) != 0) {
        continue;
      }
      uint32_t draw = guest::LoadU32(base, guest::LoadU32(base, view));
      debug::FrameStateDiff_Before(base);
      ctx.r3.u64 = view;
      REX_CALL_INDIRECT_FUNC(draw);
      debug::FrameStateDiff_After(base, "view draw", draw);
      view = guest::LoadU32(base, kViews + 4 * i);
      uint32_t flip = guest::LoadU32(base, view + 8060);
      guest::StoreU32(base, view + 8056, 0);
      guest::StoreU32(base, view + 48, 0);
      guest::StoreU32(base, view + 8060, flip ^ 1);
    }
  }
  debug::FrameStateDiff_After(base, "draw step part", 0x82133960);
}

// Tick handler 0x82096340 is a thunk calling vtable slot 1 of the object in
// r3; name the function it reaches (logged as "thunk target").
extern "C" REX_FUNC(sub_82096340) {
  REX_FUNC_PROLOGUE();
  uint32_t target = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 4);
  debug::FrameStateDiff_Before(base);
  __imp__sub_82096340(ctx, base);
  debug::FrameStateDiff_After(base, "thunk target", target);
}
