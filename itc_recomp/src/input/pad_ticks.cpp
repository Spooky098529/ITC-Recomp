#include "pad_ticks.h"

#include "base/flags.h"
#include "base/log.h"

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
// sticks (lx, ly, rx, ry). A race's start is a line `race <n>`.
struct Read {
  uint32_t result;
  XInputState state;
};

constexpr uint32_t kUsers = 4;

// One user's reads, with where each race's mark falls (the index of the
// first read after it).
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
size_t g_races = 0;  // races entered so far
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
        if (std::sscanf(line, "race %u", &race) == 1) {
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
      ITC_LOG_INFO("pad ticks: playing {} reads ({} for user 0, {} races) from {}", count,
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
  // At the next race's mark before the game has entered that race: the last
  // read again, until it does.
  const bool held = g_races < t.marks.size() && t.next >= t.marks[g_races];
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

void PadTicks_OnRaceEntered() {
  std::lock_guard lock(g_mutex);
  Load();
  if (g_record) {
    std::fprintf(g_record, "race %zu\n", g_races);
    std::fflush(g_record);
  }
  if (g_playing && g_races < g_tracks[0].marks.size()) {
    // Reads left before the mark (the game got here sooner) are skipped.
    const size_t skipped = g_tracks[0].marks[g_races] - std::min(g_tracks[0].next,
                                                                  g_tracks[0].marks[g_races]);
    for (Track& t : g_tracks) {
      if (g_races < t.marks.size() && t.next < t.marks[g_races]) {
        t.next = t.marks[g_races];
      }
    }
    ITC_LOG_INFO("pad ticks: race {} entered, {} reads skipped to line up", g_races, skipped);
  }
  g_races++;
}

}  // namespace input
