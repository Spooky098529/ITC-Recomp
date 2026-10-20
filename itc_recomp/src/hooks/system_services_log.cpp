// TEMPORARY (rexglue-removal B5, docs/rexglue-removal.md): counts the game's
// functions that call system service imports (Xam user, profile, UI,
// sessions, voice, notifications, the loader, XMsg, sockets, Xex, console
// settings), to find which run, how often, from where and what the SDK
// answers, before any is made native or dropped. Removed when the survey is
// done (a kept wrapper would block a real override).
//
// --log_system_services logs each function's first calls and every power of
// two (caller, r3-r10 in, r3 out, and what the function answers through
// memory where that's known), and every 10 s the counts of those called.
// The two functions with overrides already (the time zone read, wrapped by
// the time harness, and Device::Init) log through the functions in
// system_services_log.h.

#include "system_services_log.h"

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <bit>
#include <chrono>
#include <mutex>
#include <string>

REXCVAR_DEFINE_BOOL(log_system_services, false, "ITC",
                    "Log calls to the game's functions that call system service kernel "
                    "imports (temporary, for rexglue-removal B5)");

namespace {

struct In {
  uint32_t lr, r1, r[8];  // r3..r10
};

using Extra = std::string (*)(uint8_t* base, const In& in, uint32_t result);

std::string None(uint8_t*, const In&, uint32_t) {
  return {};
}

std::string WideString(uint8_t* base, uint32_t address, uint32_t max = 64) {
  if (!address) {
    return "(null)";
  }
  std::string s;
  for (uint32_t i = 0; i < max; i++) {
    uint16_t c = guest::LoadU16(base, address + 2 * i);
    if (!c) {
      break;
    }
    s += c < 0x80 ? char(c) : '?';
  }
  return "\"" + s + "\"";
}

std::string Hex(uint8_t* base, uint32_t address, uint32_t size) {
  std::string s;
  bool zero = true;
  for (uint32_t i = 0; i < size; i++) {
    uint8_t b = guest::LoadU8(base, address + i);
    zero = zero && !b;
    s += fmt::format("{:02X}", b);
  }
  return zero ? fmt::format("{} zero bytes", size) : s;
}

// XUserGetName (user, buffer, size): the name written.
std::string UserName(uint8_t* base, const In& in, uint32_t) {
  std::string s;
  for (uint32_t i = 0; in.r[1] && i < 32; i++) {
    char c = char(guest::LoadU8(base, in.r[1] + i));
    if (!c) {
      break;
    }
    s += c;
  }
  return fmt::format("; name \"{}\"", s);
}

// XUserGetXUID wrapper (user, xuid out).
std::string Xuid(uint8_t* base, const In& in, uint32_t) {
  if (!in.r[1]) {
    return {};
  }
  return fmt::format("; xuid {:08X}{:08X}", guest::LoadU32(base, in.r[1]),
                     guest::LoadU32(base, in.r[1] + 4));
}

// XUserCheckPrivilege wrapper (user, privilege, result out).
std::string Privilege(uint8_t* base, const In& in, uint32_t) {
  return in.r[2] ? fmt::format("; result {}", guest::LoadU32(base, in.r[2])) : std::string();
}

// XEnumerate wrapper (handle, buffer, size, items out, overlapped).
std::string Enumerate(uint8_t* base, const In& in, uint32_t) {
  return in.r[3] ? fmt::format("; items {}", guest::LoadU32(base, in.r[3])) : std::string();
}

// XShowMessageBoxUI wrapper (user, title, text, buttons, button texts,
// focus, flags, result out).
std::string MessageBox(uint8_t* base, const In& in, uint32_t) {
  std::string buttons;
  for (uint32_t i = 0; in.r[4] && i < in.r[3] && i < 4; i++) {
    buttons += " " + WideString(base, guest::LoadU32(base, in.r[4] + 4 * i));
  }
  return fmt::format("; title {} text {} buttons{}", WideString(base, in.r[1]),
                     WideString(base, in.r[2], 200), buttons);
}

// XUserReadProfileSettings wrapper (title, user, count, setting ids, size in
// and out, results, overlapped): the ids asked for and each setting returned
// (source: 0 none, 1 the profile, 2 the title's).
std::string ProfileSettings(uint8_t* base, const In& in, uint32_t result) {
  const uint32_t count = in.r[2], ids = in.r[3], size = in.r[4], buffer = in.r[5];
  std::string s = "; ids";
  for (uint32_t i = 0; ids && i < count && i < 32; i++) {
    s += fmt::format(" {:08X}", guest::LoadU32(base, ids + 4 * i));
  }
  if (size) {
    s += fmt::format("; size {}", guest::LoadU32(base, size));
  }
  if (!buffer || (result != 0 && result != 997)) {
    return s;
  }
  const uint32_t n = guest::LoadU32(base, buffer), settings = guest::LoadU32(base, buffer + 4);
  for (uint32_t i = 0; settings && i < n && i < 32; i++) {
    const uint32_t setting = settings + 40 * i;
    const uint32_t id = guest::LoadU32(base, setting + 16);
    const uint8_t type = guest::LoadU8(base, setting + 24);
    s += fmt::format("; [{:08X} from {} type {}", id, guest::LoadU32(base, setting), type);
    if (type == 4 || type == 6) {  // a wide string or binary: size and pointer
      const uint32_t bytes = guest::LoadU32(base, setting + 32);
      const uint32_t data = guest::LoadU32(base, setting + 36);
      s += fmt::format(" {} bytes", bytes);
      if (data && bytes) {
        s += type == 4 ? " " + WideString(base, data, bytes / 2)
                       : " " + Hex(base, data, bytes < 64 ? bytes : 64);
      }
    } else {
      s += fmt::format(" {:08X}{:08X}", guest::LoadU32(base, setting + 32),
                       guest::LoadU32(base, setting + 36));
    }
    s += "]";
  }
  return s;
}

struct Function {
  uint32_t address;
  const char* name;
  Extra extra;
  bool log_entry;  // may not return: log on the way in too
  std::atomic<uint64_t> calls{0};
};

// Names are guesses from the imports each calls ("probably" throughout);
// XMsg ids are the app (FA XMP, FB XGI, FC XLiveBase) and message.
#define SYSTEM_SERVICES(X)                                                              \
  X(82099828, "XNotifyGetNext caller (vtable)", None, false)                            \
  X(820A4610, "XNotifyGetNext caller (vtable) 2", None, false)                          \
  X(82149080, "XNotifyGetNext caller (from 8214E268)", None, false)                     \
  X(8214E268, "network update? (XNotify, XNet, XUID, name)", None, false)               \
  X(8214F290, "VS MODE screen? (sign-in UI, message box, launch)", None, false)         \
  X(82294E58, "XUserWriteAchievements (FB B0008)", None, false)                         \
  X(82294ED8, "XUserSetContextEx (FB B0006)", None, false)                              \
  X(82294F78, "XUserSetPropertyEx (FB B0007)", None, false)                             \
  X(82295020, "XUserGetName", UserName, false)                                          \
  X(82295028, "XUserGetSigninState", None, false)                                       \
  X(82295030, "XUserCheckPrivilege", Privilege, false)                                  \
  X(822950C0, "XGI FB B0021", None, false)                                              \
  X(822951A8, "XUserCreateStatsEnumerator (r4 1)", None, false)                         \
  X(82295208, "XUserCreateStatsEnumerator 2", None, false)                              \
  X(82295268, "XUserGetXUID", Xuid, false)                                              \
  X(822952B8, "XNotifyCreateListener", None, false)                                     \
  X(822952C0, "XShowSigninUI", None, false)                                             \
  X(822952C8, "XShowFriendsUI", None, false)                                            \
  X(822952D0, "XShowGamerCardUI", None, false)                                          \
  X(822952D8, "XShowPlayerReviewUI", None, false)                                       \
  X(822952E0, "XShowDeviceSelectorUI", None, false)                                     \
  X(822952E8, "XShowMessageBoxUI", MessageBox, false)                                   \
  X(82295310, "XShowDirtyDiscErrorUI", None, true)                                      \
  X(82295D30, "XEnumerate", Enumerate, false)                                           \
  X(822967E0, "XLaunchNewImage", None, true)                                            \
  X(82296950, "PAL-50 message box", None, false)                                        \
  X(82296AB0, "PAL-50 check", None, false)                                              \
  X(82296EE0, "title id check (XamGetExecutionId)", None, false)                        \
  X(82297C08, "heap check (RtlImageXexHeaderField)", None, false)                       \
  X(82297CE0, "heap check or HalReturnToFirmware", None, true)                          \
  X(82459B68, "WSAStartup", None, false)                                                \
  X(82459C20, "socket", None, false)                                                    \
  X(82459C38, "closesocket", None, false)                                               \
  X(82459C48, "ioctlsocket", None, false)                                               \
  X(82459C60, "bind", None, false)                                                      \
  X(82459C78, "connect", None, false)                                                   \
  X(82459C90, "listen", None, false)                                                    \
  X(82459CA0, "accept", None, false)                                                    \
  X(82459CB8, "select", None, false)                                                    \
  X(82459CD8, "recvfrom", None, false)                                                  \
  X(82459CF8, "sendto", None, false)                                                    \
  X(82459D18, "WSAGetLastError", None, false)                                           \
  X(82459D20, "__WSAFDIsSet", None, false)                                              \
  X(82459D28, "XNetXnAddrToInAddr", None, false)                                        \
  X(82459D40, "XNetXnAddrToMachineId", None, false)                                     \
  X(82459D50, "XNetGetTitleXnAddr", None, false)                                        \
  X(82575C90, "XLiveBase FC 58004 (logon id)", None, false)                             \
  X(82575E50, "XSessionCreate (FB B0010)", None, false)                                 \
  X(82575FE0, "XSessionModify (FB B0018)", None, false)                                 \
  X(82576080, "XSessionDelete (FB B0011)", None, false)                                 \
  X(82576130, "XSession FB B0012", None, false)                                         \
  X(825761D8, "XSession FB B0012 2", None, false)                                       \
  X(82576280, "XSession FB B0013", None, false)                                         \
  X(82576328, "XSession FB B0013 2", None, false)                                       \
  X(825763D0, "XSession FB B001A", None, false)                                         \
  X(825764A8, "XSessionStart (FB B0014)", None, false)                                  \
  X(82576540, "XSessionEnd (FB B0015)", None, false)                                    \
  X(825765F0, "XSessionSearchEx (FB B001C)", None, false)                               \
  X(825766E0, "XSession FB B0025", None, false)                                         \
  X(825767D0, "XLiveBase FC 58023", None, false)                                        \
  X(82576900, "XLiveBase FC (with alloc)", None, false)                                 \
  X(82576BE0, "XLiveBase FC (with alloc) 2", None, false)                               \
  X(82576E78, "XUserReadProfileSettings", ProfileSettings, false)                       \
  X(82576EE0, "XLiveBase FC 5800E", None, false)                                        \
  X(82576F30, "XLiveBase FC 58035", None, false)                                        \
  X(82576FE8, "XLiveBase FC 58035 2", None, false)                                      \
  X(8257AF30, "voice: XNotifyGetNext", None, false)                                     \
  X(8257B7A8, "voice: init, XNotifyCreateListener", None, false)                        \
  X(8257D798, "voice: user check (sign-in, privileges, profile, headset)", None, false) \
  X(8257DDA0, "voice: close", None, false)                                              \
  X(8257DFA8, "voice: submit", None, false)                                             \
  X(8257E3B8, "voice: close 2", None, false)                                            \
  X(8257E4B0, "voice: XamVoiceCreate", None, false)                                     \
  X(8257E568, "voice: profile read", None, false)                                       \
  X(825816D8, "voice: XamVoiceSubmitPacket", None, false)                               \
  X(82589360, "XMP FA 7001A (set playback controller)", None, false)                    \
  X(82589408, "XMP FA 7001B (get playback controller)", None, false)                    \
  X(825894C8, "XMP controller (from 82107B90)", None, false)                            \
  X(825895A0, "XMP controller (from 82107BF8)", None, false)

#define ENTRY(address, name, extra, log_entry) {0x##address, #address " " name, extra, log_entry},
Function g_functions[] = {SYSTEM_SERVICES(ENTRY){0x8229B440, "8229B440 time zone read", None,
                                                  false},
                          {0x822AD298, "822AD298 Device::Init's config read", None, false}};
#undef ENTRY

#define INDEX(address, name, extra, log_entry) k##address,
enum Index { SYSTEM_SERVICES(INDEX) k8229B440, k822AD298 };
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
  REXLOG_INFO("system services: counts:{}", counts);
}

// Counts the call; true when it's one to log (the first 3 and powers of 2).
bool Count(Index i) {
  if (!REXCVAR_GET(log_system_services)) {
    return false;
  }
  uint64_t n = ++g_functions[i].calls;
  Summary();
  return n <= 3 || std::has_single_bit(n);
}

In Capture(PPCContext& ctx) {
  return {uint32_t(ctx.lr), ctx.r1.u32,
          {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
           ctx.r10.u32}};
}

std::string Registers(const In& in) {
  return fmt::format("r3 {:08X} r4 {:08X} r5 {:08X} r6 {:08X} r7 {:08X} r8 {:08X} r9 {:08X} "
                     "r10 {:08X}",
                     in.r[0], in.r[1], in.r[2], in.r[3], in.r[4], in.r[5], in.r[6], in.r[7]);
}

void LogEntry(Index i, const In& in) {
  REXLOG_INFO("system services: {} call {} from {:08X} (entering): {}", g_functions[i].name,
              g_functions[i].calls.load(), in.lr, Registers(in));
}

void Log(Index i, uint8_t* base, const In& in, uint32_t result) {
  REXLOG_INFO("system services: {} call {} from {:08X}: {} -> {:08X}{}", g_functions[i].name,
              g_functions[i].calls.load(), in.lr, Registers(in), result,
              g_functions[i].extra(base, in, result));
}

}  // namespace

#define WRAPPER(address, name, extra, log_entry) \
  extern "C" REX_FUNC(sub_##address) {           \
    REX_FUNC_PROLOGUE();                         \
    bool log = Count(k##address);                \
    const In in = Capture(ctx);                  \
    if (log && (log_entry)) {                    \
      LogEntry(k##address, in);                  \
    }                                            \
    __imp__sub_##address(ctx, base);             \
    if (log) {                                   \
      Log(k##address, base, in, ctx.r3.u32);     \
    }                                            \
  }

SYSTEM_SERVICES(WRAPPER)
#undef WRAPPER

namespace system_services_log {

void TimeZoneRead(uint8_t* base, uint32_t lr, uint32_t zone, uint32_t uses_daylight,
                  uint32_t result) {
  if (!Count(k8229B440)) {
    return;
  }
  REXLOG_INFO("system services: {} call {} from {:08X}: zone {:08X} -> {:08X}; bias {} "
              "standard {} {} bias {} daylight {} {} bias {}; uses daylight {}",
              g_functions[k8229B440].name, g_functions[k8229B440].calls.load(), lr, zone, result,
              int32_t(guest::LoadU32(base, zone)), WideString(base, zone + 4, 32),
              Hex(base, zone + 68, 16), int32_t(guest::LoadU32(base, zone + 84)),
              WideString(base, zone + 88, 32), Hex(base, zone + 152, 16),
              int32_t(guest::LoadU32(base, zone + 168)),
              uses_daylight ? guest::LoadU32(base, uses_daylight) : 0);
}

void ConfigSetting(uint8_t* base, uint32_t category, uint32_t setting, uint32_t buffer,
                   uint32_t size, uint32_t result) {
  if (!Count(k822AD298)) {
    return;
  }
  REXLOG_INFO("system services: {}: ExGetXConfigSetting({}, {}) -> {:08X}; value {}",
              g_functions[k822AD298].name, category, setting, result, Hex(base, buffer, size));
}

}  // namespace system_services_log
