// The game's main thread's start ("A4 brought forward" in
// docs/rexglue-removal.md): the kernel starts the thread at xstart's address
// (0x82296C80), and this is what runs there instead of the game's xstart,
// the C runtime's entry point (docs/functions/system-services.md). By "plug
// in data": its C runtime set-up and `main`, without the 360's checks.
//
// xstart did, in order: -1 into two C runtime globals; the heap check
// (CheckHeap sub_82297CE0: it makes the process heap first, CreateHeap
// sub_82297C08, so it stays); the thread notify
// routines (sub_82297B38 with 1); the PAL-50 check (sub_82296AB0, Dropped);
// _mtinit (sub_825AA708), _initterm_e (sub_82297AC0) and _cinit
// (sub_822979E0 with 1: the static initializers); the command line split
// into main's arguments only when [0x829537B8] is set (nothing sets it:
// main gets none); main (sub_82090108, native); then _cexit (sub_825AA388),
// DbgPrint and XamLoaderTerminateTitle, never reached in play (closing the
// window ends the process).

#include "app/app.h"
#include "base/flags.h"
#include "base/log.h"
#include "guest.h"
#include "lifted/sdk.h"
#include "schema/system_services.h"

ITC_FLAG_BOOL(native_start_up, true, "ITC",
              "Start the game's main thread in our start-up (src/app/start_up.cpp: the C "
              "runtime's set-up and main) instead of the game's xstart (the recompiled one when "
              "off).");

namespace app {
namespace {

using G = schema::SystemServicesGlobals;

constexpr uint32_t kFrameBytes = 496;            // xstart's frame
constexpr uint32_t kCheckHeap = 0x82297CE0;
constexpr uint32_t kThreadNotify = 0x82297B38;
constexpr uint32_t kMtInit = 0x825AA708;
constexpr uint32_t kInittermE = 0x82297AC0;
constexpr uint32_t kCInit = 0x822979E0;
constexpr uint32_t kMain = 0x82090108;
constexpr uint32_t kCExit = 0x825AA388;

}  // namespace

lifted::Result StartUp(const lifted::Args& in) {
  ITC_NATIVE_RUNNING("xstart StartUp");
  const uint32_t frame = uint32_t(in.r1) - kFrameBytes;
  guest::StoreU32(frame, uint32_t(in.r1));
  G::set_crt_word_a(0xFFFFFFFFu);
  G::set_crt_word_b(0xFFFFFFFFu);
  auto call = [&](uint32_t function, uint64_t r3) {
    lifted::Args args;
    args.r1 = frame;
    args.r13 = in.r13;
    args.r[0] = r3;
    return lifted::Call(function, args).r3;
  };
  call(kCheckHeap, 0);
  call(kThreadNotify, 1);
  call(kMtInit, 0);
  call(kInittermE, 0);
  call(kCInit, 1);
  if (G::command_line_set() != 0) {
    ITC_LOG_WARN("start-up: [0x829537B8] is set, but main gets no arguments");
  }
  const uint64_t result = call(kMain, 0);
  call(kCExit, 0);
  lifted::Args args;
  args.r1 = frame;
  args.r13 = in.r13;
  lifted::Import(__imp__XamLoaderTerminateTitle, args);
  return {result, in.f[0]};
}

}  // namespace app
