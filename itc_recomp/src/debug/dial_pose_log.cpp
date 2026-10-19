// Logs the cockpit needles' draws (--log_dial_pose=<file>), to find the code
// that poses the second needle (r31 ...0130), which render-only passes draw
// at a fixed rest pose (docs/cockpit-dial-interpolation.md, "Finding the code
// that poses the needle").
//
// sub_820D04E8 draws a car part: r3 = the part, r4 = the camera, r5 = its
// world matrix (on the caller's stack), r6 = the part's index. For parts whose
// address ends in one of --log_dial_pose_parts' 16-bit suffixes, each call
// records the pass, the caller, the arguments, the world matrix and the
// part's first 0x200 bytes: all of them on the part's first draw, then only
// the words that changed since its last draw. Written to the file every 5 s.

#include "frame_pacing/game_messages.h"
#include "guest.h"

#include <rex/cvar.h>

#include <chrono>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_STRING(log_dial_pose, "", "ITC",
                      "Write the cockpit needles' draws (sub_820D04E8: pass, caller, world "
                      "matrix, the part's changed words) to this file");
REXCVAR_DEFINE_STRING(log_dial_pose_parts, "0130,06A0", "ITC",
                      "--log_dial_pose: the parts' addresses' low 16 bits, in hex, comma-separated");

namespace debug {
namespace {

constexpr uint32_t kPartWords = 0x200 / 4;

struct DialPoseLog {
  std::vector<uint32_t> suffixes;
  bool parsed = false;
  std::unordered_map<uint32_t, std::vector<uint32_t>> last;  // part -> its words at its last draw
  std::string pending;
  std::chrono::steady_clock::time_point written = std::chrono::steady_clock::now();

  bool Wanted(uint32_t part) {
    if (!parsed) {
      parsed = true;
      const std::string& list = REXCVAR_GET(log_dial_pose_parts);
      size_t at = 0;
      while (at < list.size()) {
        size_t end = list.find(',', at);
        if (end == std::string::npos) {
          end = list.size();
        }
        if (end > at) {
          suffixes.push_back(uint32_t(std::stoul(list.substr(at, end - at), nullptr, 16)));
        }
        at = end + 1;
      }
    }
    for (uint32_t s : suffixes) {
      if ((part & 0xFFFF) == s) {
        return true;
      }
    }
    return false;
  }

  void Note(uint8_t* base, uint32_t lr, uint32_t part, uint32_t camera, uint32_t world,
            uint32_t index, uint32_t r7) {
    char line[512];
    std::snprintf(line, sizeof(line), "pass %llu %s caller %08X part %08X camera %08X index %u r7 %u world",
                  (unsigned long long)frame_pacing::GameMessages_PassNumber(),
                  frame_pacing::GameMessages_IsRenderOnlyPass() ? "render-only" : "logic", lr, part,
                  camera, index, r7);
    pending += line;
    for (int i = 0; i < 16; i++) {
      std::snprintf(line, sizeof(line), " %.4f", guest::LoadFloat(base, world + 4 * i));
      pending += line;
    }
    std::vector<uint32_t> words(kPartWords);
    for (uint32_t i = 0; i < kPartWords; i++) {
      words[i] = guest::LoadU32(base, part + 4 * i);
    }
    auto it = last.find(part);
    pending += it == last.end() ? " part (all)" : " part changed";
    for (uint32_t i = 0; i < kPartWords; i++) {
      if (it == last.end() || it->second[i] != words[i]) {
        std::snprintf(line, sizeof(line), " +%03X=%08X", 4 * i, words[i]);
        pending += line;
      }
    }
    pending += '\n';
    last[part] = std::move(words);
    Flush();
  }

  void Flush() {
    auto now = std::chrono::steady_clock::now();
    if (now - written >= std::chrono::seconds(5)) {
      written = now;
      if (std::FILE* file = std::fopen(REXCVAR_GET(log_dial_pose).c_str(), "a")) {
        std::fwrite(pending.data(), 1, pending.size(), file);
        std::fclose(file);
      }
      pending.clear();
    }
  }
};

DialPoseLog& GetDialPoseLog() {
  static DialPoseLog log;
  return log;
}

}  // namespace
}  // namespace debug

// A car part's draw: r3 = the part, r4 = camera, r5 = world matrix, r6 = index.
extern "C" REX_FUNC(sub_820D04E8) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(log_dial_pose).empty()) [[unlikely]] {
    debug::DialPoseLog& log = debug::GetDialPoseLog();
    if (log.Wanted(ctx.r3.u32)) {
      log.Note(base, uint32_t(ctx.lr), ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
    }
  }
  __imp__sub_820D04E8(ctx, base);
}

namespace {
const char* PassType() {
  return frame_pacing::GameMessages_IsRenderOnlyPass() ? "render-only" : "logic";
}
}  // namespace

// The cockpit dials' posing (sub_820E45A8, one caller):
// r3 = the dash, r4 = the car. Its needles' angles come from sub_820D3688
// unless [dash + 4] is 5.
extern "C" REX_FUNC(sub_820E45A8) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(log_dial_pose).empty()) [[unlikely]] {
    char line[256];
    uint32_t dash = ctx.r3.u32;
    std::snprintf(line, sizeof(line),
                  "pass %llu %s dials dash %08X car %08X r5 %08X mode %u ranges %08X %08X\n",
                  (unsigned long long)frame_pacing::GameMessages_PassNumber(), PassType(), dash,
                  ctx.r4.u32, ctx.r5.u32, guest::LoadU32(base, dash + 4),
                  guest::LoadU32(base, dash + 5328), guest::LoadU32(base, dash + 5332));
    debug::GetDialPoseLog().pending += line;
  }
  __imp__sub_820E45A8(ctx, base);
}

// A dial's angle from a car value: r3 = its range (from, to, angle from,
// angle to), r4 = which value (car + 704, 712, 716 or 720), r5 = the car.
extern "C" REX_FUNC(sub_820D3688) {
  REX_FUNC_PROLOGUE();
  uint32_t lr = uint32_t(ctx.lr);
  bool log = !REXCVAR_GET(log_dial_pose).empty() && lr >= 0x820E45A8 && lr < 0x820E4A00;
  uint32_t range = ctx.r3.u32, kind = ctx.r4.u32, car = ctx.r5.u32;
  __imp__sub_820D3688(ctx, base);
  if (log) [[unlikely]] {
    char line[320];
    static constexpr uint32_t kOffsets[4] = {704, 712, 716, 720};
    float value = range && kind < 4 ? guest::LoadFloat(base, car + kOffsets[kind]) : 0.0f;
    std::snprintf(line, sizeof(line),
                  "pass %llu %s angle caller %08X range %08X kind %u car %08X value %.4f "
                  "car704 %.4f 712 %.4f 716 %.4f 720 %.4f angle %.4f\n",
                  (unsigned long long)frame_pacing::GameMessages_PassNumber(), PassType(), lr,
                  range, kind, car, value, guest::LoadFloat(base, car + 704),
                  guest::LoadFloat(base, car + 712), guest::LoadFloat(base, car + 716),
                  guest::LoadFloat(base, car + 720), ctx.f1.f64);
    debug::DialPoseLog& dial_log = debug::GetDialPoseLog();
    dial_log.pending += line;
    dial_log.Flush();
  }
}
