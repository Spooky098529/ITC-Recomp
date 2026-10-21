// Finds the pause menu's selection (--find_menu_selection).

#include "menu_selection_scan.h"

#include "frame_pacing/game_messages.h"
#include "guest.h"

#include "base/flags.h"
#include "base/log.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <map>
#include <string>
#include <vector>

ITC_FLAG_BOOL(find_menu_selection, false, "ITC",
              "While paused, find words that change on render-only passes only while a "
              "direction is held, and log the code changing them by pass type");
ITC_FLAG_STRING(menu_watch, "", "ITC",
                "Guest words (hex, comma-separated) to watch like --find_menu_selection's "
                "candidates, without its scan");

namespace debug {

struct Range {
  uint32_t start;
  uint32_t size;
};
std::vector<Range> ReadableGuestRanges(uint8_t* base);  // frame_state_diff.cpp

namespace {

constexpr auto kSampleInterval = std::chrono::milliseconds(300);
constexpr size_t kMaxCandidates = 64;
constexpr int kMaxChangeLines = 300;  // per 2 s

class MenuSelectionScan {
 public:
  void OnPassStart(uint8_t* base, bool render_only, bool paused, uint32_t listener) {
    render_only_ = render_only;
    before_.clear();
    if (!watch_parsed_) {
      watch_parsed_ = true;
      std::string list = ITC_FLAG(menu_watch);
      for (size_t start = 0; start < list.size();) {
        size_t end = list.find(',', start);
        std::string item = list.substr(start, end == std::string::npos ? end : end - start);
        if (!item.empty()) {
          candidates_.push_back(uint32_t(std::stoul(item, nullptr, 16)));
        }
        start = end == std::string::npos ? list.size() : end + 1;
      }
    }
    if (!ITC_FLAG(find_menu_selection)) {
      return;
    }
    if (sampling_) {
      Compare(base);
      sampling_ = false;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ >= std::chrono::seconds(2)) {
      last_log_ = now;
      change_lines_ = 0;
    }
    if (!paused || !render_only || listener == 0 || now - last_sample_ < kSampleInterval) {
      return;
    }
    last_sample_ = now;
    held_ = guest::LoadU32(base, listener + 0x180) != 0 ||
            guest::LoadU32(base, listener + 0x190) != 0;
    ranges_ = ReadableGuestRanges(base);
    snapshot_.clear();
    for (const Range& r : ranges_) {
      snapshot_.insert(snapshot_.end(), base + r.start, base + r.start + r.size);
    }
    sampling_ = true;
  }

  bool Active() const { return !candidates_.empty(); }

  void Before(uint8_t* base) {
    if (candidates_.empty()) {
      return;
    }
    std::vector<uint32_t> values(candidates_.size());
    for (size_t i = 0; i < candidates_.size(); i++) {
      values[i] = guest::LoadU32(base, candidates_[i]);
    }
    before_.push_back(std::move(values));
  }

  void After(uint8_t* base, const char* kind, uint32_t id) {
    if (before_.empty()) {
      return;
    }
    std::vector<uint32_t> values = std::move(before_.back());
    before_.pop_back();
    for (size_t i = 0; i < values.size() && i < candidates_.size(); i++) {
      uint32_t now = guest::LoadU32(base, candidates_[i]);
      if (now != values[i] && change_lines_ < kMaxChangeLines) {
        change_lines_++;
        ITC_LOG_INFO("menu selection: pass {} {} {} {:08X} changed {:08X}: {} -> {}",
                     frame_pacing::GameMessages_PassNumber(),
                     render_only_ ? "render-only" : "logic", kind, id, candidates_[i], values[i],
                     now);
      }
    }
  }

 private:
  void Compare(uint8_t* base) {
    size_t offset = 0;
    int changed = 0;
    for (const Range& r : ranges_) {
      const uint8_t* now = base + r.start;
      const uint8_t* then = snapshot_.data() + offset;
      offset += r.size;
      for (uint32_t block = 0; block < r.size; block += 4096) {
        uint32_t block_size = std::min<uint32_t>(4096, r.size - block);
        if (std::memcmp(now + block, then + block, block_size) == 0) {
          continue;
        }
        for (uint32_t i = block; i + 4 <= block + block_size; i += 4) {
          if (std::memcmp(now + i, then + i, 4) == 0) {
            continue;
          }
          changed++;
          uint32_t address = r.start + i;
          uint32_t before = __builtin_bswap32(*reinterpret_cast<const uint32_t*>(then + i));
          uint32_t after = guest::LoadU32(base, address);
          Hits& hits = hits_[address];
          if (!held_) {
            hits.idle++;
            continue;
          }
          int64_t step = int64_t(after) - int64_t(before);
          if (before < 256 && after < 256 && step >= -8 && step <= 8) {
            hits.held++;
            hits.before = before;
            hits.after = after;
          }
        }
      }
    }
    held_samples_ += held_ ? 1 : 0;
    idle_samples_ += held_ ? 0 : 1;
    // Candidates: changed by a small step in at least one held sample and
    // never in an idle one, once both kinds have been sampled.
    std::vector<std::pair<int, uint32_t>> ranked;
    if (idle_samples_ >= 2) {
      for (const auto& [address, hits] : hits_) {
        if (hits.held > 0 && hits.idle == 0) {
          ranked.push_back({hits.held, address});
        }
      }
    }
    std::sort(ranked.rbegin(), ranked.rend());
    std::vector<uint32_t> candidates;
    std::string list;
    for (const auto& [n, address] : ranked) {
      if (candidates.size() == kMaxCandidates) {
        break;
      }
      candidates.push_back(address);
      const Hits& hits = hits_[address];
      list += fmt::format(" {:08X}x{}({}->{})", address, n, hits.before, hits.after);
    }
    ITC_LOG_INFO(
        "menu selection: sample ({}): {} words changed; {} held and {} idle samples; "
        "{} candidates:{}",
        held_ ? "held" : "idle", changed, held_samples_, idle_samples_, ranked.size(), list);
    candidates_ = std::move(candidates);
    if (hits_.size() > 4000000) {
      hits_.clear();
    }
  }

  struct Hits {
    int held = 0, idle = 0;
    uint32_t before = 0, after = 0;  // the last held change
  };
  bool render_only_ = false;
  bool watch_parsed_ = false;
  bool sampling_ = false;
  bool held_ = false;
  int held_samples_ = 0, idle_samples_ = 0;
  int change_lines_ = 0;
  std::vector<Range> ranges_;
  std::vector<uint8_t> snapshot_;
  std::map<uint32_t, Hits> hits_;
  std::vector<uint32_t> candidates_;
  std::vector<std::vector<uint32_t>> before_;  // nested calls
  std::chrono::steady_clock::time_point last_sample_{}, last_log_{};
};

MenuSelectionScan& Get() {
  static MenuSelectionScan scan;
  return scan;
}

}  // namespace

void MenuSelectionScan_OnPassStart(uint8_t* base, bool render_only, bool paused,
                                   uint32_t listener) {
  Get().OnPassStart(base, render_only, paused, listener);
}

bool MenuSelectionScan_Active() {
  return Get().Active();
}

void MenuSelectionScan_Before(uint8_t* base) {
  Get().Before(base);
}

void MenuSelectionScan_After(uint8_t* base, const char* kind, uint32_t id) {
  Get().After(base, kind, id);
}

}  // namespace debug

// Wrapped to name them in --find_menu_selection's log: Scene::Update's two
// screen calls (slot 6 and slot 2 of the screen object at scene + 64), and
// others below.
#define ITC_MENU_SCAN_WRAP(address)                         \
  extern "C" REX_FUNC(sub_##address) {                      \
    REX_FUNC_PROLOGUE();                                    \
    debug::MenuSelectionScan_Before(base);                  \
    __imp__sub_##address(ctx, base);                        \
    debug::MenuSelectionScan_After(base, "call", 0x##address); \
  }
ITC_MENU_SCAN_WRAP(8258E5E0)
ITC_MENU_SCAN_WRAP(8258ED78)
// The callees of component update 82119118 (component 40066EE0 in the
// user's race), which posts the events the pause screen's Update consumes.
ITC_MENU_SCAN_WRAP(8259D1A8)
ITC_MENU_SCAN_WRAP(82094B10)
ITC_MENU_SCAN_WRAP(821184E8)
ITC_MENU_SCAN_WRAP(82118FB0)
ITC_MENU_SCAN_WRAP(82118C18)
#undef ITC_MENU_SCAN_WRAP

// The app's update (tick handler 82093DC0) runs its component lists through
// this: r4's low word is a component, whose vtable slot 1 it calls (r4 =
// the app + 20). Logged as "component <address>" with that slot's target
// (a delegate's, sub_8223A3F0 or sub_82233460, is at +8).
extern "C" REX_FUNC(sub_82092968) {
  REX_FUNC_PROLOGUE();
  if (!debug::MenuSelectionScan_Active()) {
    __imp__sub_82092968(ctx, base);
    return;
  }
  uint32_t component = ctx.r4.u32;
  uint32_t update = component ? guest::LoadU32(base, guest::LoadU32(base, component) + 4) : 0;
  if (update == 0x8223A3F0 || update == 0x82233460) {
    update = guest::LoadU32(base, component + 8);
  }
  debug::MenuSelectionScan_Before(base);
  __imp__sub_82092968(ctx, base);
  static thread_local std::string kind;
  kind = fmt::format("component {:08X}", component);
  debug::MenuSelectionScan_After(base, kind.c_str(), update);
}
