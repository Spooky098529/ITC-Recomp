// The glow (light bloom) between ticks. See "The bloom" under Known issues in
// docs/native-renderer.md.
//
// Each view's post-processing object records its glow chain in
// sub_821379D8 (vtable slot 0, called by the draw step part sub_82133960).
// The chain keeps a 256x144 history: a cross-fade blends it, weighted by
// +8460, over the frame's glow (history = w * history + (1 - w) * current),
// and the result is resolved back into it only when +48 is set (and +44
// isn't). +48 is set by the object's update and cleared after each draw, so
// with render-only passes the history moved at 30 Hz while the scene moved
// every frame. Its pixel constant c33 (+8436 or +8440, +8452 the glow
// strength, +8456, +8476) also changes once a tick.
//
// Around the recorder, on every pass of a race: those fields and the weight
// are blended between the last two ticks; on render-only passes +48 is set
// as the last logic pass had it, so the history is written every frame; and
// the weight is raised to the pass's share of a tick, so the history fades
// as fast in time as at 30 Hz. Everything is put back afterwards.

#include "frame_pacing/frame_pacer.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"

#include "base/flags.h"
#include "base/log.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, interpolate_draw_state);
ITC_FLAG_BOOL(blend_glow, true, "ITC",
              "Blend the glow's strength and history weight between ticks");
ITC_FLAG_BOOL(glow_history_every_frame, true, "ITC",
              "Write the glow's history every frame, its weight scaled to the frame's "
              "share of a tick, instead of once a tick");
ITC_FLAG_BOOL(log_glow_recorder, false, "ITC",
              "Log the glow recorder's history flags and weights, every 2 s");

namespace {

using Clock = std::chrono::steady_clock;

// The post-processing object's fields (sub_821379D8, set up by
// sub_82138610).
constexpr uint32_t kReset = 40;           // the history is reset: weight unused
constexpr uint32_t kNoHistory = 44;       // set: no history resolve
constexpr uint32_t kWriteHistory = 48;    // set: the history resolve is recorded
constexpr uint32_t kHistoryWeight = 8460;
// Floats changing once a tick: c33's components and the history weight.
constexpr std::array<uint32_t, 6> kBlended = {8436, 8440, 8452, 8456, 8476, kHistoryWeight};

struct Glow {
  std::array<float, kBlended.size()> before{}, now{};
  bool have_before = false;
  uint32_t write_history = 0;  // +48 on the last logic pass
  Clock::time_point last_record{};
};
std::unordered_map<uint32_t, Glow> g_glows;  // by object

struct GlowLog {
  uint64_t logic = 0, render_only = 0, logic_writes = 0, forced_writes = 0, resets = 0;
  float weight_min = 1, weight_max = 0, tick_weight = 0, share_min = 1, share_max = 0;
  Clock::time_point last{};
} g_log;

void LogGlow() {
  auto now = Clock::now();
  if (now - g_log.last < std::chrono::seconds(2)) {
    return;
  }
  ITC_LOG_INFO("glow recorder: logic passes {} (writing the history {}), render-only {} "
               "(history write added {}), resets {}; tick weight {:.4f}, weights used "
               "{:.4f}-{:.4f}, pass shares of a tick {:.3f}-{:.3f}",
               g_log.logic, g_log.logic_writes, g_log.render_only, g_log.forced_writes,
               g_log.resets, g_log.tick_weight, g_log.weight_min, g_log.weight_max,
               g_log.share_min, g_log.share_max);
  g_log = {};
  g_log.last = now;
}

}  // namespace

extern "C" REX_FUNC(sub_821379D8) {
  REX_FUNC_PROLOGUE();
  bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
  if (!ITC_FLAG(interpolate_draw_state) || !frame_pacing::GameMessages_InRace()) {
    __imp__sub_821379D8(ctx, base);
    return;
  }
  uint32_t object = ctx.r3.u32;
  Glow& glow = g_glows[object];
  auto now = Clock::now();
  bool log = ITC_FLAG(log_glow_recorder);
  if (!render_only) {
    // The object's update has set this tick's values.
    glow.before = glow.now;
    glow.have_before = glow.last_record != Clock::time_point{};
    for (size_t i = 0; i < kBlended.size(); i++) {
      glow.now[i] = guest::LoadFloat(base, object + kBlended[i]);
    }
    glow.write_history = guest::LoadU32(base, object + kWriteHistory);
  }
  std::vector<std::pair<uint32_t, uint32_t>> saved;
  auto replace = [&](uint32_t offset, uint32_t value) {
    saved.emplace_back(offset, guest::LoadU32(base, object + offset));
    guest::StoreU32(base, object + offset, value);
  };
  auto replace_float = [&](uint32_t offset, float value) {
    uint32_t bits;
    std::memcpy(&bits, &value, 4);
    replace(offset, bits);
  };

  bool reset = guest::LoadU32(base, object + kReset) != 0;
  float amount = frame_pacing::GetFramePacer().TickProgress();
  float weight = guest::LoadFloat(base, object + kHistoryWeight);
  if (ITC_FLAG(blend_glow) && glow.have_before && !reset) {
    for (size_t i = 0; i < kBlended.size(); i++) {
      float a = glow.before[i], b = glow.now[i];
      if (!std::isfinite(a) || !std::isfinite(b)) {
        continue;
      }
      float value = a + (b - a) * amount;
      replace_float(kBlended[i], value);
      if (kBlended[i] == kHistoryWeight) {
        weight = value;
      }
    }
  }
  if (ITC_FLAG(glow_history_every_frame) && glow.write_history) {
    // This pass's share of a tick: the time since the object's last pass.
    float share = 1.0f;
    if (glow.last_record != Clock::time_point{}) {
      share = std::clamp(std::chrono::duration<float>(now - glow.last_record).count() /
                             std::chrono::duration<float>(
                                 frame_pacing::FramePacer::kTickPeriod).count(),
                         0.0f, 1.0f);
    }
    if (render_only && !guest::LoadU32(base, object + kWriteHistory)) {
      replace(kWriteHistory, glow.write_history);
      g_log.forced_writes += log;
    }
    if (std::isfinite(weight) && weight > 0.0f && weight < 1.0f) {
      float scaled = std::pow(weight, share);
      replace_float(kHistoryWeight, scaled);
      if (log) {
        g_log.tick_weight = weight;
        g_log.weight_min = std::min(g_log.weight_min, scaled);
        g_log.weight_max = std::max(g_log.weight_max, scaled);
        g_log.share_min = std::min(g_log.share_min, share);
        g_log.share_max = std::max(g_log.share_max, share);
      }
    }
  }
  glow.last_record = now;
  if (log) {
    (render_only ? g_log.render_only : g_log.logic)++;
    g_log.logic_writes += !render_only && glow.write_history;
    g_log.resets += reset;
    LogGlow();
  }

  __imp__sub_821379D8(ctx, base);

  for (auto it = saved.rbegin(); it != saved.rend(); ++it) {
    guest::StoreU32(base, object + it->first, it->second);
  }
}
