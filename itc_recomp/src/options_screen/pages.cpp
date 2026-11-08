// The options screen's machine dispatch and page helpers
// (docs/functions/options-screen.md): see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "career/career.h"
#include "guest.h"
#include "memory/heap.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kMinusOne = 0xFFFFFFFFu;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t Settings(uint8_t* base) { return LoadU32(base, kSettings); }

// A guest call with every argument register given (r3 to r10: the
// originals' leftovers included).
uint64_t CallAll(const Calls& calls, uint32_t function, uint32_t frame, uint32_t lr,
                 const uint64_t (&args)[8]) {
  return calls.call(calls.user, function, args, nullptr, frame, lr);
}

// A code address as the originals build it in a register (lis, addi: the
// 64-bit register sign-extended).
constexpr uint64_t Code(uint32_t address) { return uint64_t(int64_t(int32_t(address))); }

// The table's words at `frame` + `at`.
template <size_t n>
void StoreTable(uint8_t* base, uint32_t at, const uint32_t (&table)[n]) {
  for (size_t i = 0; i < n; i++) {
    StoreU32(base, at + 4 * uint32_t(i), table[i]);
  }
}

// Pages 9 and 10: with the dialog's cancel (`o` +708) and the sub-pages'
// page (`pages` +0) not 5, the options' page block goes to `step` with
// `page` after, and the sub-pages' to 5.
void CancelDialog(uint8_t* base, uint32_t pages_global, uint32_t step, uint32_t page) {
  if (LoadU32(base, Settings(base) + 708) == 0 ||
      LoadU32(base, LoadU32(base, pages_global) + 0) == 5) {
    return;
  }
  const uint32_t p = LoadU32(base, kPage);
  const uint32_t current = LoadU32(base, p + 0);
  if (current != page) {
    StoreU32(base, p + 8, current);
  }
  StoreU32(base, p + 12, step);
  StoreU32(base, p + 16, kMinusOne);
  StoreU32(base, p + 4, page);
  StoreU32(base, p + 0, kMinusOne);
  const uint32_t pages = LoadU32(base, pages_global);
  const uint32_t sub = LoadU32(base, pages + 0);
  if (sub != 5) {
    StoreU32(base, pages + 8, sub);
  }
  StoreU32(base, pages + 12, kMinusOne);
  StoreU32(base, pages + 16, kMinusOne);
  StoreU32(base, pages + 0, 5);
}

}  // namespace

uint64_t Draw(uint8_t* base, uint64_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C80F0 Options::Draw");
  const uint32_t frame = Frame(base, sp, 224);
  static constexpr uint32_t kPages[15] = {
      0x821BC670, 0x821C1270, 0x821C1498, 0x821C1888, 0x821C1AA0, 0x821C1D08, 0x821C1ED8, 0x821C21A8,
      0x821C2388, 0x821C6F58, 0x821BD228, 0x821BA010, 0x821BA0F0, 0x821BA380, 0x821BD330};
  StoreTable(base, frame + 80, kPages);
  const uint32_t page = LoadU32(base, LoadU32(base, kPage) + 0);
  if (page == kMinusOne) {
    return Code(0x821C2388);  // the table's last register
  }
  const uint32_t handler = LoadU32(base, frame + 80 + (page << 2));
  return CallAll(calls, handler, frame, 0x821C81DC,
                 {dispatcher, dispatcher, Code(0x821C6F58), Code(0x821BD228), Code(0x821BA010),
                  Code(0x821BA0F0), Code(0x821BA380), frame + 80});
}

uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C81E8 Options::Update");
  // The empty sub_82106540 (r3 = 100, r4 = 30, r5 = 0, r6 = "TXR_MenuSystem")
  // does nothing: not called.
  const uint32_t frame = Frame(base, sp, 480);
  const uint32_t m = FindMovie(base, frame);
  // The 49 steps at +128; +80 to +112 the high half the original spills.
  static constexpr uint32_t kSteps[49] = {
      0x821BD3C8, 0x821C2778, 0x821C2A10, 0x821C7080, 0x821BD468, 0x821BD5B0, 0x821C2BE0,
      0x821C7188, 0x821BD680, 0x821BD728, 0x821C2E28, 0x821BD800, 0x821C75E0, 0x821BD908,
      0x821C3008, 0x821C3278, 0x821C7710, 0x821BD9A0, 0x821C3398, 0x821C35F0, 0x821C7868,
      0x821BDA38, 0x821C36F0, 0x821C3910, 0x821C79B8, 0x821BDAD0, 0x821C3A10, 0x821C3E48,
      0x821C7B08, 0x821BDC18, 0x821BDCB0, 0x821C41D0, 0x821C7BF0, 0x821BDE88, 0x821C43D0,
      0x821C47A0, 0x821C7D18, 0x821BDFA8, 0x821BE040, 0x821C4A58, 0x821C7E00, 0x821C6468,
      0x821C6550, 0x821C6638, 0x821C08B0, 0x821C09B0, 0x821C69D0, 0x821C0A50, 0x821C0B50};
  for (uint32_t at = 80; at <= 112; at += 4) {
    StoreU32(base, frame + at, 0x821C0000);
  }
  StoreTable(base, frame + 128, kSteps);
  uint32_t p = LoadU32(base, kPage);
  const int32_t step = int32_t(LoadU32(base, p + 12));
  // A step's end: the queued step, or with none the page after.
  const auto advance = [&]() {
    const uint32_t queued = LoadU32(base, p + 16);
    if (queued != kMinusOne) {
      StoreU32(base, p + 16, kMinusOne);
      StoreU32(base, p + 20, 0);
      StoreU32(base, p + 12, queued);
    } else {
      StoreU32(base, p + 0, LoadU32(base, p + 4));
      StoreU32(base, p + 12, kMinusOne);
    }
    StoreU32(base, p + 24, 0);
  };
  if (step == -1) {
    if (LoadU32(base, p + 0) == 14) {
      return m;
    }
    return calls.Call(kRun, frame, 0x821C856C, m, 0);
  }
  if (step < 0) {
    advance();
    return m;
  }
  const uint32_t index = uint32_t(step) << 2;
  const uint32_t function = LoadU32(base, frame + 128 + index);
  const uint64_t r3 = CallAll(calls, function, frame, 0x821C84F0,
                              {p + 24, Code(0x821C6638), Code(0x821C08B0), Code(0x821C09B0),
                               Code(0x821C69D0), Code(0x821C0A50), frame + 128, index});
  p = LoadU32(base, kPage);
  if (uint32_t(r3) != 0) {
    advance();
  }
  return r3;
}

uint64_t SavePage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C6F58 Options::SavePage");
  const uint32_t frame = Frame(base, sp, 128);
  CancelDialog(base, 0x826D847C, 41, 9);
  static constexpr uint32_t kPages[7] = {0x821BBC10, 0x821BC7B0, 0x821BC940, 0x821C2620,
                                         0x821B9FB0, 0x821B9FF8, 0x821BBC68};
  StoreTable(base, frame + 80, kPages);
  const int32_t sub = int32_t(LoadU32(base, LoadU32(base, 0x826D847C) + 0));
  if (sub >= 0) {
    const uint32_t handler = LoadU32(base, frame + 80 + (uint32_t(sub) << 2));
    CallAll(calls, handler, frame, 0x821C705C,
            {r3, Code(0x821BBC10), Code(0x821BC7B0), Code(0x821BC940), Code(0x821C2620),
             Code(0x821B9FB0), Code(0x821B9FF8), frame + 80});
  }
  return calls.Call(kMenuPressed, frame, 0x821C706C, LoadU32(base, kMenu), 158);
}

uint64_t Category6Page(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BD228 Options::Category6Page");
  const uint32_t frame = Frame(base, sp, 112);
  CancelDialog(base, 0x826D8488, 42, 10);
  static constexpr uint32_t kPages[6] = {0x821BBCC0, 0x821BCA60, 0x821BCC10,
                                         0x821BBD18, 0x821BA508, 0x821B9FF8};
  StoreTable(base, frame + 80, kPages);
  const int32_t sub = int32_t(LoadU32(base, LoadU32(base, 0x826D8488) + 0));
  if (sub < 0) {
    return r3;
  }
  const uint32_t handler = LoadU32(base, frame + 80 + (uint32_t(sub) << 2));
  return CallAll(calls, handler, frame, 0x821BD320,
                 {r3, r4, Code(0x821BBCC0), Code(0x821BCA60), Code(0x821BCC10), Code(0x821BBD18),
                  Code(0x821BA508), frame + 80});
}

uint64_t StepHud(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BE3E0 Options::StepHud");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t menu = LoadU32(base, kMenu);
  const uint32_t o = Settings(base);
  const int32_t side = int32_t(LoadU32(base, menu + 36));
  const uint32_t cursor = LoadU32(base, menu + 0);
  // The five values' addresses at +80: the four HUD edits and OK/Default.
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(base, frame + 80 + 4 * i, o + 236 + 4 * i);
  }
  StoreU32(base, frame + 96, o + 660);
  if (side != -1 && side != 1) {
    return 0;
  }
  // Read through the table as the original does, for any cursor.
  const uint32_t value = LoadU32(base, frame + 80 + (cursor << 2));
  if (int32_t(LoadU32(base, value)) != (side == -1 ? 0 : 1)) {
    return 0;
  }
  StoreU32(base, value, side == -1 ? 1 : 0);
  ShowHudValues(base, frame);
  return 1;
}

uint64_t ShowVolumeBars(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BE8A8 Options::ShowVolumeBars");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = FindMovie(base, frame);
  const uint32_t music = ui_movies::FindNode(base, m, 6, 6);
  ShowVolumeBar(base, music, LoadU32(base, Settings(base) + 252), frame);
  const uint32_t effects = ui_movies::FindNode(base, m, 6, 7);
  return ShowVolumeBar(base, effects, LoadU32(base, Settings(base) + 256), frame);
}

uint64_t ShowPrompts(uint8_t* base, const uint64_t (&values)[6], uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821C1068 Options::ShowPrompts");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t m = FindMovie(base, frame);
  // r3, r5, r7 (the first frames) at +88; r4, r6, r8 (the second) at +104.
  for (uint32_t i = 0; i < 3; i++) {
    StoreU32(base, frame + 88 + 4 * i, uint32_t(values[2 * i]));
    StoreU32(base, frame + 104 + 4 * i, uint32_t(values[2 * i + 1]));
  }
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t prompt = ui_movies::FindNode(base, m, 1, i);
    const uint32_t first = ui_movies::FindNode(base, m, 1, LoadU32(base, prompt + 100));
    const uint32_t second = ui_movies::FindNode(base, m, 1, LoadU32(base, prompt + 100) + 1);
    r3 = second;
    const uint32_t a = LoadU32(base, frame + 88 + 4 * i);
    const uint32_t b = LoadU32(base, frame + 104 + 4 * i);
    if (a == kMinusOne || b == kMinusOne) {
      StoreU32(base, prompt + 80, 0);
      continue;
    }
    StoreU32(base, prompt + 80, 1);
    StoreU32(base, first + 108, a);
    StoreU32(base, second + 108, b);
  }
  return r3;
}

uint64_t SettingsReload(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C0FF0 Settings::Reload");
  const uint32_t frame = Frame(base, sp, 96);
  switch (int32_t(LoadU32(base, Settings(base) + 4))) {
    case 1:
      // Options::ShowCareerInfo sub_821C0EF0 (never seen in play: the
      // original, through the function table).
      calls.Call(kShowCareerInfo, frame, 0x821C1044);
      career::PlayTimeMark(base, vehicle_accessors::SaveDataOf(base, 0));
      break;
    case 2: StoreU32(base, 0x826367E8 + 1972, kMinusOne); break;
    case 3: StoreU32(base, 0x826371D0 + 1972, kMinusOne); break;
    default: break;
  }
  return SettingsRead(base, frame, calls);
}

uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B9E70 Options::OnLeft");
  const uint32_t frame = Frame(base, sp, 160);
  // The sound manager's slot 13 (StopCue: bank 2's cue 0 from +84 and +80)
  // and slot 5 (ReleaseBank: bank 2 at +88).
  StoreU32(base, frame + 80, 0);
  StoreU32(base, frame + 84, 2);
  uint32_t manager = LoadU32(base, kSoundManagerGlobal);
  calls.Call(LoadU32(base, LoadU32(base, manager) + 52), frame, 0x821B9EAC, manager, frame + 84,
             frame + 80);
  manager = LoadU32(base, kSoundManagerGlobal);
  StoreU32(base, frame + 88, 2);
  calls.Call(LoadU32(base, LoadU32(base, manager) + 20), frame, 0x821B9EC8, manager, frame + 88);
  // VirtualFree(block, 0, MEM_RELEASE) of the seven blocks, over the
  // allocation layer's pages (the adapter runs it only with the native
  // heap active), then the globals 0.
  static constexpr uint32_t kLr[7] = {0x821B9EE4, 0x821B9EF8, 0x821B9F0C, 0x821B9F20,
                                      0x821B9F34, 0x821B9F48, 0x821B9F5C};
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t block = LoadU32(base, kPage + 4 * i);
    if (memory::InRange(block)) {
      r3 = memory::FreePages(block) ? 1 : 0;
    } else {
      r3 = calls.Call(kVirtualFree, frame, kLr[i], block, 0, 0x8000);
    }
  }
  for (uint32_t i = 0; i < 7; i++) {
    StoreU32(base, kPage + 4 * i, 0);
  }
  return r3;
}

uint64_t CategoryPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BC670 Options::CategoryPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x821BC68C, LoadU32(base, kMenu));
  const uint32_t menu = LoadU32(base, kMenu);
  uint32_t p;
  if (LoadU32(base, menu + 8) != 0) {
    // Moved: step 6, the categories after.
    p = LoadU32(base, kPage);
    const uint32_t page = LoadU32(base, p + 0);
    if (page != 0) {
      StoreU32(base, p + 8, page);
    }
    StoreU32(base, p + 4, 0);
    StoreU32(base, p + 16, kMinusOne);
    StoreU32(base, p + 12, 6);
    StoreU32(base, p + 0, kMinusOne);
    return calls.Call(kMenuSoundPlay, frame, 0x821BC728, 4);
  }
  if (uint32_t(calls.Call(kMenuPressed, frame, 0x821BC6D4, menu, 159)) != 0) {
    // Accept: step 7 (OpenCategory) on the cursor.
    StoreU32(base, Settings(base) + 664, LoadU32(base, LoadU32(base, kMenu) + 0));
    p = LoadU32(base, kPage);
    const uint32_t page = LoadU32(base, p + 0);
    if (page != kMinusOne) {
      StoreU32(base, p + 8, page);
    }
    StoreU32(base, p + 4, kMinusOne);
    StoreU32(base, p + 16, kMinusOne);
    StoreU32(base, p + 12, 7);
    StoreU32(base, p + 0, kMinusOne);
    return calls.Call(kMenuSoundPlay, frame, 0x821BC728, 0);
  }
  const uint64_t back = calls.Call(kMenuPressed, frame, 0x821BC738, LoadU32(base, kMenu), 158);
  if (uint32_t(back) == 0) {
    return back;
  }
  // Back: step 8 (the fade out), the close page after.
  const uint64_t r3 = calls.Call(kMenuSoundPlay, frame, 0x821BC748, 2);
  p = LoadU32(base, kPage);
  const uint32_t page = LoadU32(base, p + 0);
  if (page != 14) {
    StoreU32(base, p + 8, page);
  }
  StoreU32(base, p + 16, kMinusOne);
  StoreU32(base, p + 12, 8);
  StoreU32(base, p + 4, 14);
  StoreU32(base, p + 0, kMinusOne);
  if (int32_t(LoadU32(base, kModeType)) == 4) {
    StoreU32(base, 0x826364EC, 0);
  }
  return r3;
}

uint64_t SetUpMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BCFF0 Options::SetUpMovie");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t m = FindMovie(base, frame);
  calls.Call(kResetPose, frame, 0x821BD020, m);
  const uint32_t career = ui_movies::FindNode(base, m, kMinusOne, 16);
  const uint32_t other = ui_movies::FindNode(base, m, kMinusOne, 17);
  const uint32_t title = ui_movies::FindNode(base, m, 17, 1);
  // The career's panel (context 1) or the others' with the title's frame.
  const auto others = [&](uint32_t title_frame) {
    StoreU32(base, career + 80, 0);
    StoreU32(base, other + 80, 1);
    StoreU32(base, title + 108, title_frame);
  };
  switch (LoadU32(base, Settings(base) + 4)) {
    case 0: others(4); break;
    case 1: {
      StoreU32(base, career + 80, 1);
      StoreU32(base, other + 80, 0);
      calls.Call(kSetChildNameText, frame, 0x821BD0D4, m, 16, 6, Settings(base) + 12);
      calls.Call(kSetNumberText, frame, 0x821BD0EC, m, 16, 9, LoadU32(base, Settings(base) + 32));
      // UiMovie::SetText(m, 16, 13, 8, the day): its unset varargs r8 to
      // r10 passed as 0 (the user's call, 2026-10-30).
      CallAll(calls, kSetText, frame, 0x821BD108,
              {m, 16, 13, 8, LoadU32(base, Settings(base) + 40), 0, 0, 0});
      const uint32_t weekday = ui_movies::FindNode(base, m, 16, 11);
      StoreU32(base, weekday + 108, LoadU32(base, Settings(base) + 36));
      break;
    }
    case 2: others(0); break;
    case 3: others(1); break;
    case 4: others(2); break;
    default: break;
  }
  StoreU32(base, ui_movies::FindNode(base, m, 1, 4) + 108, 0);
  StoreU32(base, ui_movies::FindNode(base, m, 1, 5) + 108, 0);
  StoreU32(base, ui_movies::FindNode(base, m, 1, 6) + 108, 1);
  StoreU32(base, ui_movies::FindNode(base, m, 1, 7) + 108, 1);
  StoreU32(base, ui_movies::FindNode(base, m, 1, 8) + 80, 0);
  StoreU32(base, ui_movies::FindNode(base, m, 1, 9) + 80, 0);
  // The menu music playing: the first of bank 1's cues 0 to 3 the sound
  // manager's slot 14 (IsCuePlaying, bank at +88 and cue at +84) answers.
  uint64_t r3 = 0;
  for (uint32_t cue = 0; cue < 4; cue++) {
    StoreU32(base, frame + 84, cue);
    StoreU32(base, frame + 88, 1);
    const uint32_t manager = LoadU32(base, kSoundManagerGlobal);
    r3 = calls.Call(LoadU32(base, LoadU32(base, manager) + 56), frame, 0x821BD1EC, manager,
                    frame + 88, frame + 84);
    if ((r3 & 0xFF) != 0) {
      StoreU32(base, Settings(base) + 704, cue);
      break;
    }
  }
  StoreU32(base, Settings(base) + 692, kMinusOne);
  StoreU32(base, Settings(base) + 0, 1);
  return r3;
}

uint64_t Show(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C1158 Options::Show");
  const uint32_t frame = Frame(base, sp, 112);
  SettingsRead(base, frame, calls);
  StoreU32(base, Settings(base) + 664, 0);
  const uint64_t r3 = SetUpMovie(base, frame, calls);
  const uint32_t p = LoadU32(base, kPage);
  const uint32_t page = LoadU32(base, p + 0);
  if (page != 0) {
    StoreU32(base, p + 8, page);
  }
  StoreU32(base, p + 4, 0);
  StoreU32(base, p + 12, 3);
  StoreU32(base, p + 16, 4);
  StoreU32(base, p + 0, kMinusOne);
  // The controller pages' open message by the mode type.
  switch (int32_t(LoadU32(base, kModeType))) {
    case 2: Post(base, dispatcher, 16); break;
    case 4: Post(base, dispatcher, 12); break;
    case 6: Post(base, dispatcher, 27); break;
    case 7: Post(base, dispatcher, 24); break;
    case 8: Post(base, dispatcher, 23); break;
    default: break;
  }
  return r3;
}

}  // namespace options_screen
