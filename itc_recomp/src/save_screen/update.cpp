// The save screen's Update (the dialog steps) and Draw (the cancel and the
// pages) (docs/functions/save-screen.md): see save_screen.h.

#include "base/log.h"
#include "save_screen/internal.h"

namespace save_screen {

namespace {

// Update's step table (steps 0 to 6) and Draw's page table (pages 0 to 26).
constexpr uint32_t kSteps[7] = {0x821A5668, 0x821A5118, 0x821A5038, 0x821A5980,
                                0x821A5D00, 0x821A6030, 0x821A63B0};
constexpr uint32_t kPageTable[27] = {
    0x821A28E0, 0x821A2AF8, 0x821A2C98, 0x821A2EE8, 0x821A4D30, 0x821A2FB0, 0x821A32C0,
    0x821A33E8, 0x821A3728, 0x821A4DE0, 0x821A3868, 0x821A3950, 0x821A3A08, 0x821A3C60,
    0x821A67E8, 0x821A3DE8, 0x821A4E60, 0x821A3ED8, 0x821A4228, 0x821A4450, 0x821A4810,
    0x821A4EF8, 0x821A2208, 0x821A4970, 0x821A4AC0, 0x821A4FD8, 0x821A2338};

// The table's addresses as the originals build them in registers (lis and
// addi: sign-extended to 64 bits).
constexpr uint64_t Extended(uint32_t address) { return 0xFFFFFFFF00000000ull | address; }

}  // namespace

uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A64A0 SaveScreen::Update");
  Fn f(base, sp, 144, calls);
  f.Call(kDebugPrint, 0x821A64C8, 100, 30, 0, 0x8200A4F0);
  const uint32_t movie = FindMovie(base, 44, f.frame + 80);
  f.r3 = movie;
  for (uint32_t i = 0; i < 7; i++) {
    Store(base, f.frame + 96 + 4 * i, kSteps[i]);
  }
  uint32_t pages = Pages(base);
  const int32_t step = int32_t(Load(base, pages + 12));
  if (step == -1) {
    if (int32_t(Load(base, Data(base) + 16)) != -1) {
      f.Call(kRun, 0x821A65F0, movie, 0);
    }
    return f.r3;
  }
  if (step >= 0) {
    const uint32_t function = Load(base, f.frame + 96 + 4 * uint32_t(step));
    const uint64_t args[8] = {pages + 24,
                              Extended(kSteps[0]),
                              Extended(kSteps[1]),
                              Extended(kSteps[2]),
                              Extended(kSteps[3]),
                              Extended(kSteps[4]),
                              Extended(kSteps[5]),
                              uint32_t(step) << 2};
    f.r3 = calls.CallArgs(function, f.frame, 0x821A656C, args);
    pages = Pages(base);
    if (int32_t(uint32_t(f.r3)) == 0) {
      return f.r3;
    }
  }
  const uint32_t queued = Load(base, pages + 16);
  if (int32_t(queued) == -1) {
    Store(base, pages + 0, Load(base, pages + 4));
    Store(base, pages + 12, kNone);
  } else {
    Store(base, pages + 16, kNone);
    Store(base, pages + 20, 0);
    Store(base, pages + 12, queued);
  }
  Store(base, pages + 24, 0);
  return f.r3;
}

uint64_t Draw(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A6AF0 SaveScreen::Draw");
  Fn f(base, sp, 368, calls, Extended(0x821A2C98));
  Store(base, sp + 20, machine);
  Store(base, sp + 28, mode);
  uint32_t pages = Pages(base);
  const int32_t page = int32_t(Load(base, pages + 0));
  const bool stepping = page == -1 && int32_t(Load(base, pages + 12)) != -1;
  if (!stepping && int32_t(Load(base, Data(base) + 28)) != 0 && page != 26 && page != 4 &&
      page != 9 && page != 16 && page != 21 && page != 25) {
    const uint32_t kind = Load(base, Data(base) + 36);
    if (kind == 0) {
      Go(base, 4, 2, kNone);
    } else if (kind == 1) {
      Go(base, 9, 2, kNone);
    } else if (kind < 6) {
      Store(base, Data(base) + 4, 0);
      Go(base, 16, 2, kNone);
    } else if (kind == 6) {
      if (int32_t(Load(base, Data(base) + 16)) == -1) {
        Jump(base, 21);
      } else {
        Go(base, 21, 2, kNone);
      }
    } else if (kind == 7) {
      Go(base, 25, 2, kNone);
    }
  }
  pages = Pages(base);
  const int32_t shown = int32_t(Load(base, pages + 0));
  Store(base, f.frame + 80, 0x821A0000);
  for (uint32_t i = 0; i < 27; i++) {
    Store(base, f.frame + 96 + 4 * i, kPageTable[i]);
  }
  if (shown != -1) {
    const uint32_t function = Load(base, f.frame + 96 + 4 * uint32_t(shown));
    const uint64_t args[8] = {Load(base, sp + 28),         Extended(kPageTable[20]),
                              Extended(kPageTable[21]),    Extended(kPageTable[22]),
                              Extended(kPageTable[23]),    Extended(kPageTable[24]),
                              Extended(kPageTable[25]),    f.frame + 96};
    f.r3 = calls.CallArgs(function, f.frame, 0x821A6DD0, args);
  }
  const uint32_t next = Load(base, Data(base) + 32);
  if (int32_t(next) != 21) {
    Store(base, Load(base, sp + 20) + kMachineState, next);
    Store(base, Data(base) + 32, 21);
    if (Load(base, Data(base) + 36) < 2) {
      f.Call(kSaveBuffersDestroy, 0x821A6E0C, f.r3);
    }
  }
  return f.r3;
}

}  // namespace save_screen
