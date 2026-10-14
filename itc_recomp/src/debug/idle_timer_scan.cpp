// Finds the title screen's idle timer (--find_idle_timer).
//
// Outside races, guest memory is compared every 30 logic passes (one second
// of game time). Words that step steadily like a timer over that second (an
// integer by +-30 or +-60 ticks or about +-1000 ms, a float by about +-1 s or
// +-30 or +-60 ticks) for several samples in a row are logged, with their
// values when an attract race starts. Words that did are then checked around
// every tick handler and message handler on logic passes, and the first
// change each handler makes to one is logged, to name the code that counts.

#include "idle_timer_scan.h"

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <cmath>
#include <cstring>
#include <set>
#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(find_idle_timer, false, "ITC",
                    "Log guest words that step steadily like a timer in menus (to find the "
                    "title screen's idle timer)");

namespace debug {

struct Range {
  uint32_t start;
  uint32_t size;
};
std::vector<Range> ReadableGuestRanges(uint8_t* base);

namespace {

constexpr int kPassesPerSample = 30;
constexpr int kStreakToLog = 3;
bool g_logic_pass = false;

float AsFloat(uint32_t bits) {
  float f;
  std::memcpy(&f, &bits, 4);
  return f;
}

// The step a word made over one sample if it looks like a timer's, else 0.
double TimerStep(uint32_t before, uint32_t after) {
  int64_t di = int64_t(after) - int64_t(before);
  if (before < 10000000 && after < 10000000) {
    int64_t a = std::llabs(di);
    if (a == 30 || a == 60 || (a >= 900 && a <= 1100)) {
      return double(di);
    }
  }
  float a = AsFloat(before), b = AsFloat(after);
  if (std::isfinite(a) && std::isfinite(b) && std::abs(a) < 1e6f && std::abs(b) < 1e6f) {
    double d = double(b) - double(a);
    double ad = std::abs(d);
    if ((ad > 0.9 && ad < 1.1) || (ad > 29 && ad < 31) || (ad > 59 && ad < 61)) {
      return d;
    }
  }
  return 0;
}

class IdleTimerScan {
 public:
  void OnLogicPass(uint8_t* base, bool in_race) {
    if (in_race) {
      if (!in_race_) {
        in_race_ = true;
        LogRaceStart(base);
      }
      return;
    }
    if (in_race_) {
      in_race_ = false;
      REXLOG_INFO("idle timer: race over, scanning again");
      snapshot_.clear();
      streaks_.clear();
    }
    if (++passes_ % kPassesPerSample != 0) {
      return;
    }
    std::vector<Range> ranges = ReadableGuestRanges(base);
    if (!snapshot_.empty()) {
      Compare(base);
    }
    ranges_ = std::move(ranges);
    snapshot_.clear();
    for (const Range& r : ranges_) {
      snapshot_.insert(snapshot_.end(), base + r.start, base + r.start + r.size);
    }
  }

 private:
  struct Streak {
    double step;
    int count;
    uint32_t value;
  };

  void Compare(uint8_t* base) {
    std::unordered_map<uint32_t, Streak> next;
    std::string sample;
    int logged = 0;
    size_t offset = 0;
    for (const Range& r : ranges_) {
      const uint8_t* then = snapshot_.data() + offset;
      offset += r.size;
      for (uint32_t i = 0; i + 4 <= r.size; i += 4) {
        if (std::memcmp(base + r.start + i, then + i, 4) == 0) {
          continue;
        }
        uint32_t address = r.start + i;
        uint32_t before = __builtin_bswap32(*reinterpret_cast<const uint32_t*>(then + i));
        uint32_t after = guest::LoadU32(base, address);
        double step = TimerStep(before, after);
        if (step == 0) {
          continue;
        }
        Streak s{step, 1, after};
        auto old = streaks_.find(address);
        if (old != streaks_.end() && std::abs(old->second.step - step) <= 0.1 * std::abs(step)) {
          s.count = old->second.count + 1;
        }
        next[address] = s;
        if (s.count >= kStreakToLog && logged < 60) {
          logged++;
          sample += fmt::format(" {:08X}:{}x{}={}({})", address, s.count, step, after,
                                AsFloat(after));
        }
      }
    }
    streaks_ = std::move(next);
    watched_.clear();
    for (const auto& [address, s] : streaks_) {
      if (s.count >= kStreakToLog && watched_.size() < 100) {
        watched_.push_back(address);
      }
    }
    REXLOG_INFO("idle timer: pass {} {} steady timers:{}", passes_, logged, sample);
  }

 public:
  void Before(uint8_t* base) {
    std::vector<uint32_t> values(watched_.size());
    for (size_t i = 0; i < watched_.size(); i++) {
      values[i] = guest::LoadU32(base, watched_[i]);
    }
    before_.push_back(std::move(values));
  }

  // Logs each watched word a handler changed, once per word and handler.
  void After(uint8_t* base, const char* kind, uint32_t id) {
    if (before_.empty()) {
      return;
    }
    std::vector<uint32_t> values = std::move(before_.back());
    before_.pop_back();
    for (size_t i = 0; i < values.size() && i < watched_.size(); i++) {
      uint32_t now = guest::LoadU32(base, watched_[i]);
      if (now != values[i] && logged_.insert({watched_[i], id}).second) {
        REXLOG_INFO("idle timer: {} {:08X} changed {:08X}: {} -> {}", kind, id, watched_[i],
                    values[i], now);
      }
    }
  }


 private:
  void LogRaceStart(uint8_t* base) {
    std::string list;
    for (const auto& [address, s] : streaks_) {
      if (s.count >= kStreakToLog) {
        uint32_t now = guest::LoadU32(base, address);
        list += fmt::format(" {:08X}:{}x{} last {}({}) now {}({})", address, s.count, s.step,
                            s.value, AsFloat(s.value), now, AsFloat(now));
      }
    }
    REXLOG_INFO("idle timer: race started; steady timers before it:{}", list);
  }

  bool in_race_ = false;
  uint64_t passes_ = 0;
  std::vector<Range> ranges_;
  std::vector<uint8_t> snapshot_;
  std::unordered_map<uint32_t, Streak> streaks_;
  std::vector<uint32_t> watched_;
  std::vector<std::vector<uint32_t>> before_;  // nested: messages within handlers
  std::set<std::pair<uint32_t, uint32_t>> logged_;
};

IdleTimerScan& Get() {
  static IdleTimerScan scan;
  return scan;
}

}  // namespace

void IdleTimerScan_OnPassStart(uint8_t* base, bool render_only, bool in_race) {
  g_logic_pass = false;
  if (!REXCVAR_GET(find_idle_timer) || render_only) {
    return;
  }
  g_logic_pass = !render_only;
  Get().OnLogicPass(base, in_race);
}

void IdleTimerScan_Before(uint8_t* base) {
  if (REXCVAR_GET(find_idle_timer) && g_logic_pass) {
    Get().Before(base);
  }
}

void IdleTimerScan_After(uint8_t* base, const char* kind, uint32_t id) {
  if (REXCVAR_GET(find_idle_timer) && g_logic_pass) {
    Get().After(base, kind, id);
  }
}

}  // namespace debug
