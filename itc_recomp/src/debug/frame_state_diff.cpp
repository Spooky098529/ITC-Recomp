#include "frame_state_diff.h"

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <map>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>

REXCVAR_DEFINE_BOOL(find_per_frame_state, false, "ITC",
                    "Find game state that advances on render-only passes, and the code that "
                    "advances it");

namespace debug {
namespace {

// Guest addresses that can hold game state: below the physical memory views.
constexpr uint64_t kGuestScanEnd = 0xA0000000;
constexpr size_t kMaxWatched = 200;

struct Range {
  uint32_t start;
  uint32_t size;
};

std::vector<Range> ReadableGuestRanges(uint8_t* base) {
  std::vector<Range> ranges;
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

float AsFloat(uint32_t bits) {
  float f;
  std::memcpy(&f, &bits, 4);
  return f;
}

// Whether a word stepped like a frame timer: a float by +-1/30 or +-1/60 s,
// or a small integer by +-1.
const char* TimerStep(uint32_t before, uint32_t after) {
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
    if (!REXCVAR_GET(find_per_frame_state)) {
      return;
    }
    if (snapshot_taken_) {
      Compare(base);
      snapshot_taken_ = false;
    }
    render_only_ = render_only;
    before_.clear();
    auto now = std::chrono::steady_clock::now();
    if (render_only && now - last_snapshot_ >= std::chrono::seconds(2)) {
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
    if (!REXCVAR_GET(find_per_frame_state) || !render_only_ || watched_.empty()) {
      return;
    }
    std::vector<uint32_t> values(watched_.size());
    for (size_t i = 0; i < watched_.size(); i++) {
      values[i] = guest::LoadU32(base, watched_[i]);
    }
    before_.push_back(std::move(values));
  }

  void After(uint8_t* base, const char* kind, uint32_t id) {
    if (!REXCVAR_GET(find_per_frame_state) || !render_only_ || before_.empty()) {
      return;
    }
    std::vector<uint32_t> values = std::move(before_.back());
    before_.pop_back();
    for (size_t i = 0; i < values.size(); i++) {
      if (guest::LoadU32(base, watched_[i]) != values[i]) {
        changes_[{std::string(kind), id, watched_[i]}]++;
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
        const char* step = TimerStep(before, after);
        if (!step) {
          continue;
        }
        found++;
        if (sample.size() < 3000) {
          sample += std::string(step) == "float"
                        ? fmt::format(" {:08X}:{}->{}", address, AsFloat(before), AsFloat(after))
                        : fmt::format(" {:08X}:{}->{}", address, before, after);
        }
        if (watched_.size() < kMaxWatched &&
            std::find(watched_.begin(), watched_.end(), address) == watched_.end()) {
          watched_.push_back(address);
        }
      }
      offset += r.size;
    }
    REXLOG_INFO("per-frame state: {} words stepped like timers over a render-only pass:{}", found,
                sample);
  }

  void LogChanges(std::chrono::steady_clock::time_point now) {
    if (now - last_log_ < std::chrono::seconds(2) || changes_.empty()) {
      return;
    }
    last_log_ = now;
    for (const auto& [key, n] : changes_) {
      REXLOG_INFO("per-frame state: {} {:08X} changed {:08X} ({} times)", std::get<0>(key),
                  std::get<1>(key), std::get<2>(key), n);
    }
    changes_.clear();
  }

  bool render_only_ = false;
  bool snapshot_taken_ = false;
  std::vector<Range> ranges_;
  std::vector<uint8_t> snapshot_;
  std::chrono::steady_clock::time_point last_snapshot_{}, last_log_{};
  std::vector<uint32_t> watched_;
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

void FrameStateDiff_After(uint8_t* base, const char* kind, uint32_t id) {
  Get().After(base, kind, id);
}

}  // namespace debug
