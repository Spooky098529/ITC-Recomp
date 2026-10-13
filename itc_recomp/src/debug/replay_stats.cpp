#include "replay_stats.h"

#include "guest.h"
#include <array>
#include <map>
#include <set>
#include <vector>
#include <rex/hash.h>
#include <tuple>
#include <chrono>
#include <string>
#include <rex/logging.h>
#include <rex/cvar.h>

REXCVAR_DEFINE_BOOL(log_replay_commands, false, "ITC",
                    "Log how often each render command is replayed per frame, and sample "
                    "arguments of the unidentified ones, every 2 s");

namespace {

constexpr uint32_t kCommandCount = 38;

// The graphics library's device (see docs/gpu-functions.md).
constexpr uint32_t kDevicePointer = 0x8296BD90;
constexpr uint32_t kRenderStateSetters = 56;    // device + 56 + state
constexpr uint32_t kSamplerStateSetters = 440;  // device + 440 + state

// A render or sampler state id seen, with its setter and values.
struct StateUse {
  uint32_t setter = 0;
  uint32_t last_value = 0;
  uint32_t distinct_values = 0;
  uint64_t count = 0;
};

// Commands not identified from the code yet (docs/gpu-functions.md).
constexpr bool IsUnknown(uint32_t id) {
  switch (id) {
    case 3: case 8: case 9: case 10: case 12: case 20: case 22: case 28: case 32: case 33:
      return true;
    default:
      return false;
  }
}

struct Stats {
  std::array<uint64_t, kCommandCount> commands{};  // calls
  std::array<uint64_t, kCommandCount> items{};     // sum of arg1
  std::array<std::string, kCommandCount> sample;   // last arguments seen
  uint32_t frames = 0;
  // Kept for the whole run; logged when something new turns up.
  std::map<uint32_t, StateUse> render_states, sampler_states;
  std::map<std::tuple<uint32_t, uint32_t, uint32_t>, uint64_t> texture_fetches;
  bool new_states = false;
  std::set<std::pair<uint32_t, uint64_t>> shaders;  // (object, microcode hash)
  std::set<uint32_t> declarations, index_buffers;
  // Frame pass trace: one whole frame every 10 s.
  bool tracing = false;
  uint32_t trace_draws = 0;  // draws since the last traced command
  std::chrono::steady_clock::time_point last_trace = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point last_log = std::chrono::steady_clock::now();
};

// Render thread only.
Stats& GetStats() {
  static Stats stats;
  return stats;
}

std::string Words(uint8_t* base, uint32_t address, uint32_t count) {
  std::string words;
  for (uint32_t i = 0; i < count; i++) {
    words += fmt::format(" {:08X}", guest::LoadU32(base, address + 4 * i));
  }
  return words;
}

// A surface: +24 surface info (pitch, MSAA), +28 colour/depth info (EDRAM
// base, format), +32 HiZ (depth).
std::string Surface(uint8_t* base, uint32_t surface) {
  if (!surface) {
    return "none";
  }
  return fmt::format("{:08X} [info{}]", surface, Words(base, surface + 24, 3));
}

// A texture: its fetch constant (+28..+48).
std::string Texture(uint8_t* base, uint32_t texture) {
  if (!texture) {
    return "none";
  }
  return fmt::format("{:08X} [fetch{}]", texture, Words(base, texture + 28, 6));
}

// One line per pass-structure command of a traced frame.
void TraceCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1, uint32_t arg2) {
  Stats& s = GetStats();
  if (id == 17 || id == 18 || id == 19 || id == 15 || id == 27) {
    s.trace_draws++;
    return;
  }
  std::string what;
  switch (id) {
    case 23:
      for (uint32_t i = 0; i < arg1; i++) {
        what += fmt::format(" rt{}={}", guest::LoadU32(base, payload + 16 * i),
                            Surface(base, guest::LoadU32(base, payload + 16 * i + 4)));
      }
      break;
    case 24:
      for (uint32_t i = 0; i < arg1; i++) {
        what += " depth=" + Surface(base, guest::LoadU32(base, payload + 16 * i));
      }
      break;
    case 25:
      for (uint32_t i = 0; i < arg1; i++) {
        uint32_t item = payload + 68 * i;
        uint32_t present = guest::LoadU32(base, item);
        what += fmt::format(" [flags {:08X}", guest::LoadU32(base, item + 4));
        if (present & 1) {
          what += " rect" + Words(base, item + 8, 4);
        }
        if (present & 2) {
          what += " dest " + Texture(base, guest::LoadU32(base, item + 24));
        }
        if (present & 4) {
          what += " point" + Words(base, item + 28, 2);
        }
        what += fmt::format(" level {} slice {}", guest::LoadU32(base, item + 36),
                            guest::LoadU32(base, item + 40));
        if (present & 8) {
          what += fmt::format(" clear colour {:08X}", guest::LoadU32(base, item + 44));
        }
        what += fmt::format(" z {:08X} stencil {}]", guest::LoadU32(base, item + 48),
                            guest::LoadU32(base, item + 52));
      }
      break;
    case 26:
      for (uint32_t i = 0; i < arg1; i++) {
        uint32_t item = payload + 280 * i;
        what += fmt::format(" [rects {} flags {:08X} colour {:08X} z {:08X} stencil {}]",
                            guest::LoadU32(base, item + 4), guest::LoadU32(base, item + 8),
                            guest::LoadU32(base, item + 268), guest::LoadU32(base, item + 272),
                            guest::LoadU32(base, item + 276));
      }
      break;
    case 29:
      what = fmt::format(" mask {:08X}", guest::LoadU32(base, payload));
      break;
    case 30:
      what = Words(base, payload, 4 * arg1);
      break;
    case 34:
    case 35:
      break;
    default:
      return;
  }
  REXLOG_INFO("frame trace: {} draws, then command {}:{}", s.trace_draws, id, what);
  s.trace_draws = 0;
}

}  // namespace

void debug::ReplayStats_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                                  uint32_t arg2) {
  if (!REXCVAR_GET(log_replay_commands) || id >= kCommandCount) {
    return;
  }
  Stats& s = GetStats();
  if (s.tracing) {
    TraceCommand(base, id, payload, arg1, arg2);
  }
  s.commands[id]++;
  s.items[id] += arg1;
  if (id == 1 || id == 2) {
    // Items: [state, value] (render) or [sampler, state, value].
    uint32_t device = guest::LoadU32(base, kDevicePointer);
    for (uint32_t i = 0; i < arg1; i++) {
      uint32_t item = payload + 16 * i;
      uint32_t state = guest::LoadU32(base, item + (id == 1 ? 0 : 4));
      uint32_t value = guest::LoadU32(base, item + (id == 1 ? 4 : 8));
      auto& uses = id == 1 ? s.render_states : s.sampler_states;
      auto [it, added] = uses.try_emplace(state);
      StateUse& use = it->second;
      if (added) {
        use.setter = guest::LoadU32(
            base, device + (id == 1 ? kRenderStateSetters : kSamplerStateSetters) + state);
        use.last_value = value;
        use.distinct_values = 1;
        s.new_states = true;
      } else if (use.last_value != value && use.distinct_values < 1000) {
        use.distinct_values++;
        use.last_value = value;
      }
      use.count++;
    }
  }
  if (id == 4) {
    // Items: [sampler, texture]. The texture's fetch constant is +28..+48.
    for (uint32_t i = 0; i < arg1; i++) {
      uint32_t texture = guest::LoadU32(base, payload + 16 * i + 4);
      if (!texture) {
        continue;
      }
      auto key = std::make_tuple(guest::LoadU32(base, texture + 28),
                                 guest::LoadU32(base, texture + 32) & 0xFFF,
                                 guest::LoadU32(base, texture + 36));
      if (s.texture_fetches[key]++ == 0) {
        REXLOG_INFO("replay texture fetch (new): dword0 {:08X} dword1&FFF {:03X} dword2 {:08X}",
                    std::get<0>(key), std::get<1>(key), std::get<2>(key));
      }
    }
  }
  if (id == 8 || id == 9) {
    // [shader]. Vertex shader: microcode at +48, size in bytes at +600.
    // Pixel shader: microcode at +24, size at +64 (see docs/gpu-functions.md).
    uint32_t shader = guest::LoadU32(base, payload);
    if (shader) {
      bool vertex = id == 8;
      uint32_t ucode = guest::LoadU32(base, shader + (vertex ? 48 : 24));
      uint32_t size = guest::LoadU32(base, shader + (vertex ? 600 : 64));
      std::vector<uint8_t> bytes(size < 0x40000 ? size : 0);
      for (uint32_t i = 0; i < bytes.size(); i++) {
        bytes[i] = guest::LoadU8(base, ucode + i);
      }
      uint64_t hash = XXH3_64bits(bytes.data(), bytes.size());
      if (s.shaders.emplace(shader, hash).second) {
        std::string header;
        for (uint32_t i = 0; i < 20; i++) {
          header += fmt::format(" {:08X}", guest::LoadU32(base, shader + 4 * i));
        }
        REXLOG_INFO("replay {} shader (new) {:08X}: microcode {:08X}, {} bytes, hash {:016X}; "
                    "words 0-19:{}",
                    vertex ? "vertex" : "pixel", shader, ucode, size, hash, header);
      }
    }
  }
  if (id == 10 || id == 12) {
    // [declaration] or [index buffer]: log each new one's first words.
    uint32_t object = guest::LoadU32(base, payload);
    auto& seen = id == 10 ? s.declarations : s.index_buffers;
    if (object && seen.insert(object).second) {
      std::string words;
      for (uint32_t i = 0; i < 16; i++) {
        words += fmt::format(" {:08X}", guest::LoadU32(base, object + 4 * i));
      }
      REXLOG_INFO("replay {} (new) {:08X}: words 0-15:{}",
                  id == 10 ? "vertex declaration" : "index buffer", object, words);
    }
  }
  if (IsUnknown(id)) {
    std::string words;
    for (uint32_t i = 0; i < 8; i++) {
      words += fmt::format(" {:08X}", guest::LoadU32(base, payload + 4 * i));
    }
    s.sample[id] = fmt::format("arg1 {} arg2 {:08X} payload{}", arg1, arg2, words);
  }
}

void debug::ReplayStats_OnFrame() {
  if (!REXCVAR_GET(log_replay_commands)) {
    return;
  }
  Stats& s = GetStats();
  s.frames++;
  auto now = std::chrono::steady_clock::now();
  if (s.tracing) {
    REXLOG_INFO("frame trace: {} draws, then the frame ends", s.trace_draws);
    s.tracing = false;
  } else if (now - s.last_trace >= std::chrono::seconds(10)) {
    REXLOG_INFO("frame trace: starts");
    s.tracing = true;
    s.trace_draws = 0;
    s.last_trace = now;
  }
  if (now - s.last_log < std::chrono::seconds(2)) {
    return;
  }
  std::string line;
  for (uint32_t id = 0; id < kCommandCount; id++) {
    if (s.commands[id]) {
      line += fmt::format(" | {}: {:.1f} ({:.1f} items)", id, double(s.commands[id]) / s.frames,
                          double(s.items[id]) / s.frames);
    }
  }
  REXLOG_INFO("replay commands per frame ({} frames):{}", s.frames, line);
  for (uint32_t id = 0; id < kCommandCount; id++) {
    if (!s.sample[id].empty()) {
      REXLOG_INFO("replay command {} sample: {}", id, s.sample[id]);
    }
  }
  if (s.new_states) {
    for (auto& [name, uses] : {std::pair{"render", &s.render_states},
                               std::pair{"sampler", &s.sampler_states}}) {
      for (auto& [state, use] : *uses) {
        REXLOG_INFO("replay {} state {} (0x{:X}): setter {:08X}, {} uses, {} values, last {:08X}",
                    name, state, state, use.setter, use.count, use.distinct_values,
                    use.last_value);
      }
    }
    s.new_states = false;
  }
  s.commands = {};
  s.items = {};
  s.sample = {};
  s.frames = 0;
  s.last_log = now;
}

// Command handlers, wrapped to count them. Command 7 (sub_8258FD48) is
// counted in its wrapper in blend_state.cpp. Ids 0, 36 and 37
// (sub_82304258) aren't counted: that function is also used elsewhere.
#define REPLAY_COUNTED_HANDLER(address, id)                                         \
  extern "C" REX_FUNC(sub_##address) {                                              \
    REX_FUNC_PROLOGUE();                                                            \
    debug::ReplayStats_OnCommand(base, id, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);     \
    __imp__sub_##address(ctx, base);                                                \
  }

REPLAY_COUNTED_HANDLER(8258FBD0, 1)
REPLAY_COUNTED_HANDLER(8258FC30, 2)
REPLAY_COUNTED_HANDLER(8258FC90, 3)
REPLAY_COUNTED_HANDLER(8258FCE0, 4)
REPLAY_COUNTED_HANDLER(8258E9F8, 5)
REPLAY_COUNTED_HANDLER(8258EA20, 6)
REPLAY_COUNTED_HANDLER(8258FF50, 8)
REPLAY_COUNTED_HANDLER(8258FFA0, 9)
REPLAY_COUNTED_HANDLER(8258FFF0, 10)
REPLAY_COUNTED_HANDLER(82590040, 11)
REPLAY_COUNTED_HANDLER(825900B8, 12)
REPLAY_COUNTED_HANDLER(8258EA58, 13)  // also 14: one handler
REPLAY_COUNTED_HANDLER(82590108, 15)
REPLAY_COUNTED_HANDLER(825901B0, 16)
REPLAY_COUNTED_HANDLER(82590268, 17)
REPLAY_COUNTED_HANDLER(825902E0, 18)
REPLAY_COUNTED_HANDLER(82590360, 19)
REPLAY_COUNTED_HANDLER(825903D8, 20)
REPLAY_COUNTED_HANDLER(8258EAD0, 21)
REPLAY_COUNTED_HANDLER(82591798, 22)
REPLAY_COUNTED_HANDLER(82590428, 23)
REPLAY_COUNTED_HANDLER(82590478, 24)
REPLAY_COUNTED_HANDLER(825904C8, 25)
REPLAY_COUNTED_HANDLER(82590620, 26)
REPLAY_COUNTED_HANDLER(82590698, 27)
REPLAY_COUNTED_HANDLER(8258EAE0, 28)
REPLAY_COUNTED_HANDLER(82590768, 29)
REPLAY_COUNTED_HANDLER(82590860, 30)
REPLAY_COUNTED_HANDLER(825908E0, 31)
REPLAY_COUNTED_HANDLER(82590968, 32)
REPLAY_COUNTED_HANDLER(825909E8, 33)
REPLAY_COUNTED_HANDLER(82590A68, 34)
REPLAY_COUNTED_HANDLER(82590AD0, 35)
