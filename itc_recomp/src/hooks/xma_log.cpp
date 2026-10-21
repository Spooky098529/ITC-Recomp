// TEMPORARY (rexglue-removal B2, docs/rexglue-removal.md): counts the audio
// library's XMA layer (the context helpers at 8245C638-8245D4D0, which read
// and write the XMA decoder's registers and contexts, and the source voice
// code at 8246A5F8-8246C320 that calls them; docs/functions/audio.md), to find
// which run, how often, from where and what the contexts hold, before XMA is
// decoded by our own code calling FFmpeg. Removed when the survey is done (a
// kept wrapper would block a real override).
//
// --log_xma logs each function's first calls and every power of two (caller,
// r3-r10 in, r3 out) and every 10 s the counts of those called. Functions
// taking (object r3, stream r4) also log the object's flags (+4) and the
// stream's 96-byte entry (object +8, 96 bytes a stream) before and after:
// an XMA context (64 bytes; or, with flag 0x20000 clear, +64 points to the
// hardware context, logged too) and the library's fields after it.
// sub_8245D288 (the hardware wait) is overridden in hooks/clock.cpp and isn't
// wrapped; its calls to sub_8245D350 and sub_8245CE98 are counted.

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <bit>
#include <chrono>
#include <mutex>
#include <string>

REXCVAR_DEFINE_BOOL(log_xma, false, "ITC",
                    "Log calls to the audio library's XMA layer (temporary, for rexglue-removal "
                    "B2)");

namespace {

struct In {
  uint32_t lr, r[8];  // r3..r10
};

using Extra = std::string (*)(uint8_t* base, const In& in);

std::string None(uint8_t*, const In&) {
  return {};
}

std::string Words(uint8_t* base, uint32_t address, uint32_t count) {
  std::string s;
  for (uint32_t i = 0; i < count; i++) {
    s += fmt::format("{}{:08X}", i ? " " : "", guest::LoadU32(base, address + 4 * i));
  }
  return s;
}

// r3 = the object, r4 = a stream: its flags and the stream's entry.
std::string Stream(uint8_t* base, const In& in) {
  const uint32_t object = in.r[0];
  if (object < 0x10000 || in.r[1] > 64) {
    return {};
  }
  const uint32_t flags = guest::LoadU32(base, object + 4);
  const uint32_t entry = guest::LoadU32(base, object + 8) + 96 * in.r[1];
  std::string s = fmt::format("; flags {:08X} entry {:08X}: {}", flags, entry,
                              Words(base, entry, 24));
  const uint32_t hardware = guest::LoadU32(base, entry + 64);
  if (!(flags & 0x20000) && hardware >= 0x10000) {
    s += fmt::format("; hardware {:08X}: {}", hardware, Words(base, hardware, 16));
  }
  return s;
}

// r3 = the object: its first 16 words.
std::string Object(uint8_t* base, const In& in) {
  return in.r[0] >= 0x10000 ? "; object " + Words(base, in.r[0], 16) : std::string();
}

struct Function {
  uint32_t address;
  const char* name;
  Extra extra;
  std::atomic<uint64_t> calls{0};
};

#define XMA_LAYER(X)                                       \
  X(8245B808, "category volume (mixer)", None)             \
  X(8245C638, "reads 7FEA1800", None)                      \
  X(8245C650, "streams set up (makes contexts)", Object)   \
  X(8245C880, "streams released", Object)                  \
  X(8245C8D0, "XMAReleaseContext caller", Object)          \
  X(8245C948, "flags bit (+4)", None)                      \
  X(8245C958, "stream set up?", Stream)                    \
  X(8245CA30, "input buffer is r5?", Stream)               \
  X(8245CA90, "context state test", Stream)                \
  X(8245CAE0, "no input buffer valid?", Stream)            \
  X(8245CB38, "helper CB38", Stream)                       \
  X(8245CBB0, "helper CBB0", Stream)                       \
  X(8245CD68, "helper CD68", Stream)                       \
  X(8245CE10, "output read offset advance?", Stream)       \
  X(8245CE98, "context reset", Stream)                     \
  X(8245CF60, "output read offset", Stream)                \
  X(8245CF98, "set input buffer", Stream)                  \
  X(8245D010, "output write offset?", Stream)              \
  X(8245D050, "entry +76", Stream)                         \
  X(8245D070, "helper D070", Stream)                       \
  X(8245D0E0, "set output buffer", Stream)                 \
  X(8245D118, "D118", None)                                \
  X(8245D138, "XMACreateContext caller", Object)           \
  X(8245D208, "register write D208", Object)               \
  X(8245D350, "hardware answered?", Object)                \
  X(8245D4D0, "register write D4D0", Object)               \
  X(8246A5F8, "source voice A5F8", Object)                        \
  X(8246A698, "source voice A698", Object)                        \
  X(8246A758, "source voice release streams", Object)             \
  X(8246AEE0, "source voice set up streams", Object)              \
  X(8246B0C8, "source voice B0C8", Object)                        \
  X(8246B1C8, "source voice B1C8", Object)                        \
  X(8246B480, "source voice submit?", Object)                     \
  X(8246BB98, "source voice BB98", Object)                        \
  X(8246BE20, "source voice makes contexts", Object)              \
  X(8246BFD8, "source voice releases contexts", Object)           \
  X(8246C320, "source voice update", Object)

#define ENTRY(address, name, extra) {0x##address, #address " " name, extra},
Function g_functions[] = {XMA_LAYER(ENTRY)};
#undef ENTRY

#define INDEX(address, name, extra) k##address,
enum Index { XMA_LAYER(INDEX) };
#undef INDEX

std::mutex g_summary_mutex;
std::chrono::steady_clock::time_point g_last_summary = std::chrono::steady_clock::now();

void Summary() {
  auto now = std::chrono::steady_clock::now();
  {
    std::lock_guard lock(g_summary_mutex);
    if (now - g_last_summary < std::chrono::seconds(10)) {
      return;
    }
    g_last_summary = now;
  }
  std::string counts;
  for (auto& f : g_functions) {
    if (uint64_t n = f.calls.load()) {
      counts += fmt::format(" [{:08X}: {}]", f.address, n);
    }
  }
  REXLOG_INFO("xma: counts:{}", counts);
}

// Counts the call; true when it's one to log (the first 3 and powers of 2).
bool Count(Index i) {
  if (!REXCVAR_GET(log_xma)) {
    return false;
  }
  uint64_t n = ++g_functions[i].calls;
  Summary();
  return n <= 3 || std::has_single_bit(n);
}

In Capture(PPCContext& ctx) {
  return {uint32_t(ctx.lr),
          {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
           ctx.r10.u32}};
}

void Log(Index i, uint8_t* base, const In& in, const std::string& before, uint32_t result) {
  REXLOG_INFO("xma: {} call {} from {:08X}: r3 {:08X} r4 {:08X} r5 {:08X} r6 {:08X} r7 {:08X} "
              "r8 {:08X} r9 {:08X} r10 {:08X} -> {:08X}; before{}; after{}",
              g_functions[i].name, g_functions[i].calls.load(), in.lr, in.r[0], in.r[1], in.r[2],
              in.r[3], in.r[4], in.r[5], in.r[6], in.r[7], result, before,
              g_functions[i].extra(base, in));
}

}  // namespace

#define WRAPPER(address, name, logged)                                    \
  extern "C" REX_FUNC(sub_##address) {                                    \
    REX_FUNC_PROLOGUE();                                                  \
    bool log = Count(k##address);                                         \
    const In in = Capture(ctx);                                           \
    std::string before = log ? g_functions[k##address].extra(base, in) : ""; \
    __imp__sub_##address(ctx, base);                                      \
    if (log) {                                                            \
      Log(k##address, base, in, before, ctx.r3.u32);                      \
    }                                                                     \
  }

XMA_LAYER(WRAPPER)
#undef WRAPPER
