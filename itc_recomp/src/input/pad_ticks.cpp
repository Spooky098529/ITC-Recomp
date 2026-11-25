#include "pad_ticks.h"

#include "base/flags.h"
#include "base/log.h"
#include "guest.h"
#include "memory/mapping.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_STRING(pad_ticks_record, "", "ITC",
                "File to record every pad read into, a line a read (docs/pad-replay.md)");
ITC_FLAG_STRING(pad_ticks_play, "", "ITC",
                "File of recorded pad reads to answer the game's reads from, in order");

namespace input {
namespace {

// A line a read: user, result, packet number, buttons, the triggers, the
// sticks (lx, ly, rx, ry). A race entered is a line `race <n>`, and its
// start (the race world loaded: the course set-up's message 4, after the
// race's loads, whose length varies) a line `start <n>`.
struct Read {
  uint32_t result;
  XInputState state;
};

constexpr uint32_t kUsers = 4;

enum class Mark { kRace, kStart };

// One user's reads, with where each mark falls (the index of the first read
// after it), in order.
struct Track {
  std::vector<Read> reads;
  std::vector<size_t> marks;
  size_t next = 0;
  Read last{0x48F, {}};
};

std::mutex g_mutex;
bool g_loaded = false;
bool g_playing = false;
Track g_tracks[kUsers];
std::vector<Mark> g_kinds;  // the marks' kinds, in order
size_t g_marks = 0;         // marks the game has reached so far
size_t g_races = 0;         // races entered so far (recording)
size_t g_starts = 0;        // races started so far (recording)
bool g_end_logged = false;
FILE* g_record = nullptr;

void Load() {
  if (g_loaded) {
    return;
  }
  g_loaded = true;
  const std::string& play = ITC_FLAG(pad_ticks_play);
  if (!play.empty()) {
    FILE* f = std::fopen(play.c_str(), "r");
    if (!f) {
      ITC_LOG_ERROR("pad ticks: can't open {}", play);
    } else {
      char line[160];
      size_t count = 0;
      while (std::fgets(line, sizeof(line), f)) {
        unsigned user, result, packet, buttons, lt, rt, race;
        int lx, ly, rx, ry;
        const bool is_race = std::sscanf(line, "race %u", &race) == 1;
        if (is_race || std::sscanf(line, "start %u", &race) == 1) {
          g_kinds.push_back(is_race ? Mark::kRace : Mark::kStart);
          for (Track& t : g_tracks) {
            t.marks.push_back(t.reads.size());
          }
          continue;
        }
        if (std::sscanf(line, "%u %x %u %x %u %u %d %d %d %d", &user, &result, &packet, &buttons,
                        &lt, &rt, &lx, &ly, &rx, &ry) != 10 ||
            user >= kUsers) {
          continue;
        }
        g_tracks[user].reads.push_back(
            {result, {packet, {uint16_t(buttons), uint8_t(lt), uint8_t(rt), int16_t(lx),
                               int16_t(ly), int16_t(rx), int16_t(ry)}}});
        count++;
      }
      std::fclose(f);
      g_playing = true;
      ITC_LOG_INFO("pad ticks: playing {} reads ({} for user 0, {} marks) from {}", count,
                   g_tracks[0].reads.size(), g_tracks[0].marks.size(), play);
    }
  }
  const std::string& record = ITC_FLAG(pad_ticks_record);
  if (!record.empty()) {
    g_record = std::fopen(record.c_str(), "w");
    if (!g_record) {
      ITC_LOG_ERROR("pad ticks: can't write {}", record);
    } else {
      ITC_LOG_INFO("pad ticks: recording into {}", record);
    }
  }
}

}  // namespace

bool PadTicks_Playing() {
  std::lock_guard lock(g_mutex);
  Load();
  return g_playing;
}

uint32_t PadTicks_Next(uint32_t user, XInputState* state) {
  std::lock_guard lock(g_mutex);
  Load();
  if (user >= kUsers) {
    return 0x48F;
  }
  Track& t = g_tracks[user];
  // At the next mark before the game has reached it: the last read again,
  // until it does.
  const bool held = g_marks < t.marks.size() && t.next >= t.marks[g_marks];
  if (!held) {
    if (t.next < t.reads.size()) {
      t.last = t.reads[t.next++];
    } else if (user == 0 && !g_end_logged) {
      g_end_logged = true;
      ITC_LOG_INFO("pad ticks: the recording ended after {} reads", t.next);
    }
  }
  if (state) {
    *state = t.last.state;
  }
  return t.last.result;
}

void PadTicks_Record(uint32_t user, uint32_t result, const XInputState& state) {
  std::lock_guard lock(g_mutex);
  Load();
  if (!g_record) {
    return;
  }
  const XInputGamepad& g = state.gamepad;
  std::fprintf(g_record, "%u %x %u %x %u %u %d %d %d %d\n", user, result, state.packet_number,
               g.buttons, g.left_trigger, g.right_trigger, g.thumb_lx, g.thumb_ly, g.thumb_rx,
               g.thumb_ry);
  std::fflush(g_record);
}

namespace {

// The game reached a mark: recorded, or the playback lined up on it (reads
// left before it, the game having got there sooner, skipped). A file with
// no mark of this kind next (one recorded before start marks) is left as it
// was.
void OnMark(Mark kind, size_t n, const char* what) {
  if (g_record) {
    std::fprintf(g_record, "%s %zu\n", kind == Mark::kRace ? "race" : "start", n);
    std::fflush(g_record);
  }
  if (!g_playing || g_marks >= g_kinds.size() || g_kinds[g_marks] != kind) {
    return;
  }
  const size_t skipped = g_tracks[0].marks[g_marks] - std::min(g_tracks[0].next,
                                                                g_tracks[0].marks[g_marks]);
  for (Track& t : g_tracks) {
    if (g_marks < t.marks.size() && t.next < t.marks[g_marks]) {
      t.next = t.marks[g_marks];
    }
  }
  ITC_LOG_INFO("pad ticks: race {} {}, {} reads skipped to line up", n, what, skipped);
  g_marks++;
}

}  // namespace

void PadTicks_OnRaceEntered() {
  std::lock_guard lock(g_mutex);
  Load();
  OnMark(Mark::kRace, g_races++, "entered");
}

void PadTicks_OnRaceStarted() {
  std::lock_guard lock(g_mutex);
  Load();
  const size_t n = g_starts;
  OnMark(Mark::kStart, g_starts++, "started");
  // The game's own random generator (sub_82293568's seed) lined up at each
  // race's start too, as the reads are: the camera shake draws from it on
  // every tick of the race's loading, whose length varies from run to run
  // and build to build, so the draws after it (the next race's traffic)
  // would otherwise move with the load times. Recording and playback alike.
  if (g_playing || g_record) {
    constexpr uint32_t kGameRandSeed = 0x82638188;  // race-overlays.toml's rand_seed
    guest::StoreU32(kGameRandSeed, 0x1234567u + uint32_t(n));
  }
}

}  // namespace input
