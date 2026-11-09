// The tuning screen's machine natively: adapters in front of
// src/tuning_machine/ (docs/functions/tuning-machine.md), with the register
// steps the harness shares (hooks/tuning_machine.h).

#include "hooks/tuning_machine.h"

#include "base/flags.h"
#include "tuning_machine/tuning_machine.h"

ITC_FLAG_DECLARE(bool, native_car_specs);

ITC_FLAG_BOOL(native_tuning_machine, true, "ITC",
              "Run the type 2 mode's tuning screen machine (docs/functions/tuning-machine.md: "
              "the tuning screen's creation, movies, dialog replies, leave, close and teardown) "
              "natively instead of through the translated code: Init sub_821E7568 (its step "
              "table, StateMachine::Add's plain C++); Create sub_821CE4C8 with ZeroRecord16 "
              "sub_821FE540 and DecalImages::Reset sub_821FE590 (VirtualAlloc through its "
              "override); OpenMovies sub_821CBDB0, WaitForMovies sub_821CE9E8 (UiMovies::IsLoaded "
              "by its plain C++), OnDialogClosed sub_821D8898 (UiMovie::SetText through its "
              "override, the varargs it leaves unset passed as 0), Unload sub_821D69A0, Close "
              "sub_821D6A78 and Destroy sub_821CBD40 (DecalImages::UnloadAll and VirtualFree "
              "through their overrides); DecalImages::SavePlaceholders sub_82208130 with "
              "SavePlaceholders14 sub_821F8950 (the same writes); TuningScreen::SetUp's callees "
              "LoadInteriorSlots sub_821DE280, LoadCurrentChoices sub_821DA4B0 "
              "(Params::GetCarSpec by its plain C++), LoadOwnedParts sub_821DBAB0 "
              "(TestAndSetWheelFlag by its plain C++), SetPrices sub_821DAD58 and "
              "LoadUnlockedParts sub_821CFFC8; TuningScreen::SetUp sub_821E1B48 (its five callees "
              "and SetAeroAvailability by their plain C++, Params::GetCarParts and CountBodyKits "
              "by theirs under --native_car_specs, Car::Init, memcpy and Car::Refresh through "
              "their overrides) (checked by --run_harnesses=tuning_machine)");

namespace hooks {

namespace {

namespace tm = tuning_machine;

// The guest calls the plain C++ makes (tuning_machine::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  tm::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = args[0];
    ctx.r4.u64 = args[1];
    ctx.r5.u64 = args[2];
    ctx.r6.u64 = args[3];
    ctx.r7.u64 = args[4];
    ctx.r8.u64 = args[5];
    ctx.r9.u64 = args[6];
    ctx.r10.u64 = args[7];
    if (f1 != nullptr) {
      ctx.f1.f64 = *f1;
    }
    ctx.lr = lr;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  PPCContext& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

}  // namespace

void TuningMachineCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tm::Create(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void TuningMachineZeroRecord16(PPCContext& ctx, uint8_t* base) {
  tm::ZeroRecord16(base, ctx.r4.u32);
}
void TuningMachineResetImages(PPCContext& ctx, uint8_t* base) {
  tm::ResetImages(base, ctx.r3.u32);
}
void TuningMachineOpenMovies(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tm::OpenMovies(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TuningMachineWaitForMovies(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tm::WaitForMovies(base, ctx.r3.u64, ctx.r1.u32);
}
void TuningMachineOnDialogClosed(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tm::OnDialogClosed(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TuningMachineUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tm::Unload(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void TuningMachineClose(PPCContext& ctx, uint8_t* base) {
  tm::Close(base, ctx.r3.u32, ctx.r4.u32);
}
void TuningMachineDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tm::Destroy(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void TuningMachineSavePlaceholders(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tm::SavePlaceholders(base, ctx.r3.u32, ctx.r1.u32);
}
void TuningMachineSavePlaceholders14(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tm::SavePlaceholders14(base, ctx.r3.u32, ctx.r1.u32);
}
// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the garage screen's Init.
void TuningMachineInit(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = tm::Init(base, machine, ctx.r1.u32 - tm::kInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}
void TuningMachineLoadInteriorSlots(PPCContext& ctx, uint8_t* base) {
  tm::LoadInteriorSlots(base, ctx.r3.u32);
}
// The vinyl angles' float math as the original's (no flush to zero).
void TuningMachineLoadCurrentChoices(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = tm::LoadCurrentChoices(base, ctx.r3.u32, ctx.r1.u64);
}
void TuningMachineLoadOwnedParts(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tm::LoadOwnedParts(base, ctx.r3.u32, ctx.r1.u32);
}
// The prices' float math as the original's (no flush to zero); r3 left.
void TuningMachineSetPrices(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  tm::SetPrices(base, ctx.r3.u32, ctx.r1.u32);
}
// A leaf; r3 left.
void TuningMachineLoadUnlockedParts(PPCContext& ctx, uint8_t* base) {
  tm::LoadUnlockedParts(base, ctx.r3.u32);
}
// The callees' float math as the originals' (no flush to zero); r3 left
// (the guest calls change ctx's, put back).
void TuningMachineSetUp(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t r3 = ctx.r3.u64, r1 = ctx.r1.u64;
  const bool native_specs = ITC_FLAG(native_car_specs);
  uint64_t out;
  {
    GuestCalls guest(ctx, base);
    out = tm::SetUp(base, r3, r1, native_specs, guest.calls());
  }
  ctx.r3.u64 = out;
}

}  // namespace hooks

#define ITC_TUNING_MACHINE(address, step)    \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_tuning_machine)) {  \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    hooks::step(ctx, base);                  \
  }

// The machine's handlers (functors the machine calls).
ITC_TUNING_MACHINE(821CE4C8, TuningMachineCreate)
ITC_TUNING_MACHINE(821CBDB0, TuningMachineOpenMovies)
ITC_TUNING_MACHINE(821CE9E8, TuningMachineWaitForMovies)
ITC_TUNING_MACHINE(821D8898, TuningMachineOnDialogClosed)
ITC_TUNING_MACHINE(821D69A0, TuningMachineUnload)
ITC_TUNING_MACHINE(821D6A78, TuningMachineClose)
ITC_TUNING_MACHINE(821CBD40, TuningMachineDestroy)
// Create's helpers ZeroRecord16 sub_821FE540 and DecalImages::Reset
// sub_821FE590 have no adapters: Create, their only caller, calls their
// plain C++ (their register steps are the harness's).
// SavePlaceholders (from Start sub_821E7368, recompiled); SavePlaceholders14
// has no adapter: SavePlaceholders, its only caller, calls its plain C++.
ITC_TUNING_MACHINE(82208130, TuningMachineSavePlaceholders)
// Init (from the type 2 mode's Init sub_82197278, recompiled).
ITC_TUNING_MACHINE(821E7568, TuningMachineInit)
// TuningScreen::SetUp's callees LoadInteriorSlots sub_821DE280, LoadCurrentChoices
// sub_821DA4B0, LoadOwnedParts sub_821DBAB0, SetPrices sub_821DAD58 and
// LoadUnlockedParts sub_821CFFC8: no adapters (removed, B7 loop, 2026-11-09:
// their only caller, SetUp, is native and calls their plain C++; the register
// steps above are the harness's).
// TuningScreen::SetUp (from Start sub_821E7368, recompiled).
ITC_TUNING_MACHINE(821E1B48, TuningMachineSetUp)
