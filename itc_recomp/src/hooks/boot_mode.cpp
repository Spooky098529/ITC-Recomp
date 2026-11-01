// The boot mode natively: adapters in front of src/boot_mode/
// (docs/functions/boot-mode.md), with the register steps the harness shares
// (hooks/boot_mode.h). Boot::WaitForMovie's and Movies::State's adapters are
// the wraps in src/hooks/title_screen.cpp (--skip_intro, --log_boot_modes),
// which call these steps. Functions reached only from native callers have no
// adapters (the flag switches their callers): Movies::Play, Start, Stop,
// Pause, Resume and Release (sub_820A43A8, sub_820A43D8, sub_820A43F0,
// sub_820A4408, sub_820A4420, sub_820A4438), from the boot machine's
// handlers and BootMode::NextMode; BootMachine::Init (sub_82107DD0), from
// BootMode::Init; BootMode::~BootMode (sub_82106D08), from its deleting
// destructor.

#include "hooks/boot_mode.h"

#include "base/flags.h"

ITC_FLAG_DECLARE(bool, play_movies_with_ffmpeg);

ITC_FLAG_BOOL(native_boot_mode, true, "ITC",
              "Run the boot mode (docs/functions/boot-mode.md: BootMode::Init sub_82107F68 with "
              "BootMachine::Init sub_82107DD0 (no adapter: only Init calls it), the boot "
              "machine's handlers Boot::StartMovie sub_82107B90, Boot::WaitForMovie sub_82107C58 "
              "(inside its --skip_intro wrap) and Boot::EndMovie sub_82107BF8, with Movies::Play "
              "sub_820A43A8, Start sub_820A43D8, Stop sub_820A43F0, Pause sub_820A4408, Resume "
              "sub_820A4420 and Release sub_820A4438 (no adapters: only these call them; the "
              "movie class's slots our player's plain C++ where they hold it) and Movies::State "
              "sub_820A4450 (inside its wrap), XMP's dropped calls left out (the last error they "
              "clear kept), BootMode::NextMode sub_82108018 (GameMessages::Dispatch's override a "
              "guest call), BootMode::~BootMode sub_82106D08 (no adapter: only its deleting "
              "destructor calls it) and BootMode::DeletingDestructor sub_82106D68, "
              "Mode::DeletingDestructor sub_82106E38 (operator delete a guest call to its "
              "adapter), Mode::NextModeThunk sub_82235C08, ModeDelegate::Invoke sub_8223D440 and "
              "ScreenMachine::ScreenMachine sub_82107358) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=boot_mode)");

namespace hooks {

namespace bm = boot_mode;

bool BootModeNative() {
  return ITC_FLAG(native_boot_mode);
}

bm::Calls BootModeCalls(PPCContext& ctx, uint8_t* base, std::function<bool()> finished_early) {
  bm::Calls calls;
  calls.call = [&ctx, base](uint32_t target, uint32_t sp, uint32_t lr, uint64_t r3,
                            uint64_t r4) -> uint64_t {
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    REX_CALL_INDIRECT_FUNC(target);
    ctx.r1.u64 = stack;
    ctx.lr = link;
    return ctx.r3.u64;
  };
  calls.pcr = ctx.r13.u32;
  calls.ffmpeg_movies = ITC_FLAG(play_movies_with_ffmpeg);
  calls.finished_early = std::move(finished_early);
  return calls;
}

namespace {

// StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
// transition's address from the machine's 64-bit r3.
uint64_t AddR3(uint64_t machine_r3, uint32_t machine, uint32_t r3, bool replaced) {
  return replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace

void BootModeInit(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64 + bm::kMachine;
  bool replaced = false;
  const uint32_t r3 = bm::Init(base, ctx.r3.u32, ctx.r1.u32 - bm::kInitFrame, &replaced);
  ctx.r3.u64 = AddR3(machine_r3, uint32_t(machine_r3), r3, replaced);
}

void BootMachineInit(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  bool replaced = false;
  const uint32_t r3 =
      bm::MachineInit(base, ctx.r3.u32, ctx.r1.u32 - bm::kMachineInitFrame, &replaced);
  ctx.r3.u64 = AddR3(machine_r3, ctx.r3.u32, r3, replaced);
}

void BootStartMovie(PPCContext& ctx, uint8_t* base) {
  const bm::Calls calls = BootModeCalls(ctx, base);
  ctx.r3.u64 = bm::StartMovie(base, ctx.r1.u32 - bm::kStartMovieFrame, calls);
}

void BootWaitForMovie(PPCContext& ctx, uint8_t* base, std::function<bool()> finished_early) {
  const bm::Calls calls = BootModeCalls(ctx, base, std::move(finished_early));
  ctx.r3.u64 = bm::WaitForMovie(base, ctx.r3.u32, ctx.r4.u32,
                                ctx.r1.u32 - bm::kWaitForMovieFrame, calls);
}

void BootEndMovie(PPCContext& ctx, uint8_t* base) {
  const bm::Calls calls = BootModeCalls(ctx, base);
  ctx.r3.u64 = bm::EndMovie(base, ctx.r1.u32 - bm::kEndMovieFrame, calls);
}

void BootMoviesState(PPCContext& ctx, uint8_t* base, std::function<bool()> finished_early) {
  const bm::Calls calls = BootModeCalls(ctx, base, std::move(finished_early));
  ctx.r3.u64 = bm::MoviesState(base, ctx.r1.u32 - bm::kStateFrame, calls);
}

void BootModeNextMode(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;  // the answer (the guest calls change r3)
  const bm::Calls calls = BootModeCalls(ctx, base);
  bm::NextMode(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - bm::kNextModeFrame, calls);
  ctx.r3.u64 = r3;
}

void BootModeDestruct(PPCContext& ctx, uint8_t* base) {
  bm::Destruct(base, ctx.r3.u32);
  ctx.r3.u64 = ctx.r3.u64 + bm::kMachine;
}

void BootModeDeletingDestructor(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const bm::Calls calls = BootModeCalls(ctx, base);
  bm::DeletingDestructor(base, ctx.r3.u32, ctx.r4.u32,
                         ctx.r1.u32 - bm::kDeletingDestructorFrame, calls);
  ctx.r3.u64 = r3;
}

void ModeDeletingDestructor(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const bm::Calls calls = BootModeCalls(ctx, base);
  bm::ModeDeletingDestructor(base, ctx.r3.u32, ctx.r4.u32,
                             ctx.r1.u32 - bm::kModeDeletingDestructorFrame, calls);
  ctx.r3.u64 = r3;
}

void ModeNextModeThunk(PPCContext& ctx, uint8_t* base) {
  const bm::Calls calls = BootModeCalls(ctx, base);
  ctx.r3.u64 = bm::NextModeThunk(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr),
                                 calls);
}

void ModeDelegateInvoke(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const bm::Calls calls = BootModeCalls(ctx, base);
  bm::ModeDelegateInvoke(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - bm::kInvokeFrame, calls);
  ctx.r3.u64 = r3;
}

void ScreenMachineConstruct(PPCContext& ctx, uint8_t* base) {
  bm::ScreenMachineConstruct(base, ctx.r3.u32);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_BOOT_MODE(address, step)    \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_boot_mode)) {  \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    hooks::step(ctx, base);             \
  }

// The mode's slots 0, 6 and 7, the delegate's slot 0 and the function it
// holds, and the machine's handlers are reached through pointers (no direct
// callers), so their adapters stay.
ITC_BOOT_MODE(82107F68, BootModeInit)
ITC_BOOT_MODE(82107B90, BootStartMovie)
ITC_BOOT_MODE(82107BF8, BootEndMovie)
ITC_BOOT_MODE(82108018, BootModeNextMode)
ITC_BOOT_MODE(82106D68, BootModeDeletingDestructor)
ITC_BOOT_MODE(82106E38, ModeDeletingDestructor)
ITC_BOOT_MODE(82235C08, ModeNextModeThunk)
ITC_BOOT_MODE(8223D440, ModeDelegateInvoke)
ITC_BOOT_MODE(82107358, ScreenMachineConstruct)
