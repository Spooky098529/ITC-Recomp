// The screens' draws (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "guest.h"

#include <initializer_list>

namespace race_screens {

namespace {

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// A lis and addi pair's value as the register holds it (sign-extended).
uint64_t Register(uint32_t address) { return uint64_t(int64_t(int32_t(address))); }

// The table's words stored at frame + 80 on.
void StoreTable(uint8_t* base, uint32_t frame, std::initializer_list<uint32_t> table) {
  uint32_t at = frame + 80;
  for (const uint32_t word : table) {
    guest::StoreU32(base, at, word);
    at += 4;
  }
}

// The transition's handler (the table's word at its index, read from the
// frame) run with `regs` (r3 to r10 at the original's bctrl).
uint64_t RunHandler(uint8_t* base, uint32_t frame, int32_t transition, const uint64_t (&regs)[8],
                    uint32_t lr, const Calls& calls) {
  const uint32_t handler = guest::LoadU32(base, (uint32_t(transition) << 2) + frame + 80);
  return calls.CallArgs(handler, frame, lr, regs);
}

}  // namespace

uint64_t PauseMenuDraw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220FB10 PauseMenu::Draw");
  const uint32_t frame = Frame(base, sp, 320);
  // RunTen, OpenMain, ShowMain, BackToMain, MoveMain, CloseMain, OpenOptions,
  // ShowOptionsRow, MoveOptions, ToggleOptions, CloseOptions, OpenVolume,
  // ShowVolumeRow, MoveVolume, SetVolume, CloseVolume, OpenAdjust,
  // ShowAdjustRow, MoveAdjust, SetAdjust, CloseAdjust.
  StoreTable(base, frame,
             {0x8220C5E0, 0x8220ED90, 0x8220EE28, 0x8220C668, 0x8220C7B8, 0x8220C700, 0x8220EEF0,
              0x8220C868, 0x8220DD98, 0x8220C930, 0x8220EFA8, 0x8220F6D0, 0x8220CA28, 0x8220DF30,
              0x8220E0F8, 0x8220F0A0, 0x8220F788, 0x8220CAB0, 0x8220E1F8, 0x8220F1B8,
              0x8220F2A8});
  const uint32_t pages = Get(base, kPausePages);
  const int32_t transition = int32_t(guest::LoadU32(base, pages + 12));
  uint64_t r3;
  if (transition == -1) {
    r3 = calls.Call(kRun, frame, 0x8220FC78, guest::LoadU32(base, screen), 0);
  } else if (transition < 0) {
    PagesFinish(base, pages, 1);
    r3 = pages;
  } else {
    const uint64_t regs[8] = {uint64_t(pages) + 24,  Register(0x8220F0A0), Register(0x8220F788),
                              Register(0x8220CAB0), Register(0x8220E1F8), Register(0x8220F1B8),
                              Register(0x8220F2A8), uint64_t(frame) + 80};
    const uint64_t result = RunHandler(base, frame, transition, regs, 0x8220FC50, calls);
    const uint32_t now = Get(base, kPausePages);
    PagesFinish(base, now, uint32_t(result));
    r3 = now;
  }
  if (int32_t(guest::LoadU32(base, Get(base, kPausePages))) == 4) {
    r3 = RaceDialogDraw(base, frame, calls);
  }
  return r3;
}

uint64_t AfterRaceMenuDraw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221BB48 AfterRaceMenu::Draw");
  const uint32_t frame = Frame(base, sp, 176);
  // The debug texts (the title, the items, the cursor at the list's
  // selection) go to the empty sub_82106540 (audio.md): only their loads,
  // which change nothing, are left out with them.
  // Intro, Show, ShowItem, Move, CloseMain, OpenList, ShowListItem, MoveList,
  // CloseList, HideListItem.
  StoreTable(base, frame,
             {0x8221A730, 0x8221A7A8, 0x8221A8A8, 0x8221AA20, 0x8221A950, 0x8221B918, 0x8221AAD8,
              0x8221B3C8, 0x8221B9C8, 0x8221AB60});
  const uint32_t pages = Get(base, kAfterPages);
  const int32_t transition = int32_t(guest::LoadU32(base, pages + 12));
  uint64_t r3;
  if (transition < 0) {
    r3 = calls.Call(kRun, frame, 0x8221BCF0, guest::LoadU32(base, screen), 0);
  } else {
    const uint64_t regs[8] = {uint64_t(pages) + 24,  Register(0x8221A950), Register(0x8221B918),
                              Register(0x8221AAD8), Register(0x8221B3C8), Register(0x8221B9C8),
                              Register(0x8221AB60), uint64_t(frame) + 80};
    const uint64_t result = RunHandler(base, frame, transition, regs, 0x8221BCC8, calls);
    const uint32_t now = Get(base, kAfterPages);
    PagesFinish(base, now, uint32_t(result));
    r3 = now;
  }
  const int32_t page = int32_t(guest::LoadU32(base, Get(base, kAfterPages)));
  if ((page >= 2 && page <= 4) || page == 6) {
    r3 = RaceDialogDraw(base, frame, calls);
  }
  return r3;
}

uint64_t CareerResultsDraw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A2F8 CareerResults::Draw");
  const uint32_t frame = Frame(base, sp, 144);
  // Intro, Show, Next2, Next3, Next4, NextEntry, Close1, Close2, Close3.
  StoreTable(base, frame,
             {0x82219210, 0x822192B0, 0x822195C0, 0x82219678, 0x82219730, 0x8221A110, 0x822193D8,
              0x822194A0, 0x82219530});
  const int32_t transition = int32_t(guest::LoadU32(base, screen + 196));
  if (transition < 0) {
    return calls.Call(kRun, frame, 0x8221A3CC, guest::LoadU32(base, screen + 180), 0);
  }
  const uint64_t regs[8] = {uint64_t(screen) + 208, Register(0x822195C0), Register(0x82219678),
                            Register(0x82219730),   Register(0x8221A110), Register(0x822193D8),
                            Register(0x822194A0),   uint64_t(frame) + 80};
  const uint64_t result = RunHandler(base, frame, transition, regs, 0x8221A3A8, calls);
  PagesFinish(base, screen + 184, uint32_t(result));
  return uint64_t(screen) + 184;
}

}  // namespace race_screens
